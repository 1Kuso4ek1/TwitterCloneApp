import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Popup {
    id: imageViewer
    parent: Overlay.overlay

    required property var imageUrl

    width: parent.width - 60
    height: parent.height - 120

    anchors.centerIn: parent

    Material.background: Material.color(Material.Grey, Material.Shade900)
    Material.roundedScale: Material.MediumScale
    Material.elevation: 6

    modal: true
    focus: true
    dim: true

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    Image {
        source: imageUrl || "https://i.postimg.cc/HkR3vr08/ptitsa.png"

        anchors.fill: parent
        anchors.margins: 12

        fillMode: Image.PreserveAspectFit
    }
}
