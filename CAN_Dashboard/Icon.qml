import QtQuick

Item {
    Rectangle
    {
        width: 100
        height: 100
        color: "lightgray"
        border.color: "black"
        border.width: 2
        radius: 10

        Text {
            anchors.centerIn: parent
            text: "Icon"
            font.pixelSize: 20
            color: "black"
        }
    }
}
