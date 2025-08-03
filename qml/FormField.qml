import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    property alias headerText: header.text
    property alias content: input.text
    property alias input: input

    property var maxLength: 30

    Label {
        id: header

        Layout.fillWidth: true

        font.pixelSize: 14
        color: "white"
    }

    TextField {
        id: input

        Layout.fillWidth: true

        font.contextFontMerging: true
    }

    Label {
        text: input.text.trim().length + " / " + maxLength

        Layout.alignment: Qt.AlignRight

        font.pixelSize: 12
        color: input.text.trim().length >= maxLength ? "red" : "white"
    }
}
