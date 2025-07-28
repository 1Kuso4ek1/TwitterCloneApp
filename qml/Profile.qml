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

    Connections {
        target: Api

        function onProfileReceived(profile) {
            if (profile.id === root.userId)
                root.userProfile = profile
        }

        function onUserPostsReceived(posts) {
            for(let i = 0; i < posts.length; i++)
                posts[i].user = root.userProfile;
            feed.model = posts;
        }

        function onPostDeleted(postId) {
            const index = feed.model.findIndex(item => item.id === postId);
            if(index !== -1)
                feed.model.splice(index, 1);
        }

        function onErrorOccurred(error) {
            console.error("Profile Error:", error)
        }
    }

    Component.onCompleted: {
        Api.getUser(root.userId)
        Api.getUserPosts(root.userId)
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
                font.family: "Noto Color Emoji [GOOG]"
                font.contextFontMerging: true

                Layout.fillWidth: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Item {
                width: backButton.width
                height: backButton.height
            }
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Pane {
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            Layout.margins: 20

            Material.background: Material.color(Material.Grey, Material.Shade900)
            Material.roundedScale: Material.MediumScale
            Material.elevation: 6

            z: 1

            ColumnLayout {
                id: layout

                anchors.fill: parent
                anchors.margins: 12

                RowLayout {
                    Layout.fillWidth: true

                    spacing: 10

                    Avatar {
                        avatarUrl: userProfile.avatar_url
                        size: 100
                    }

                    ColumnLayout {
                        Layout.fillWidth: true

                        spacing: 2

                        Label {
                            text: userProfile.display_name || "Unknown User"

                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            font.bold: true
                            font.pixelSize: 26
                            font.family: "Noto Color Emoji [GOOG]"
                            font.contextFontMerging: true

                            color: "white"
                        }

                        Label {
                            text: "@" + userProfile.username || "unknown_user"

                            font.pixelSize: 20

                            color: "white"
                        }
                    }
                }
            }
        }

        ListView {
            id: feed

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20

            spacing: 20

            model: ({})
            delegate: Post {
                item: modelData
                userId: currentUserId
            }
        }
    }
}
