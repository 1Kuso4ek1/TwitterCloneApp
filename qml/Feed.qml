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
            console.error("Feed.qml: Error:", error)
        }
    }

    Connections {
        target: Api.users

        function onProfileReceived(profile) {
            root.currentUser = profile
            Api.posts.getFeed();
        }
    }

    // Get rid of the code duplication in Profile.qml
    Connections {
        target: Api.posts

        function onFeedReceived(feedData) {
            feedModel.clear()
            for(let i = 0; i < feedData.length; i++)
                feedModel.append({ post: feedData[i] })
            root.isLoading = false
        }

        function onPostCreated(post) {
            feedModel.insert(0, { post: post })
        }

        function onPostDeleted(postId) {
            for(let i = 0; i < feedModel.count; i++) {
                if(feedModel.get(i).post.id === postId) {
                    feedModel.remove(i)
                    break
                }
            }
        }

        function onPostLiked(postId) {
            for(let i = 0; i < feedModel.count; i++) {
                if(feedModel.get(i).post.id === postId) {
                    let post = feedModel.get(i).post
                    post.liked = true
                    post.likes_count++

                    feedModel.set(i, { post: post })
                    break
                }
            }
        }

        function onPostUnliked(postId) {
            for(let i = 0; i < feedModel.count; i++) {
                if(feedModel.get(i).post.id === postId) {
                    let post = feedModel.get(i).post
                    post.liked = false
                    post.likes_count--

                    feedModel.set(i, { post: post })
                    break
                }
            }
        }
    }

    function loadData() {
        root.isLoading = true

        Api.users.getMe()
    }

    Component.onCompleted: {
        Api.auth.updateLoginState()
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
                    onClicked: Navigation.push("Profile.qml", { userId: root.currentUser.id, currentUserId: root.currentUser.id })
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

            model: feedModel
            delegate: Component {
                Post {
                    required property var post

                    item: post
                    userId: root.currentUser.id || -1
                }
            }
        }
    }

    ListModel {
        id: feedModel
    }

    Timer {
        interval: 30000; running: true; repeat: true
        onTriggered: Api.posts.getFeed()
    }
}
