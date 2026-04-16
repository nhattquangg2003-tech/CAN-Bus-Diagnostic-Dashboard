import QtQuick
import QtCharts

// ChartView {
//     id: chart
//     anchors.fill: parent
//     antialiasing: true
//     legend.visible: true

//     ValueAxis {
//         id: axisX
//         min: 0
//         max: 100   // 100 mẫu
//     }

//     ValueAxis {
//         id: axisYSpeed
//         min: 0
//         max: 200
//         titleText: "Speed"
//     }

//     ValueAxis {
//         id: axisYRpm
//         min: 0
//         max: 8000
//         titleText: "RPM"
//     }

//     LineSeries {
//         id: speedSeries
//         name: "Speed"
//         axisX: axisX
//         axisY: axisYSpeed
//     }

//     LineSeries {
//         id: rpmSeries
//         name: "RPM"
//         axisX: axisX
//         axisY: axisYRpm
//     }
//     property int maxSamples: 100
//     property int sampleIndex: 0
//     property var speedData: []
//     property var rpmData: []
//     Timer {
//         interval: 100   // 10Hz
//         running: true
//         repeat: true
//         onTriggered: {
//             let spd = cluster.speed
//             let rpm = cluster.rpm

//             speedData.push(spd)
//             rpmData.push(rpm)

//             if (speedData.length > maxSamples) {
//                 speedData.shift()
//                 rpmData.shift()
//             }

//             redraw()
//         }
//     }
//     function redraw() {
//         speedSeries.clear()
//         rpmSeries.clear()

//         for (let i = 0; i < speedData.length; i++) {
//             speedSeries.append(i, speedData[i])
//             rpmSeries.append(i, rpmData[i])
//         }
//     }
// }

