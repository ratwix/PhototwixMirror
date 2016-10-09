import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtWebSockets 1.0

Item {
    id:phototwixServerItem
    anchors.fill: parent

    property WebSocket clientSocket: null

    //Connection to Action button
    Connections {
        target: parameters.raspiGPIO
        onActionButtonPushed: {
            console.debug("Receive command button");
            sendStartGlobalPhotoProcess(clientSocket);
        }
    }

    WebSocketServer {
        id: phototwixServer
        listen: true
        port:54345
        host: parameters.commandIP
        accept: true

        onErrorStringChanged: {
            console.error("Error server:" + errorString)
        }

        onUrlChanged: {
            console.debug("Server URL : " + url)
        }

        onClientConnected: {
            phototwixServerItem.clientSocket = webSocket
            webSocket.onTextMessageReceived.connect(
                function(message) {
                    //console.debug("Server received message: " + message);
                    if (message == "askPhoto") {
                        console.debug("Ask start photo from mirror");
                        sendStartGlobalPhotoProcess(webSocket);
                    } else {
                        console.debug("Get photo result");
                        phototwixServerItem.getResultPhotoProcess(webSocket, message);
                    }
                }
            );
            webSocket.onStatusChanged.connect (
                function(status) {
                    if (status === webSocket.Open) {
                        phototwixServerItem.clientSocket = webSocket;
                        parameters.mirrorConnected = true;
                    } else {
                        console.debug("Client not connected");
                        phototwixServerItem.clientSocket = null;
                        parameters.mirrorConnected = false;
                    }
                }
            )

            console.log("Client connected");
            parameters.mirrorConnected = true;
        }
    }

    function sendStartGlobalPhotoProcess(socket) {
        var message = JSON.stringify({
                                         startGlobalPhotoProcess: {
                                             templateName: globalVar.currentTemplate.name,
                                             effectName: globalVar.currentEffect,
                                             nbPhoto: globalVar.currentTemplate.nbPhotos()
                                         }

                                     });
        if ((socket != null) && (socket.status == WebSocket.Open)) {
            socket.sendTextMessage(message);
        } else {
            console.error("No connection to mirror")
        }
    }

    function getResultPhotoProcess(socket, message) {
        //Get the json message
        var j = JSON.parse(message);
        j.photoProcessResult["clientIP"] = parameters.mirrorIP;
        message = JSON.stringify(j);
        parameters.photoQueueManager.pushMirror("photo1_mirror", message);
    }
}
