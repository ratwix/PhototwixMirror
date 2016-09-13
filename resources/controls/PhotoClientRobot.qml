import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebSockets 1.0

Item {
    id:phototwixClientItem
    anchors.fill: parent

    property alias client : phototwixClient;

    Column {
        Button {
            text:"Send Finish"
            onClicked: {
                if (phototwixClient.status == WebSocket.Open) {
                    var message= JSON.stringify({ //TODO
                          photoProcessResult:{
                            templateName: "myTemplateName",
                            effectName: "Noir et Blanc",
                            nbPhoto: 3,
                            photosPath: "/tmp/",
                            photos: [
                                {fileName:"phototwix_1.jpg"},
                                {fileName:"phototwix_2.jpg"},
                                {fileName:"phototwix_3.jpg"}
                            ]
                          }
                        });
                    phototwixClient.sendTextMessage(message);
                }
            }
        }
    }

    WebSocket {
            id: phototwixClient
            url: "ws://127.0.0.1:54345" //TODO : IP parameter
            onTextMessageReceived: {
                var j = JSON.parse(message);
                var templateName = j.startGlobalPhotoProcess.templateName;
                //TODO: search template from templateName
                var effectName = j.startGlobalPhotoProcess.effectName;
                var nbPhoto = j.startGlobalPhotoProcess.nbPhoto;
                mirrorScreen.startGlobalPhotoProcess(templateName, effectName, nbPhoto)
            }
            onStatusChanged: if (phototwixClient.status == WebSocket.Error) {
                                 console.log("Error: " + phototwixClient.errorString)
                             } else if (phototwixClient.status == WebSocket.Open) {
                                 //TODO: mettre un indicateur
                                 console.log("Session open")
                             } else if (phototwixClient.status == WebSocket.Closed) {
                                 console.log("Socket closed");
                             }
            active: false
    }

    Timer {
        interval: 4000
        triggeredOnStart: true
        running: true
        repeat: true
        onTriggered: {
            if ((phototwixClient.status != WebSocket.Open) && (phototwixClient.status != WebSocket.Connecting)) {
                phototwixClient.active = false;
                phototwixClient.active = true;
            }
        }
    }
}
