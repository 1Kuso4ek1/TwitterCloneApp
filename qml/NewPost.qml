import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

Popup {
    parent: Overlay.overlay

    anchors.centerIn: parent

    width: 350
    height: contentLayout.implicitHeight + 200

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    modal: true
    focus: true
    dim: true

    onOpened: contentArea.forceActiveFocus()
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

            wrapMode: Text.Wrap

            font.family: "Noto Color Emoji [GOOG]"
            font.contextFontMerging: true

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 120

            color: "white"

            placeholderText: "What's new?"
        }

        Label {
            text: contentArea.text.length + " / 280"

            Layout.alignment: Qt.AlignRight

            font.pixelSize: 12
            color: contentArea.text.length >= 280 ? "red" : "white"
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
                    if(contentArea.text.length === 0 || contentArea.text.length > 280) {
                        contentArea.forceActiveFocus()
                        return
                    }

                    Api.createPost(contentArea.text)

                    contentArea.clear()
                    newPostPopup.close()
                }
            }
        }
    }
}
