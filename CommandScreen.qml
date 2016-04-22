import QtQuick 2.0

import "./resources/controls"

Item {
    id:commandScreenItem

    state:"CHOOSE_TEMPLATE"

    // To be sure that an unwanted click do something
    MouseArea {
        anchors.fill: parent
    }

    Item {
        id:chooseTemplateItem
        anchors.fill: parent

        //Background image
        Image {
            id: backgroundImage
            anchors.fill: parent
            fillMode:Image.Stretch
            source: parameters.backgroundImage
        }

        ChooseTemplateControl {
            id:chooseTemplateListView
            height: parent.height * 0.5
            width: parent.width
            anchors.left: parent.left
            anchors.top:parent.top
            anchors.topMargin: 20
        }

        ButtonAwesome {
            id:configButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            size:40
            code:"\uf085"
            onClicked: {
                passScreen.state = "show"
            }
        }


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

    states: [
        State {
            name: "CHOOSE_TEMPLATE"
        },
        State {
            name: "CONFIG"
            PropertyChanges { target: configScreen; visible:true}
            PropertyChanges { target: chooseTemplateItem; visible: false}
        }
    ]
}
