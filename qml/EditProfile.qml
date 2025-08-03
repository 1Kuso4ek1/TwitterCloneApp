import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

Popup {
    id: editUser

    property var user: ({})
    property var maxDisplayNameLength: 30
    property var maxUsernameLength: 20

    parent: Overlay.overlay

    anchors.centerIn: parent

    width: 350

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    modal: true
    focus: true
    dim: true

    onOpened: displayName.forceActiveFocus()
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    Connections {
        target: Api.users

        function onUserReceived(user) {
            busyIndicator.visible = false
            errorLabel.visible = false
            editUser.close()
        }
    }

    Connections {
        target: Api.requestHandler

        function onErrorOccurred(error) {
            busyIndicator.visible = false
            errorLabel.visible = true
            errorLabel.text = "Error: " + error
        }
    }

    ColumnLayout {
        id: contentLayout

        anchors.fill: parent
        anchors.margins: 12

        Material.accent: Material.color(Material.Blue, Material.Shade500)

        Label {
            text: "Edit profile"

            Layout.alignment: Qt.AlignHCenter

            font.bold: true
            font.pixelSize: 18

            color: "white"
        }

        FormField {
            id: displayName

            headerText: "Display name"
            content: user.display_name || ""
            maxLength: maxDisplayNameLength

            Layout.fillWidth: true
        }

        FormField {
            id: username

            headerText: "Username"
            content: user.username || ""
            maxLength: maxUsernameLength

            Layout.fillWidth: true
        }

        Label {
            id: errorLabel
            visible: false

            Layout.fillWidth: true

            horizontalAlignment: Qt.AlignHCenter

            font.pixelSize: 12
            color: "red"
        }

        BusyIndicator {
            id: busyIndicator
            visible: false

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            Material.accent: Material.color(Material.Blue, Material.Shade500)
        }

        RowLayout {
            Layout.fillWidth: true

            Layout.bottomMargin: 12

            spacing: 10

            Button {
                text: "Cancel"

                Layout.fillWidth: true
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter

                onClicked: editUser.close()
            }

            Button {
                text: "Save"
                highlighted: true
                enabled: displayName.content.trim().length !== 0
                    && displayName.content.trim().length <= maxDisplayNameLength
                    && username.content.trim().length >= 5
                    && username.content.trim().length <= maxUsernameLength
                    && username.content.indexOf(' ') === -1

                Layout.fillWidth: true
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter

                onClicked: {
                    // Api call
                    // editUser.close()
                    Api.users.updateMe(displayName.content.trim(), username.content.trim())
                    busyIndicator.visible = true
                }
            }
        }
    }
}
