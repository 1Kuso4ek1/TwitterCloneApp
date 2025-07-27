import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Twitter Clone")

    Material.theme: Material.Dark

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Login{}

        Component.onCompleted: Navigation.stackView = stackView
    }

    Item {
        id: backHandler
        visible: false

        Keys.onBackPressed: {
            if(stackView.depth > 1)
                stackView.pop();
            else
                Qt.quit();
        }

        Component.onCompleted: forceActiveFocus()
    }
}
