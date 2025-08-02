import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

ColumnLayout {
    id: root
    spacing: 0

    Connections {
        target: Api.auth

        function onLoggedInChanged(loggedIn) {
            if(loggedIn)
                Navigation.replace("Feed.qml", {});
        }

        function onErrorOccurred(error) {
            console.error("Login.qml: Error:", error);
        }
    }

    Component.onCompleted: Api.auth.handleLoginCode()

    ToolBar {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 50

        Material.primary: Material.color(Material.Grey, Material.Shade900)
        Material.elevation: 6

        RowLayout {
            anchors.fill: parent

            Label {
                text: "Login"

                font.pixelSize: 18
                font.bold: true

                Layout.fillWidth: true

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.alignment: Qt.AlignCenter

        spacing: 50

        Label {
            text: "TwitterClone"

            font.pixelSize: 32
            font.bold: true

            color: "white"

            horizontalAlignment: Qt.AlignHCenter
        }

        Button {
            text: "Login with Google"

            Layout.alignment: Qt.AlignCenter

            onClicked: Api.auth.login()
        }
    }
}
