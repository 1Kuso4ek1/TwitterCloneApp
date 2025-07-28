import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

ColumnLayout {
    id: root
    spacing: 0

    property var currentUser: ({})

    Connections {
        target: Api

        function onLoggedInChanged(loggedIn) {
            if(!loggedIn)
                Navigation.replace("Login.qml", {});
        }

        function onProfileReceived(profile) {
            root.currentUser = profile;
        }

        function onFeedReceived(feedData) {
            feed.model = feedData;
        }

        function onPostCreated(post) {
            feed.model.unshift(post);
        }

        function onPostDeleted(postId) {
            const index = feed.model.findIndex(item => item.id === postId);
            if(index !== -1)
                feed.model.splice(index, 1);
        }

        function onErrorOccurred(error) {
            console.error("Error:", error);
        }
    }

    Component.onCompleted: {
        Api.updateLoginState();
        Api.getMe();
        Api.getFeed();
    }

    NewPost {
        id: newPostPopup
    }

    ToolBar {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 54

        Material.primary: Material.color(Material.Grey, Material.Shade900)
        Material.elevation: 6

        z: 1

        RowLayout {
            anchors.fill: parent

            anchors.rightMargin: 5

            RoundButton {
                Layout.alignment: Qt.AlignLeft

                text: "+"

                font.pixelSize: 24

                width: 24
                height: 24

                onClicked: newPostPopup.open()
            }

            Label {
                text: "Feed"

                font.pixelSize: 18
                font.bold: true

                Layout.fillWidth: true

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }

            Avatar {
                avatarUrl: currentUser.avatar_url
                size: 40

                MouseArea {
                    anchors.fill: parent
                    onClicked: Navigation.push("Profile.qml", { userId: currentUser.id, currentUserId: currentUser.id })
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
        delegate: Component {
            Post {
                item: modelData
                userId: root.currentUser.id
            }
        }
    }

    Timer {
        interval: 30000; running: true; repeat: true
        onTriggered: Api.getFeed();
    }
}
