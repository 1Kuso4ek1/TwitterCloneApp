import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

Pane {
    property int userId
    property var item

    width: feed.width

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    ColumnLayout {
        id: layout

        anchors.fill: parent

        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

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

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: item.user.display_name || "Unknown User"

                        font.bold: true
                        font.pixelSize: 16
                        font.contextFontMerging: true

                        color: "white"
                    }

                    Label {
                        text: {
                            // Since QML's Date does not automatically convert to local time
                            const utc = new Date(item.created_at);
                            return (new Date(utc.getTime() - utc.getTimezoneOffset() * 60000))
                                .toLocaleString(Qt.locale(), "ddd yyyy-MM-dd hh:mm:ss");
                        }

                        Layout.fillWidth: true

                        font.pixelSize: 10
                        color: "#606060"

                        elide: Text.ElideRight
                    }
                }

                Label {
                    text: "@" + item.user.username || "unknown_user"

                    font.pixelSize: 13

                    color: "white"
                }
            }

            Item { Layout.fillWidth: true }

            RoundButton {
                text: "ᐧᐧᐧ"

                font.pixelSize: 16

                Layout.alignment: Qt.AlignTop

                background: null

                onClicked: {
                    contextMenu.popup()
                }
            }

            Menu {
                id: contextMenu

                MenuItem {
                    text: "Delete post"

                    onClicked: Api.deletePost(item.id)
                    Component.onCompleted: {
                        if(item.user.id !== userId) {
                            visible = false
                            height = 0
                        }
                    }
                }

                MenuItem {
                    text: "Follow " + item.user.display_name

                    onClicked: {}
                    Component.onCompleted: {
                        if(item.user.id === userId) {
                            visible = false
                            height = 0
                        }
                    }
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

            Button {
                text: (item.liked ? "♥ " : "♡ ") + item.likes_count

                Layout.preferredHeight: 40

                font.pixelSize: 15
            }
        }
    }
}
