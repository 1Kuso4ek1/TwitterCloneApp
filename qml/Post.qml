import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

Pane {
    property int userId
    property var item

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
                avatarUrl: item.user.avatar_url
                size: 40

                MouseArea {
                    anchors.fill: parent
                    onClicked: Navigation.push("Profile.qml", { userId: item.user.id, currentUserId: userId })
                }
            }

            ColumnLayout {
                Layout.fillWidth: true

                spacing: 2

                Label {
                    text: item.user.display_name || "Unknown User"

                    font.bold: true
                    font.pixelSize: 16
                    font.contextFontMerging: true

                    color: "white"
                }

                Label {
                    text: "@" + item.user.username || "unknown_user"

                    font.pixelSize: 13

                    color: "white"
                }
            }

            Item { Layout.fillWidth: true }

            RoundButton {
                text: "X"

                width: 16
                height: 16

                font.pixelSize: 12

                Layout.alignment: Qt.AlignRight

                visible: item.user.id === userId

                onClicked: {
                    Api.deletePost(item.id);
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: item.content
                wrapMode: Text.WordWrap

                Layout.fillWidth: true

                font.pixelSize: 15
                font.contextFontMerging: true

                color: "white"
            }

            Label {
                text: item.created_at

                Layout.fillWidth: true
                Layout.topMargin: 10

                font.pixelSize: 10
                color: "#606060"
            }
        }
    }
}
