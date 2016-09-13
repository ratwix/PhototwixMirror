import QtQuick 2.0

import "./resources/controls"

Item {
    id:commandScreenItem
    property alias viewResultScreenPhoto :viewResultScreen.currentPhoto
    state:"CHOOSE_TEMPLATE"

    // To be sure that an unwanted click do something
    MouseArea {
        anchors.fill: parent
    }

    //Background image
    Image {
        id: backgroundImage
        anchors.fill: parent
        fillMode:Image.Stretch
        source: parameters.backgroundImage
    }


    //Manual Control Item
    Item {
        id:chooseTemplateItem
        anchors.fill: parent

        ChooseTemplateControl {
            id:chooseTemplateListView
            height: parent.height * 0.5
            width: parent.width * 0.5 - 100
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top:parent.top
            anchors.topMargin: 20
            visible:!parameters.twitterTwitterOnly
        }

        ChooseEffectControl {
            id:chooseEffectListView
            height: parent.height * 0.5
            width: parent.width * 0.5 - 100
            anchors.left: chooseTemplateListView.right
            anchors.leftMargin: 100
            anchors.top:parent.top
            anchors.topMargin: 20
            visible:!parameters.twitterTwitterOnly
        }
        //Twitter message if twitter box only
        Item {
            id: tweeterText
            height: parent.height * 0.5
            width: parent.width * 0.5
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            visible:parameters.twitterTwitterOnly


            Text {
                anchors.fill: parent
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
                font.pixelSize:height / 6
                elide: Text.ElideMiddle
                color:parameters.twitterMessageColor
                text: qsTr(parameters.twitterMessage).arg(parameters.twitterAccount).arg(parameters.twitterTag)
            }
        }

        //Twitter message if not twitter box and twitter active

        Item {
            id: tweeterTextSmall
            height: parent.height * 0.1
            width: parent.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: choosePhotoView.top
            anchors.bottomMargin: 5
            visible:!parameters.twitterTwitterOnly && parameters.twitterListenTwitter


            Text {
                anchors.fill: parent
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
                fontSizeMode:Text.HorizontalFit
                elide: Text.ElideMiddle
                minimumPixelSize: 5
                font.pixelSize: height / 2
                color:parameters.twitterMessageColor
                text: qsTr(parameters.twitterMessage.replace(/\n/g, " ")).arg(parameters.twitterAccount).arg(parameters.twitterTag)
            }
        }

        ChoosePhotoControl {
            id:choosePhotoView
            height: parent.height * 0.4
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }
        //Bouton de config
        ButtonAwesome {
            id:configButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            size:parent.height * 0.05
            code:"\uf085"
            onClicked: {
                passScreen.state = "show"
            }
        }

        //Nombre de photos dans la queue
        Rectangle {
            id:queueNumber
            visible: parameters.twitterListenTwitter
            anchors.top: parent.top
            anchors.right: configButton.left
            anchors.topMargin: 10
            anchors.rightMargin: 10
            height: parent.height * 0.05
            width: height
            radius: parent.height * 0.01
            color: parameters.photoQueueManager.canPop ? "#97BF41" : "#A60311"
            border.color: "black"
            border.width: 2

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                fontSizeMode: Text.Fit
                text: parameters.photoQueueManager.nbPhotoInQueue
            }
        }

        //Connexion Wifi
        Text {
            height: parent.height * 0.05
            width: height
            anchors.top: parent.top
            anchors.right: queueNumber.left
            anchors.topMargin: 10
            anchors.rightMargin: 10
            font.pixelSize: height
            font.family: "FontAwesome"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "\uf1eb "
            color:(parameters.wifiManager.connectWifiQuality > 75) ? "green" :  ((parameters.wifiManager.connectWifiQuality > 45) ? "orange" : (parameters.wifiManager.connectWifiQuality > 0) ? "red" : "black")
        }
    }

    ResultScreen {
        id:viewResultScreen
        anchors.fill: parent
        visible: false
    }



    Item {
         id:configItem
         anchors.fill: parent

         //Mot de passe pour aller au panneau de configuration
         PasswordScreen {
             id: passScreen
             onSuccess: {
                 configScreen.admin = false
                 commandScreenItem.state = "CONFIG"
             }

             onSuccessAdmin: {
                 configScreen.admin = true
                 commandScreenItem.state = "CONFIG"
             }
         }

         //Ecran de configuration
         ConfigScreen {
             id: configScreen
             anchors.fill: parent
             visible: false
         }
    }

    MessageScreen {
        id:mbox
    }

    ConfirmScreen {
        id:cbox
    }

    //Robot tweeter
    TwitterRobot {
        id:tweeter
        visible: false
    }

    //Robot de connection au Mirroir (server)
    PhotoServerRobot {
        id:photoServerRobot
        visible:false
    }

    //Timer pour depiler toute les PhotoShootRenderer
    Timer {
        id:timerPopPhoto
        interval: 2000
        running: commandScreenItem.state == "CHOOSE_TEMPLATE"
        repeat: true
        triggeredOnStart: false

        onTriggered: {
            parameters.photoQueueManager.pop()
        }
    }

    //Affiche une photo qui a été ajoutée a la gallerie
    //Démarage de l'auto print, qui imprimera la photo si autoprint actif
    Connections {
        target: parameters.photoGallery
        onShowPhoto : {
            //console.debug("On a reçu une nouvelle prise de vue")
            viewResultScreen.currentPhoto = photo
            commandScreenItem.state = "RESULT_PHOTO"
            //console.debug("Démarage du timer de prise de photo et autoprint")
            viewResultScreen.timerStartPrint.start()
            //console.debug("Démarage de clignottement")
            parameters.raspiGPIO.blink(3000,200)
        }
    }

    //Quand on essaye de se connecter au wifi
    Connections {
        target: parameters.wifiManager
        onWifiTryConnect : {
            mbox.message = "Connexion à " + wifiName
            mbox.imageTag = "\uf1eb"
            mbox.state = "show"
        }
    }

    //Quand on passe a la vision d'une photo, si le timer pour voir une photo est > a 0, alors on enclanche le timer
    onStateChanged: {
        if (state == "CONFIG") {
            parameters.showCursor();
        } else {
            parameters.hideCursor();
        }
        //On a quitté la preview volontairement, on arette l'auto print et auto close
        if (state == "RESULT_PHOTO") {
            viewResultScreen.timerStartClose.start();
        } else {
            viewResultScreen.timerPrint.stop()
            viewResultScreen.timerShow.stop()
        }
    }

    states: [
        State {
            name: "CHOOSE_TEMPLATE"
        },
        State {
            name: "CONFIG"
            PropertyChanges { target: configScreen; visible:true}
            PropertyChanges { target: chooseTemplateItem; visible: false}
        },
        State {
            name: "RESULT_PHOTO"
            PropertyChanges {target: chooseTemplateItem; visible: false}
            PropertyChanges {target: viewResultScreen; visible: true}
        }

    ]
}
