import QtQuick
Rectangle {
    id: rectCanRawTop
    anchors.fill: parent
    anchors.margins: 5
    color: "black"
    Text {
        anchors.centerIn: parent
        width: parent.width * 0.95
        text: cluster.rawFrame
        color: "lightgreen"
        font.pixelSize: 14
        font.family: "monospace"
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }
}
