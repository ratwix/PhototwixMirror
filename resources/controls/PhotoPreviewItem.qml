import QtQuick 2.5
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import com.phototwix.components 1.0

Item {
    id:photoPreviewItem
    property Photo photo
    width: photoImage.status === Image.Ready ? photoImage.width : 0
    //height: parent.height

     MouseArea {
        anchors.fill: parent
        onClicked: {
            commandScreenItem.viewResultScreenPhoto = photo
            commandScreenItem.state = "RESULT_PHOTO"
        }
    }

    Image {
        id:photoImage
        height: parent.height
        //width: (photoImage.sourceSize.width / photoImage.sourceSize.height) * parent.height
        fillMode: Image.PreserveAspectFit //TODO: Test
        asynchronous: true
        cache: false
        source: photo ? photo.finalResultSD : ""
    }

    Text {
        color: "#00aced"
        height: photoImage.height * 0.2
        width: height
        anchors.top : photoImage.top
        anchors.right: photoImage.right
        fontSizeMode: Text.Fit
        font.pixelSize: height
        font.family: "FontAwesome"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "\uf081"
        visible: photo ? photo.tweeter : false
    }

    BusyIndicator {
        anchors.verticalCenter: photoImage.verticalCenter
        anchors.horizontalCenter: photoImage.horizontalCenter
        height: photoImage.height * 0.5
        width: photoImage.height * 0.5
        running: photoImage.status === Image.Loading
    }
}
