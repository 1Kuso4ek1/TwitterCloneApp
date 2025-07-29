import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

ColumnLayout {
    id: root
    spacing: 0

    Connections {
        target: Api

        function onLoggedInChanged(loggedIn) {
            if(loggedIn)
                Navigation.replace("Feed.qml", {});

            console.log("loggedInChanged:", loggedIn);
        }

        function onErrorOccurred(error) {
            console.error("Error:", error);
        }
    }

    Component.onCompleted: {
        Api.updateLoginState()
        Api.handleLoginCode()
    }

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
        Layout.margins: 20

        spacing: 20

        Button {
            text: "Login with Google"
            Layout.fillWidth: true

            onClicked: Api.login()
        }
    }
}
