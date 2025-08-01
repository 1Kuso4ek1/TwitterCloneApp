import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 640
    height: 480

    minimumWidth: 400
    minimumHeight: 400

    visible: true
    title: qsTr("Twitter Clone")

    Material.theme: Material.System

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Feed{}

        Component.onCompleted: Navigation.stackView = stackView
    }

    onClosing: (close) => {
        close.accepted = stackView.depth === 1

        if(!close.accepted)
            stackView.pop()
    }
}
