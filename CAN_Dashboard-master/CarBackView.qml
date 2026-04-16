import QtQuick
import STM32_ 1.0



Item {
    id: root
    width: parent ? parent.width : 800
    height: parent ? parent.height : 480
    property real speed: stm32Logic.speed
    FontLoader {
            id: digitalFont
            source: "qrc:/resource/assets/font/digital-7.ttf"
        }
    Rectangle
    {
        anchors.fill:parent
        color: "transparent"
        Item {
            id: displayArea
            anchors.bottom: parent.bottom
            height: parent.height*0.8
            width: parent.width
            Image {
                id: carBackView
                source: "qrc:/image/assets/carback.png"
                anchors.centerIn: parent
                width: parent.width*1.1
                height: parent.height*1.1
                fillMode: Image.PreserveAspectFit
            }
        }
        Text {
            id: textSpeed
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: displayArea.top
            anchors.bottomMargin: 10
            font.pixelSize: 60
            color: "white"
            font.bold: true
            text: Math.round(root.speed).toString()
        }
        Text {
            id: textUnit
            text: "km/h"
            anchors.bottom: textSpeed.top
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 20
            color: "gray"
            font.italic: true
        }
        Text {
            id: displayTimer
            text: realTime.datetime
            font.pixelSize: 40
            font.family: digitalFont.name
            color: "white"
            antialiasing: true
            anchors.top: displayArea.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
