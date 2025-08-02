import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

ColumnLayout {
    id: root
    spacing: 0

    property var currentUser: ({})
    property bool isLoading: false

    Connections {
        target: Api.auth

        function onLoggedInChanged(loggedIn) {
            if(!loggedIn)
                Navigation.replace("Login.qml", {})
            else
                loadData()
        }

        function onErrorOccurred(error) {
            console.error("Error:", error)
        }
    }

    Connections {
        target: Api

        function onProfileReceived(profile) {
            root.currentUser = profile
            Api.getFeed();
        }

        function onFeedReceived(feedData) {
            feed.model = feedData
            root.isLoading = false
        }

        function onPostCreated(post) {
            feed.model.unshift(post)
        }

        function onPostDeleted(postId) {
            const index = feed.model.findIndex(item => item.id === postId)
            if(index !== -1)
                feed.model.splice(index, 1)
        }

        function onErrorOccurred(error) {
            console.error("Error:", error)
        }
    }

    function loadData() {
        root.isLoading = true

        Api.getMe();
    }

    Component.onCompleted: Api.auth.updateLoginState()

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
                text: "+"

                font.pixelSize: 20

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
                avatarUrl: root.currentUser.avatar_url
                size: 40

                MouseArea {
                    anchors.fill: parent
                    onClicked: Navigation.push("Profile.qml", { userId: currentUser.id, currentUserId: currentUser.id })
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
            delegate: Component {
                Post {
                    item: modelData
                    userId: root.currentUser.id || -1
                }
            }
        }
    }

    Timer {
        interval: 30000; running: true; repeat: true
        onTriggered: Api.getFeed();
    }
}
