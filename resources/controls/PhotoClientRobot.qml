import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebSockets 1.0

Item {
    id:phototwixClientItem
    anchors.fill: parent

    property alias client : phototwixClient;
    property bool photoInProgress: false

    /*
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
    */

    WebSocket {
            id: phototwixClient
            url: "ws://127.0.0.1:54345" //TODO : IP parameter
            onTextMessageReceived: {
                if (!phototwixClientItem.photoInProgress) {
                    phototwixClientItem.photoInProgress = true;
                    mirrorScreen.receiveMessage(message);
                }

            }
            onStatusChanged: if (phototwixClient.status == WebSocket.Error) {
                                 console.log("Error: " + phototwixClient.errorString)
                                 parameters.mirrorConnected = false;
                             } else if (phototwixClient.status == WebSocket.Open) {
                                 console.log("Session open");
                                 parameters.mirrorConnected = true;
                             } else if (phototwixClient.status == WebSocket.Closed) {
                                 console.log("Socket closed");
                             } else {
                                 parameters.mirrorConnected = false;
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
