import QtQuick
import QtQuick.Controls

Item {
    id: root
    property real rpm: 0

    property int maxRpm: 8000
    property int vachchialon: 1000
    property int vachchianho: 250

    property real startAngle: -120
    property real sweepAngle: 240

    property real size: Math.min(width, height)
    property real m_radius: size * 0.5
    property real cx: width / 2
    property real cy: height / 2

    property string iconSource: ""
    property string speedColor: "black"



    Rectangle {
        id: background
        anchors.fill: parent
        radius: width/2
        color: speedColor
        border.color: "transparent"
    }
    Item {
        id: centerPoint
        width: 1
        height: 1
        anchors.centerIn: parent
    }
    //Vach chia
    Repeater {
        model: (root.maxRpm/ root.vachchianho) +1

        Rectangle {
            property int rpm : index * root.vachchianho
            property bool major: rpm % root.vachchialon === 0

            width: major ? parent.width * 0.01 : parent.width * 0.005
            height: major ? parent.height * 0.1 : parent.height * 0.05
            color: rpm>= 6500 ? "red" : "white"
            x: cx - width/2
            y: cy - m_radius
            antialiasing: true

            transform: Rotation {
                origin.x: width / 2
                origin.y: m_radius
                angle: root.startAngle + (rpm / root.maxRpm) * root.sweepAngle
            }
        }
    }
    //So chia
    Repeater
    {
        model: (root.maxRpm / root.vachchialon) +1
        Text {
            property int rpm : index * root.vachchialon
            font.pixelSize: root.size * 0.06
            font.bold: true
            color: rpm>= 6500 ? "red" : "white"
            text: index===0 ? "0" : (rpm/1000).toString()
            property real deg_angle: (root.startAngle + (rpm / root.maxRpm) * root.sweepAngle)
            property real rad_angle: (deg_angle-90) * Math.PI / 180
            x: cx + Math.cos(rad_angle) * (m_radius - size * 0.15) - width / 2
            y: cy + Math.sin(rad_angle) * (m_radius - size * 0.15) - height / 2
        }
    }
    //cay kim
    Rectangle {
        id: needle
        width: 4
        height: parent.height*0.4
        color: "red"
        x: centerPoint.x - needle.width / 2
        y: centerPoint.y - needle.height
        z: 1
        property real needleAngle: -120 + (root.rpm / root.maxRpm) * 240
        transform: Rotation {
            origin.x: needle.width / 2
            origin.y: needle.height
            angle:  needle.needleAngle
        }
        Behavior on needleAngle  {
            NumberAnimation {
                duration: 300
                easing.type: Easing.Linear
            }
        }
    }
    Image {
        id: rpmIcon
        source: iconSource
        anchors.centerIn: parent
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 1.15
        height: parent.height *1.15
        fillMode: Image.PreserveAspectFit
        antialiasing: true
        opacity: 0.5
    }
    Text {
        anchors.centerIn: parent
        y: 60
        text: Math.round(root.rpm) + "r/min"
        color: "gray"
        font.pixelSize: 22
    }
}
