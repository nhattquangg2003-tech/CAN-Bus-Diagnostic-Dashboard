import QtQuick
import QtQuick.Controls
import RealTimeLib 1.0
import QtQuick.Layouts
import QtQuick.Effects
import STM32_ 1.0

Window {
    id: root
    visible: true
    width: 900
    height: 500
    ////////BACK GROUND////////
    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "qrc:/image/assets/backgroundTest.jpg"
    }
    ////////////////////////////////
    /////PROPERTY//////////////////////
    property double speed: stm32Logic.speed
    property double rpm: stm32Logic.rpm
    property bool num1: false
    property bool num2: false
    property bool num3: false
    property bool num4: false
    property bool num5: false
    property bool num6: false
    property bool num7: false
    property real progressFuel: stm32Logic.fuelLevel / 100.0
    property real progressTemp: 0.5

    property int stateIndex: 0
    property var states: ["normal","can", "chart"]
    function updateState() {
        root.mode = root.states[root.stateIndex]
    }
    property string mode: "normal"
    //////////////////////////////////////
    ////////// CHẠY THỜI GIAN THỰC //////////
    RealTime
    {
        id: realTime
        Component.onCompleted: {
            startTimer()
        }
    }
     Stm32    { id: stm32Logic }
    /////////////////////////////////////////////////////////
    //// HIỂN THỊ ĐỒNG HỒ TỐC ĐỘ
    Rectangle
    {
        id: rectSpeed
        width: root.width/3
        height: rectSpeed.width
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 20
        color: "transparent"
        DongHoTocDo
        {
            id: speedGause
            width: parent.width
            height: width
            anchors.verticalCenter: parent.verticalCenter
            speed: root.speed
            iconSource: "qrc:/image/assets/purple.png"
        }
    }
    ///////////////////////////////////////////////////////////////
    //// HIỂN THỊ VÙNG MÀN HÌNH ////
    ////MÀN HÌNH CHUYỂN ĐỔI GIỮA 3 STATE//////////////
    Rectangle
    {
        id:rectDisplay
        anchors.left: rectSpeed.right
        anchors.right: rectRpm.left
        height: rectDisplay.width
        anchors.verticalCenter: parent.verticalCenter
        color: "transparent"
        //property string mode: "normal"

        Loader
                {
                    id: normalState
                    anchors.fill: parent
                    source: "CarBackView.qml"
                    active: mode === "normal"
                    // onLoaded:
                    // {
                    //     normalState.item.speed = Qt.biding(function(){
                    //     root.s
                    // }
                }
                // Loader
                // {
                //     id: chartState
                //     anchors.fill: parent
                //     source: "Chart.qml"
                //     active: mode === "chart"
                // }
                Loader
                {
                    id: canState
                    anchors.fill: parent
                    source: "CAN.qml"
                    active: mode === "can"
                }

    }
    ///////////////////////////////////////////////////////////
    ///////// HIỂN THỊ ĐỒNG TỐC TỐC ĐỘ ĐỘNG CƠ /////////////
    Rectangle
    {
        id: rectRpm
        width: root.width/3
        height: rectRpm.width
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 20
        color: "transparent"
        DongHoRPM
        {
            id: rpmGause
            width: parent.width
            height: width
            anchors.verticalCenter: parent.verticalCenter
            rpm: root.rpm
            iconSource: "qrc:/image/assets/purple.png"
        }
    }
    //////////////////////////////////////////////////
    ///////////// HIỂN THỊ CÁC BIỂU TƯỢNG CẢNH BÁO /////////////
    Rectangle
    {
        id: rectDiag
        anchors.bottom: rectSpeed.top
        anchors.top: headerBar.bottom
        width: parent.width/3
        x: (parent.width-width)/2
        color: "transparent"
        RowLayout
        {
            anchors.fill: rectDiag
            spacing: 20
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: battery
                    source: "qrc:/image/assets/battery1.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: stm32Logic.led1
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: oil
                    source: "qrc:/image/assets/oilpressure1.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: stm32Logic.led2
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: abs
                    source: "qrc:/image/assets/abs1.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: stm32Logic.led3
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: airbag
                    source: "qrc:/image/assets/airbag1.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: stm32Logic.led4
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: brake
                    source: "qrc:/image/assets/brake-system-warning.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: stm32Logic.led5
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight:40
                Image {
                    id: engine
                    source: "qrc:/image/assets/engine.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: num6
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Image {
                    id: temperature
                    source: "qrc:/image/assets/temp.png"
                    width: 32
                    height: 32
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    visible: num7
                }
            }
        }
    }
    //////////////////////////////////////////////////////////////////
    ////////// HIỂN THỊ MỨC XĂNG////////////////////////////////
    Rectangle
    {
        id: rectFuel
        anchors.top: rectSpeed.bottom
        anchors.bottom: parent.bottom
        anchors.right: rectDiag.left
        anchors.left: parent.left
        anchors.leftMargin: 20
        color: "transparent"
        MyProgressBar
        {
            id: fuelBar
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: parent.width
            radiusprogress: height/2
           colorprogress: progressFuel < 0.2 ? "#CC0000" : "#00C853"
            progress: progressFuel
        }
        Image {
            id: fuelImage
            source: "qrc:/image/assets/local_gas_station_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.png"
            width: 32
            height: width
            anchors.centerIn: fuelBar
            fillMode: Image.PreserveAspectFit
        }
    }
    ////////////////////////////////////////////////////////
    /////////HIỂN THỊ NHIỆT ĐỘ ĐỘNG CƠ//////////////////
    Rectangle
    {
        id: rectTemp
        anchors.top: rectRpm.bottom
        anchors.bottom: parent.bottom
        anchors.left: rectDiag.right
        anchors.right: parent.right
        anchors.rightMargin: 20
        color: "transparent"

        MyProgressBar
        {
            id: tempBar
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: parent.width
            radiusprogress: height/2
           colorprogress: progressTemp > 0.85 ? "#CC0000" : "#0288D1"
            progress: root.progressTemp
        }
        Image {
            id: tempImage
            source: "qrc:/image/assets/device_thermostat_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.png"
            width: 32
            height: width
            anchors.centerIn: tempBar
            fillMode: Image.PreserveAspectFit
        }
    }
    ////////CÁC ICON BÁO STATE HIỆN TẠI Ở MÀN HÌNH/////////
    Rectangle
        {
            id: rectButton
            anchors.top: rectDisplay.bottom
            anchors.bottom: parent.bottom
            anchors.left: rectFuel.right
            anchors.right: rectTemp.left
            anchors.margins: 20
            anchors.topMargin: 32
            color: "transparent"
            RowLayout
            {
                anchors.fill: parent
                spacing: 20
                Mybutton
                {
                    id:btnChart
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    buttonColor: mode === "chart" ? "red" : "lightgray"
                    btnradius: 20
                    iconSource: "qrc:/image/assets/chart.png"
                }
                Mybutton
                {
                    id:btnNormal
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    buttonColor: mode === "normal" ? "red" : "lightgray"
                    btnradius: 20
                    iconSource: "qrc:/image/assets/nornal.png"

                }
                Mybutton
                {
                    id:btnCan
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    buttonColor: mode === "can" ? "red" : "lightgray"
                    btnradius: 20
                    iconSource: "qrc:/image/assets/error.png"

                }
            }
        }
    ////////////////////////////////////////////////////////////////////
       /////////////VÙNG ĐIỀU KHIỂN GIẢ LẬP////////////////////
       Rectangle
       {
           id: keyArea
           anchors.fill: parent
           color: "transparent"
           focus: true
           Component.onCompleted:
           {
               keyArea.forceActiveFocus()
           }

           Keys.onPressed: (event)=> {
                               //if (event.isAutoRepeat) return
                               if (event.key === Qt.Key_Up) ecuCmd.throttleOn()
                               if (event.key === Qt.Key_Down) ecuCmd.throttleOff()
                               if (event.key === Qt.Key_Left)  ecuCmd.brakeOn()
                               if (event.key === Qt.Key_Right) ecuCmd.brakeOff()
                               if (event.key === Qt.Key_1) num1 = !num1
                               if (event.key === Qt.Key_2) num2 = !num2
                               if (event.key === Qt.Key_3) num3 = !num3
                               if (event.key === Qt.Key_4) num4 = !num4
                               if (event.key === Qt.Key_5) num5 = !num5
                               if (event.key === Qt.Key_6) num6 = !num6
                               if (event.key === Qt.Key_7) num7 = !num7
                               // if (event.key === Qt.Key_W) {
                               //     progressFuel += 0.1
                               //     if (progressFuel > 1) progressFuel = 1
                               // }
                               // if (event.key === Qt.Key_S) {
                               //     progressFuel -= 0.1
                               //     if (progressFuel <0) progressFuel = 0
                               // }
                               // if (event.key === Qt.Key_D) {
                               //     progressTemp += 0.1
                               //     if (progressTemp > 1) progressTemp = 1
                               // }
                               // if (event.key === Qt.Key_A) {
                               //     progressTemp -= 0.1
                               //     if (progressTemp < 0) progressTemp = 0
                               // }
                               if (event.key === Qt.Key_D) {
                                   root.stateIndex++
                                   if (stateIndex > 2)
                                   root.stateIndex = 0
                                   root.updateState()
                               }
                               if (event.key === Qt.Key_A) {
                                   root.stateIndex--
                                   if (stateIndex < 0)
                                   root.stateIndex = 2
                                   root.updateState()
                               }
                           }
       }
    //
    //
    //
    // stm32 cua quang gui logic lay data tu stm32 ve va hien thi len header
    // --- HEADER TOOLBAR ---
    Rectangle {
        id: headerBar
        width: parent.width
        height: 45
        color: Qt.rgba(0, 0, 0, 0.7)
        border.color: "#2A2A2A"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            spacing: 15

            // 1. Tiêu đề
            Text {
                text: "DATA LOGS"
                font.bold: true
                color: "#757575"
                font.pixelSize: 13
                Layout.alignment: Qt.AlignVCenter
            }

            // 2. Hiển thị data mới nhất (1 dòng)
            Text {
                id: latestData
                text: stm32Logic.dataFromStm32
                color: "#CC0000"
                font.family: "Consolas"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                elide: Text.ElideLeft
            }

            // 3. Ô nhập cổng COM
            Rectangle {
                width: 100
                height: 28
                color: "#0E0E0E"
                radius: 4
                border.color: portField.activeFocus ? "#CC0000" : "#333"

                TextInput {
                    id: portField
                    anchors.fill: parent
                    anchors.margins: 6
                    text: "COM7"
                    color: "#CCCCCC"
                    font.pixelSize: 12
                    font.family: "Consolas"
                    verticalAlignment: TextInput.AlignVCenter
                }
            }

            // 4. Nút CONNECT
            Rectangle {
                width: 90
                height: 28
                radius: 4
                color: connectMouse.pressed ? "#990000" : "#CC0000"

                Text {
                    anchors.centerIn: parent
                    text: "CONNECT"
                    color: "#FFFFFF"
                    font.bold: true
                    font.pixelSize: 10
                }

                MouseArea {
                    id: connectMouse
                    anchors.fill: parent
                    onClicked: stm32Logic.getDataFromStm32(portField.text)
                }
            }

            // 5. Thời gian
            Text {
                text: Qt.formatDateTime(new Date(), "hh:mm:ss")
                color: "#555555"
                font.pixelSize: 11
                font.family: "Consolas"
                Layout.alignment: Qt.AlignVCenter
            }
        }
    }
}


