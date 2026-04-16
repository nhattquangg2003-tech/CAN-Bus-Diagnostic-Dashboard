import QtQuick
import QtQuick.Controls

Item {
    property string buttonColor: "white"
    property real btnradius: 25
    property string iconSource: ""
    property string textbtn: ""
    property bool enabledbtn: true
    Rectangle {
        id: buttonBackground
        anchors.fill: parent
        color: enabledbtn ? buttonColor : "gray"
        radius: btnradius

        Image {
            id: buttonIcon
            source: iconSource
            anchors.centerIn: parent
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.3
            height: buttonIcon.width
            fillMode: Image.PreserveAspectFit
            antialiasing: true
            opacity: enabledbtn ? 1 : 0.5
        }
        Text {
            id: buttonText
            anchors.left: buttonIcon.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: textbtn
        }
    }

}
