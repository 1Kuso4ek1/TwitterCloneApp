import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0

    ToolBar {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 40

        RowLayout {
            anchors.fill: parent

            Label {
                text: "Feed"
                font.pixelSize: 18
                font.bold: true

                Layout.fillWidth: true

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }
        }
    }
}
