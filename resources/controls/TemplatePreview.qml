import QtQuick 2.5
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

Item {
    id:templatePreview
    width: photoTemplate.width
    property Photo photo


    DelegateModel {
        id: photoPartModel
        model:photo ? photo.photoPartList : null
        delegate: PhotoShootRenderer {
            y: photoTemplate.y + photoTemplate.height * modelData.photoPosition.y
            x: photoTemplate.x + photoTemplate.width * modelData.photoPosition.x + photoTemplate.width * modelData.photoPosition.width * modelData.photoPosition.xphoto
            xphoto: modelData.photoPosition.xphoto
            height: photoTemplate.height * modelData.photoPosition.height
            width: photoTemplate.width * modelData.photoPosition.width - photoTemplate.width * modelData.photoPosition.width * modelData.photoPosition.xphoto * 2
            rotation: modelData.photoPosition.rotate
            effectSource: photo.tweeter ? parameters.effectDefault : globalVar.currentEffect
        }
    }

    Repeater {
        id:photoPartRepeater
        anchors.fill: parent
        model:photoPartModel
    }

    Image { //Back template
        id: photoTemplate
        fillMode: Image.PreserveAspectCrop
        source: photo ? photo.currentTemplate.url : ""
        height: templatePreview.height
        width: photoTemplate.sourceSize.width / photoTemplate.sourceSize.height * templatePreview.height  //TODO : add for test
        cache: true
        asynchronous: false
        antialiasing: true
    }

    Text {
        id:twitterTextMessage
        anchors.bottom: photoTemplate.bottom
        anchors.bottomMargin: 10
        anchors.right: photoTemplate.right
        anchors.rightMargin: 10
        color: photo ? photo.currentTemplate.twitterMessageColor : "#000000"
        width: parent.width * 0.6
        height: parent.height * 0.15
        horizontalAlignment: Text.AlignRight
        fontSizeMode: Text.Fit
        minimumPixelSize: 10
        font.pixelSize: 72
        wrapMode: Text.WordWrap
        font.family: "Gabrielle"
        text: photo ? cleanTwitterMessage(photo.tweeterMessage) : ""
    }

    Text {
        id:twitterTextId
        anchors.bottom: twitterTextMessage.top
        anchors.right: parent.right
        anchors.rightMargin: 10
        color: photo ? photo.currentTemplate.twitterMessageColor : "#000000"
        width: parent.width * 0.6
        height: parent.height * 0.04
        horizontalAlignment: Text.AlignRight
        fontSizeMode: Text.Fit
        minimumPixelSize: 5
        font.pixelSize: 72
        text:photo ? "@" + photo.photoTweeterProfileName : "" //TODO
    }



    //Quand on change la photo, regarder si l'image a déjà été gérérée. Si non, générer l'image, mettre à jour, et serialiser
    //Lance un timer car l'image peut ne pas être chargée
    onPhotoChanged: {
        if (photo) {
            if (photo.finalResult == "") {
                waitLoadingTimer.start()
            }
        }
    }

    Timer {
        id:waitLoadingTimer
        interval: 1000
        running: false
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            if (photoTemplate.sourceSize.width > 0) {
                waitLoadingTimer.stop()
                saveImageToFile();
            }
        }
    }

    function cleanTwitterMessage(message) {
        var result = message
        //Remove URL
        result = result.replace(/(?:https?|ftp):\/\/[\n\S]+/g, '');
        //Remove hash tag
        if (parameters.twitterTag !== "") {
            var regexp = new RegExp(parameters.twitterTag,'g');
            result = result.replace(regexp, '');
        }

        if (parameters.twitterAccount !== "") {
            var regexp = new RegExp(parameters.twitterAccount,'g');
            result = result.replace(regexp, '');
        }
        return result
    }

    function saveImageToFile() {
        var photoHeighP = 6;
        var dpi = 300;
        var firstsave = 0;

        function saveImage(result) {
            var d = new Date();
            var date = d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d.getDate() + "_" + d.getHours() + "h" + d.getMinutes() + "m" + d.getSeconds() + "s"
            var imageNamePrep = photo.tweeter ? "phototwix-tweeter-" : "phototwix-";
            var imageName = imageNamePrep + date + ".png"
            var path = applicationDirPath + "/photos/" + imageName;
            result.saveToFile(path);
            photo.name = d.getDate() + "/" + (d.getMonth() + 1) + "/" + d.getFullYear() + " " + d.getHours() + "h" + d.getMinutes() + "m" + d.getSeconds() + "s"; //save image name
            photo.finalResult = "file:///" + path; //save image path
            if (firstsave == 0) {
                firstsave = 1;
            } else {
                //La photo est finalisée, on l'ajoute a la gallerie
                parameters.photoGallery.prependPhoto(photo);
                //parameters.photoGallery.Serialize();
            }
        }

        function saveImageSD(result) {
            var d = new Date();
            var date = d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d.getDate() + "_" + d.getHours() + "h" + d.getMinutes() + "m" + d.getSeconds() + "s"
            var imageNamePrep = photo.tweeter ? "phototwix-tweeter-" : "phototwix-";
            var imageName = imageNamePrep + date + ".png"
            var path = applicationDirPath + "/photos/sd/" + imageName;
            result.saveToFile(path);
            photo.finalResultSD = "file:///" + path; //save image path
            if (firstsave == 0) {
                firstsave = 1;
            } else {    
                parameters.photoGallery.prependPhoto(photo);
                //parameters.photoGallery.Serialize();
            }
        }

        //HQ
        templatePreview.grabToImage(saveImage, Qt.size(templatePreview.width, templatePreview.height));
        templatePreview.grabToImage(saveImageSD, Qt.size(templatePreview.width / 10, templatePreview.height / 10));
        /*
        templatePreview.grabToImage(saveImage, Qt.size(photoTemplate.sourceSize.width, photoTemplate.sourceSize.height));
        templatePreview.grabToImage(saveImageSD, Qt.size(photoTemplate.sourceSize.width / 5, photoTemplate.sourceSize.height / 5));
        */
    }



}
