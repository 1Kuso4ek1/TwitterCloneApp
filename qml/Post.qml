import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Pane {
    width: feed.width
    implicitHeight: layout.implicitHeight + 50

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.margins: 12

        RowLayout {
            Layout.fillWidth: true

            spacing: 10

            Avatar {
                avatarUrl: modelData.user.avatar_url
                size: 40
            }

            ColumnLayout {
                Layout.fillWidth: true

                spacing: 2

                Label {
                    text: modelData.user.display_name

                    font.bold: true
                    font.pixelSize: 16

                    color: "white"
                }

                Label {
                    text: "@" + modelData.user.username

                    font.pixelSize: 13

                    color: "white"
                }
            }

            Label {
                text: modelData.created_at
                horizontalAlignment: Text.AlignRight

                Layout.fillWidth: true

                font.pixelSize: 11
                color: "white"
            }
        }

        Label {
            text: modelData.content
            wrapMode: Text.WordWrap

            Layout.fillWidth: true

            font.pixelSize: 15
            color: "white"
        }
    }
}
