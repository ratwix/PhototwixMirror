import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebSockets 1.0

Item {
    id:phototwixServerItem
    anchors.fill: parent

    property WebSocket clientSocket: null

    /* TODO: Connecter a un signal d'un press button
    Row {
        Button {
            text:"Start Photo"
            onClicked: {
                phototwixServerItem.sendStartGlobalPhotoProcess();
            }
        }
    }
    */

    WebSocketServer {
        id: phototwixServer
        listen: true
        port:54345
        onClientConnected: {
            phototwixServerItem.clientSocket = webSocket
            webSocket.onTextMessageReceived.connect(
                function(message) {
                    //console.log("Server received message: " + message);
                    phototwixServerItem.getResultPhotoProcess(message);
                }
            );
            webSocket.onStatusChanged.connect (
                function(status) {
                    if (status === webSocket.Open) {
                        phototwixServerItem.clientSocket = webSocket;
                    } else {
                        console.debug("Client not connected");
                        phototwixServerItem.clientSocket = null;
                    }
                }
            )

            console.log("Client connected");
        }
        onErrorStringChanged: {
            console.log("Server error: " + errorString);
        }
        onListenChanged: {
            console.log("Client url : " + phototwixServer.url);
        }
    }

    function sendStartGlobalPhotoProcess() {
        var message = JSON.stringify({
                                         startGlobalPhotoProcess: {
                                             templateName: "myTemplateName", //TODO:
                                             effectName: "Noir et Blanc",   //TODO:
                                             nbPhoto: 3                     //TODO
                                         }

                                     });

        if ((phototwixServerItem.clientSocket != null) && (phototwixServerItem.clientSocket.status == WebSocket.Open)) {
            phototwixServerItem.clientSocket.sendTextMessage(message);
        } else {
            console.error("No connection to mirror")
        }
    }

    function getResultPhotoProcess(message) {
        //Get the json message
        var j = JSON.parse(message);
        //extract IP url from
        var surl = phototwixServerItem.clientSocket.url.toString();
        surl = surl.match(".*://(.*):.*")[1];
        j.photoProcessResult["serverIP"] = surl;
        message = JSON.stringify(j);
        //parameters.photoQueueManager.pushMirror(message); Push directement du message JSON, on parse en c++
        parameters.photoQueueManager.pushTwitter("photo1_mirror", message);
    }
}
