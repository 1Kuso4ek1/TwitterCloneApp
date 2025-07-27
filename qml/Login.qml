import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import AuthManager

ColumnLayout {
    id: root
    spacing: 0

    ToolBar {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 50

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

            onClicked: {
                AuthManager.login()
            }
        }
    }
}
