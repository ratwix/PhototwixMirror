#ifndef COMMON_H
#define COMMON_H

#include <sstream>

#define VIDEO_FOLDER        "video"
#define PHOTOS_FOLDER       "photos"
#define PHOTOSSD_FOLDER     "photos/sd"
#define PHOTOSS_FOLDER      "photos/single"
#define PHOTOSD_FOLDER      "photos/deleted"
#define PHOTOSDS_FOLDER     "photos/deleted/single"
#define TEMPLATE_FOLDER     "templates"
#define BACKGROUND_FOLDER   "background"

#define SCRIPT_CONNECT_WIFI "scripts/connectWifi.sh"
#define SCRIPT_CHANGE_IP_ETH0 "scripts/connectEth.sh"
#define SCRIPT_GET_PAPER    "scripts/dnpds40"
#define SCRIPT_PRINT        "scripts/print.sh"

#define VIDEO_PATH          string(m_applicationDirPath.toString().toStdString() + "/video").c_str()
#define PHOTOS_PATH         string(m_applicationDirPath.toString().toStdString() + "/photos").c_str()
#define PHOTOSSD_PATH       string(m_applicationDirPath.toString().toStdString() + "/photos/sd").c_str()
#define PHOTOSS_PATH        string(m_applicationDirPath.toString().toStdString() + "/photos/single").c_str()
#define PHOTOSD_PATH        string(m_applicationDirPath.toString().toStdString() + "/photos/deleted").c_str()
#define PHOTOSDS_PATH       string(m_applicationDirPath.toString().toStdString() + "/photos/deleted/single").c_str()
#define TEMPLATE_PATH       string(m_applicationDirPath.toString().toStdString() + "/templates").c_str()
#define BACKGROUND_PATH     string(m_applicationDirPath.toString().toStdString() + "/background").c_str()
#define CONFIG_FILE         string(m_applicationDirPath.toString().toStdString() + "/config.json").c_str()
#define GALLERY_FILE        string(m_applicationDirPath.toString().toStdString() + "/gallery.json").c_str()

enum VideoType {
    VIDEO_WAIT = 0,
    VIDEO_STARTGLOBALPHOTOPROCESS,
    VIDEO_STARTPHOTOPROCESS,
    VIDEO_ENDGLOBALPHOTOPROCESS,
    VIDEO_TWITTER
};

static inline std::string itos(int i) {
    std::ostringstream tmp;
    tmp  << i;
    return tmp.str();
}


#endif // COMMON_H
