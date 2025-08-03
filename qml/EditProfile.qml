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

                Layout.fillWidth: true
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter

                onClicked: {
                    // Api call
                    // editUser.close()
                    errorLabel.visible = true
                    errorLabel.text = "This feature is not implemented yet."
                }
            }
        }
    }
}
