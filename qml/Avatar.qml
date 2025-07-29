import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Effects

Item {
    property var avatarUrl: ""
    property int size: 32

    width: size
    height: size

    Image {
        id: avatar

        source: avatarUrl || "https://i.postimg.cc/HkR3vr08/ptitsa.png"

        visible: false
    }

    MultiEffect {
        source: avatar

        anchors.fill: parent

        maskEnabled: true
        maskSource: mask
    }

    Item {
        id: mask

        width: size
        height: size

        layer.enabled: true
        visible: false

        Rectangle {
            anchors.fill: parent

            radius: parent.width / 2

            color: "black"
        }
    }
}
