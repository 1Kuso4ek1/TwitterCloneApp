import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

Popup {
    parent: Overlay.overlay

    anchors.centerIn: parent

    width: 400
    height: contentLayout.implicitHeight + 24

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    modal: true
    focus: true
    dim: true

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    ColumnLayout {
        id: contentLayout

        anchors.fill: parent
        anchors.margins: 12

        spacing: 10

        Material.accent: Material.color(Material.Blue, Material.Shade500)

        Label {
            text: "New post"

            Layout.alignment: Qt.AlignHCenter

            font.bold: true
            font.pixelSize: 18

            color: "white"
        }

        TextArea {
            id: contentArea

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 120

            color: "white"

            placeholderText: "What's new?"
        }

        RowLayout {
            Layout.fillWidth: true

            spacing: 10

            Button {
                text: "Cancel"

                Layout.fillWidth: true
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter

                onClicked: newPostPopup.close()
            }

            Button {
                text: "Post"
                highlighted: true

                Layout.fillWidth: true
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter

                onClicked: {
                    Api.createPost(contentArea.text)

                    contentArea.clear()
                    newPostPopup.close()
                }
            }
        }
    }
}
