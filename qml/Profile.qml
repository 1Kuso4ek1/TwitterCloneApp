import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

ColumnLayout {
    id: root
    spacing: 0

    required property int userId
    required property int currentUserId

    property var userProfile: ({})
    property bool isLoading: false

    // We're receiving only posts from the server,
    // so the 'user' field is empty, but since 'Post'
    // relies on the 'full' model (with user inside),
    // we must add the 'user' object received earlier ourselves
    function addUserToFeed(model) {
        for(let i = 0; i < model.length; i++)
            model[i].user = root.userProfile
    }

    Connections {
        target: Api

        function onUserReceived(user) {
            if(user.id === root.userId) {
                root.userProfile = user
                Api.getUserPosts(root.userId)
            }
        }

        function onUserPostsReceived(posts) {
            addUserToFeed(posts)
            feed.model = posts
            root.isLoading = false
        }

        function onPostDeleted(postId) {
            const index = feed.model.findIndex(item => item.id === postId)
            if(index !== -1)
                feed.model.splice(index, 1)
        }

        function onErrorOccurred(error) {
            console.error("Profile Error:", error)
        }
    }

    Component.onCompleted: {
        root.isLoading = true
        Api.getUser(root.userId)
    }

    ToolBar {
        id: toolBar

        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 54

        Material.primary: Material.color(Material.Grey, Material.Shade900)
        Material.elevation: 6

        z: 2

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            RoundButton {
                id: backButton

                text: "←"

                font.pixelSize: 20
                font.bold: true

                onClicked: Navigation.pop()
            }

            Label {
                text: root.userProfile.display_name || "User Profile"

                font.pixelSize: 18
                font.bold: true
                font.contextFontMerging: true

                Layout.fillWidth: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Pane {
            Layout.fillWidth: true
            Layout.margins: 20

            Material.background: Material.color(Material.Grey, Material.Shade900)
            Material.roundedScale: Material.MediumScale
            Material.elevation: 6

            z: 1

            RowLayout {
                anchors.fill: parent

                ColumnLayout {
                    id: layout

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout {
                        Layout.fillWidth: true

                        spacing: 10

                        Avatar {
                            avatarUrl: userProfile.avatar_url
                            size: 80
                        }

                        ColumnLayout {
                            Layout.fillWidth: true

                            spacing: 2

                            Label {
                                text: userProfile.display_name || "Unknown User"

                                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                                font.bold: true
                                font.pixelSize: 20
                                font.contextFontMerging: true

                                color: "white"
                            }

                            Label {
                                text: "@" + userProfile.username || "unknown_user"

                                font.pixelSize: 16

                                color: "white"
                            }
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                RoundButton {
                    text: "ᐧᐧᐧ"

                    font.pixelSize: 16

                    Layout.alignment: Qt.AlignTop

                    background: null

                    onClicked: {
                        if(root.userProfile.id !== root.currentUserId) {
                            contextMenu.visible = false
                            contextMenu.height = 0
                        }
                        contextMenu.popup()
                    }
                }

                Menu {
                    id: contextMenu

                    MenuItem {
                        text: "Logout"

                        onClicked: {
                            Api.auth.logout()
                        }
                    }
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20

            currentIndex: root.isLoading ? 0 : 1

            Item {
                Layout.alignment: Qt.AlignCenter

                BusyIndicator {
                    running: root.isLoading

                    Material.accent: Material.color(Material.Blue, Material.Shade500)

                    anchors.centerIn: parent
                }
            }

            ListView {
                id: feed

                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: 20

                model: ({})
                delegate: Post {
                    item: modelData
                    userId: root.currentUserId
                }
            }
        }
    }
}
