#include <QDebug>
#include <QFile>
#include <string.h>

#include "clog.h"
#include "cameraworker.h"

#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#define VIDEO_OUTPUT_BUFFERS_NUM 3

//TODO: Rechercher automatiquement la meilleur resolution
#define CAMERA_WIDTH    3280    //2592
#define CAMERA_HEIGHT   2464    //1944

#define CAMERA_PREVIEW_4x3_WIDTH    1296    //1024
#define CAMERA_PREVIEW_4x3_HEIGHT   972     //768

#define CAMERA_PREVIEW_16x9_WIDTH   1280
#define CAMERA_PREVIEW_16x9_HEIGHT  720

typedef struct
{
    MMAL_POOL_T      *encoder_pool;
    QFile            *result_file;
    VCOS_SEMAPHORE_T complete_semaphore;
} PORT_USERDATA;

/**
 *  buffer header callback function for camera control
 *
 *  No actions taken in current version
 *
 * @param port Pointer to port from which callback originated
 * @param buffer mmal buffer header pointer
 */
static void camera_control_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
{
    if (buffer->cmd == MMAL_EVENT_PARAMETER_CHANGED) {
       MMAL_EVENT_PARAMETER_CHANGED_T *param = (MMAL_EVENT_PARAMETER_CHANGED_T *)buffer->data;
       switch (param->hdr.id) {
          case MMAL_PARAMETER_CAMERA_SETTINGS:
          {
             CLog::Write(CLog::Info, "PICAM: Camera control callback error");
          }
          break;
       }
    }
    else if (buffer->cmd == MMAL_EVENT_ERROR) {
        CLog::Write(CLog::Info, "PICAM: No data received from sensor. Check all connections, including the Sunny one on the camera board");
    }
    else {
        CLog::Write(CLog::Info, QString("PICAM: Received unexpected camera control callback event") + buffer->cmd);
    }

    mmal_buffer_header_release(buffer);
}

/**
 * @brief encoder_buffer_callback
 * @param port
 * @param buffer
 *
 * Get and encode e capture un JPEG
 */
static void encoder_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
{
    CLog::Write(CLog::Debug, "PICAM: Receive encoder_buffer_callback");

    PORT_USERDATA *pData = (PORT_USERDATA *)port->userdata;

    int complete = 0;

    if (!pData) {
        CLog::Write(CLog::Error, "PICAM: No used data");
        return;
    }

    QFile *file = pData->result_file;

    if (buffer->length && file && file->isOpen())
    {
        mmal_buffer_header_mem_lock(buffer);
        if (file->write((char*) buffer->data, buffer->length) < buffer->length) {
            CLog::Write(CLog::Error, "PICAM: Unable to write buffer to file - aborting");
        }
        //file.flush();
        mmal_buffer_header_mem_unlock(buffer);
    }

    // Now flag if we have completed
    if (buffer->flags & (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED))
        complete = 1;
    else {
        CLog::Write(CLog::Debug, "PICAM: Received a encoder buffer callback with no state");
    }

    // release buffer back to the pool
    mmal_buffer_header_release(buffer);

    // and send one back to the port (if still open)
    if (port->is_enabled)
    {
       MMAL_STATUS_T status = MMAL_SUCCESS;
       MMAL_BUFFER_HEADER_T *new_buffer;

       new_buffer = mmal_queue_get(pData->encoder_pool->queue);

       if (new_buffer) {
          status = mmal_port_send_buffer(port, new_buffer);
       }
       if (!new_buffer || status != MMAL_SUCCESS) {
           CLog::Write(CLog::Error, "PICAM: Unable to return a buffer to the encoder port");
       }
    }

    if (complete)
        vcos_semaphore_post(&(pData->complete_semaphore));
}

CameraWorker::CameraWorker(QQmlImageProviderBase::ImageType type, QQmlImageProviderBase::Flags flags):
    QQuickImageProvider(type, flags)
{
    m_camera_enabled = false;
    openCamera();
}

CameraWorker::~CameraWorker()
{
    closeCamera();
    m_camera_enabled = false;
}

MMAL_STATUS_T CameraWorker::connect_ports(MMAL_PORT_T *output_port, MMAL_PORT_T *input_port, MMAL_CONNECTION_T **connection)
{
    MMAL_STATUS_T status;

   status =  mmal_connection_create(connection, output_port, input_port, MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);

   if (status == MMAL_SUCCESS)
   {
      status =  mmal_connection_enable(*connection);
      if (status != MMAL_SUCCESS)
         mmal_connection_destroy(*connection);
   }

   return status;
}

void CameraWorker::initCamera()
{
    MMAL_COMPONENT_T *camera = 0;
    MMAL_ES_FORMAT_T *format;
    MMAL_STATUS_T status;

    if ((status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Failed to create camera component"); //TODO check status
        return;
    }

    if (!camera->output_num)	{
        status = MMAL_ENOSYS;
        CLog::Write(CLog::Error, "PICAM: Camera doesn't have output ports"); //TODO check status
        mmal_component_destroy(camera);
        return;
    }

    m_preview_port = camera->output[MMAL_CAMERA_PREVIEW_PORT];
    m_still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];
    m_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];

    // Enable the camera, and tell it its control callback function
    if ((status = mmal_port_enable(camera->control, camera_control_callback)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, QString("PICAM: Unable to enable control port") + QString::number(status)); //TODO check status
        mmal_component_destroy(camera);
        return;
    }

    //Initialisation de la camera
    MMAL_PARAMETER_CAMERA_CONFIG_T cam_config;
    /*
    cam_config.hdr = MMAL_PARAMETER_CAMERA_CONFIG;
    cam_config.hdr.size = sizeof(cam_config);
    */
    cam_config.max_stills_w = CAMERA_WIDTH;
    cam_config.max_stills_h = CAMERA_HEIGHT;
    cam_config.stills_yuv422 = 0;
    cam_config.one_shot_stills = 1;
    cam_config.max_preview_video_w = CAMERA_PREVIEW_4x3_WIDTH;
    cam_config.max_preview_video_h = CAMERA_PREVIEW_4x3_HEIGHT;
    cam_config.num_preview_video_frames = 3;
    cam_config.stills_capture_circular_buffer_height = 0;
    cam_config.fast_preview_resume = 0;
    cam_config.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;
    status = mmal_port_parameter_set(camera->control, &cam_config.hdr);

    // Now set up the still port formats
    format = m_still_port->format;
    format->encoding = MMAL_ENCODING_OPAQUE;
    format->es->video.width = VCOS_ALIGN_UP(CAMERA_WIDTH, 32);
    format->es->video.height = VCOS_ALIGN_UP(CAMERA_HEIGHT, 16);
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = CAMERA_WIDTH;
    format->es->video.crop.height = CAMERA_HEIGHT;
    format->es->video.frame_rate.num = STILLS_FRAME_RATE_NUM;
    format->es->video.frame_rate.den = STILLS_FRAME_RATE_DEN;

    if ((status = mmal_port_format_commit(m_still_port)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: camera still format couldn't be set");
        mmal_component_destroy(camera);
        return;
    }

    if (m_still_port->buffer_num < VIDEO_OUTPUT_BUFFERS_NUM)
        m_still_port->buffer_num = VIDEO_OUTPUT_BUFFERS_NUM;

    // Enable component
    if ((status = mmal_component_enable(camera)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: camera component couldn't be enabled");
        mmal_component_destroy(camera);
        return;
    }
    m_camera = camera;

    CLog::Write(CLog::Debug, "PICAM: Camera component done");

    init_still_encoder();

    m_camera_enabled = true;
}


void CameraWorker::init_still_encoder()
{
    MMAL_STATUS_T status;
    MMAL_COMPONENT_T *encoder = 0;
    MMAL_PORT_T *encoder_input = NULL, *encoder_output = NULL;
    MMAL_POOL_T *pool;

    if ((status = mmal_component_create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER, &encoder)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Unable to create JPEG encoder component");
        if (encoder) {mmal_component_destroy(encoder);}
        return;
    }

    if (!encoder->input_num || !encoder->output_num) {
        CLog::Write(CLog::Error, "PICAM: JPEG encoder doesn't have input/output ports");
        if (encoder) {mmal_component_destroy(encoder);}
        return;
    }

    encoder_input = encoder->input[0];
    encoder_output = encoder->output[0];

    // We want same format on input and output
    mmal_format_copy(encoder_output->format, encoder_input->format);
    // Specify out output format
    encoder_output->format->encoding = MMAL_ENCODING_JPEG; //MMAL_ENCODING_PNG, MMAL_ENCODING_JPEG, MMAL_ENCODING_BMP, MMAL_ENCODING_GIF
    encoder_output->buffer_size = encoder_output->buffer_size_recommended;

    if (encoder_output->buffer_size < encoder_output->buffer_size_min)
        encoder_output->buffer_size = encoder_output->buffer_size_min;

    encoder_output->buffer_num = encoder_output->buffer_num_recommended;

    if (encoder_output->buffer_num < encoder_output->buffer_num_min)
        encoder_output->buffer_num = encoder_output->buffer_num_min;

    // Commit the port changes to the output port
    if ((status = mmal_port_format_commit(encoder_output)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Unable to set format on video encoder output port");
        if (encoder) {mmal_component_destroy(encoder);}
        return;
    }

    // Set the JPEG quality level
    if ((status = mmal_port_parameter_set_uint32(encoder_output, MMAL_PARAMETER_JPEG_Q_FACTOR, PICAM_JPEG_QUALITY)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Unable to set JPEG quality");
        if (encoder) {mmal_component_destroy(encoder);}
        return;
    }

    // Enable component
    if ((status = mmal_component_enable(encoder)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Unable to enable video encoder component");
        if (encoder) {mmal_component_destroy(encoder);}
        return;
    }

    //Create pool of buffer headers for the output port to consume
    pool = mmal_port_pool_create(encoder_output, encoder_output->buffer_num, encoder_output->buffer_size);
    if (!pool) {
        CLog::Write(CLog::Error, QString("PICAM: Failed to create buffer header pool for encoder output port") + encoder_output->name);
    }

    m_encoder_pool = pool;
    m_encoder_component = encoder;

    CLog::Write(CLog::Debug, "PICAM: Encoder component done");

    CLog::Write(CLog::Debug, "PICAM: Connecting camera stills port to encoder input port");


    // Now connect the camera to the encoder

    if ((status = connect_ports(m_still_port, m_encoder_component->input[0], &m_encoder_connection)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Failed to connect camera video port to encoder input");
        freeCamera();
        return;
    }

    CLog::Write(CLog::Debug, "PICAM: Connect Camera Encoder Done");
}

void CameraWorker::capturePhoto(const QString &fileName)
{
    MMAL_PORT_T *encoder_output_port = NULL;

    if (!m_camera_enabled)
        return;

    encoder_output_port = m_encoder_component->output[0];

    //No exif data
    mmal_port_parameter_set_boolean(m_encoder_component->output[0], MMAL_PARAMETER_EXIF_DISABLE, 1);
    /*
    // There is a possibility that shutter needs to be set each loop.
    if (mmal_status_to_int(mmal_port_parameter_set_uint32(m_camera->control, MMAL_PARAMETER_SHUTTER_SPEED, state.camera_parameters.shutter_speed)) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, "PICAM: Unable to set shutter speed");
    }
    */
    // Enable the encoder output port
    PORT_USERDATA callback_data;

    VCOS_STATUS_T vcos_status = vcos_semaphore_create(&callback_data.complete_semaphore, "RaspiStill-sem", 0);
    vcos_assert(vcos_status == VCOS_SUCCESS);
    callback_data.encoder_pool = m_encoder_pool;
    encoder_output_port->userdata = (struct MMAL_PORT_USERDATA_T *)&callback_data;


    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        CLog::Write(CLog::Error, QString("PICAM: unable to open file ") + fileName);

    callback_data.result_file = &file;

    mmal_port_enable(encoder_output_port, encoder_buffer_callback);

    // Send all the buffers to the encoder output port
    int num = mmal_queue_length(m_encoder_pool->queue);

    for (int q = 0; q < num; q++)
    {
       MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(m_encoder_pool->queue);
       if (!buffer)
           CLog::Write(CLog::Error, QString("PICAM: Unable to get a required buffer ") + QString::number(q) + " from pool queue");

       if (mmal_port_send_buffer(encoder_output_port, buffer)!= MMAL_SUCCESS)
           CLog::Write(CLog::Error, QString("PICAM: Unable to send a buffer to encoder output port (") + QString::number(q) + ")");
    }

    // Starting capture
    CLog::Write(CLog::Debug, "PICAM: Starting capture");

    if (mmal_port_parameter_set_boolean(m_still_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
        CLog::Write(CLog::Error, QString("Failed to start capture"));
    } else {
        vcos_semaphore_wait(&callback_data.complete_semaphore);
        CLog::Write(CLog::Debug, "PICAM: Finished Capture");
        emit imageCaptured(fileName);
        file.close();
    }

    mmal_port_disable(encoder_output_port);
    vcos_semaphore_delete(&callback_data.complete_semaphore);
}


void CameraWorker::freeCamera()
{
    m_camera_enabled = false;
    CLog::Write(CLog::Debug, "PICAM: Closing down");

    if (m_still_port && m_still_port->is_enabled)
        mmal_port_disable(m_still_port);

    if (m_video_port && m_video_port->is_enabled)
        mmal_port_disable(m_video_port);

    if (m_preview_port && m_preview_port->is_enabled)
        mmal_port_disable(m_preview_port);

    if (m_encoder_component)
        mmal_component_disable(m_encoder_component);

    if (m_encoder_connection)
        mmal_connection_destroy(m_encoder_connection);

    if (m_camera)
        mmal_component_disable(m_camera);

    if (m_encoder_pool)
        mmal_port_pool_destroy(m_encoder_component->output[0], m_encoder_pool);

    if (m_encoder_component)
        mmal_component_destroy(m_encoder_component);

    if (m_camera)
        mmal_component_destroy(m_camera);

    CLog::Write(CLog::Debug, "PICAM: Close down completed, all components disconnected, disabled and destroyed");
}

void CameraWorker::openCamera()
{
    initCamera();
}

void CameraWorker::closeCamera()
{
    freeCamera();
}

QImage CameraWorker::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return m_preview_result;
}

QImage CameraWorker::preview_result() const
{
    return m_preview_result;
}

void CameraWorker::setPreview_result(const QImage &preview_result)
{
    m_preview_result = preview_result;
}
