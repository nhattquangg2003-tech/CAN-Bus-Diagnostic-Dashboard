import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 400  // Thêm kích thước mẫu
    height: 20

    // progress chạy từ 0.0 đến 1.0 (ví dụ: 0.5 là 50%)
    property real progress: 0.5
    property real radiusprogress: 4
    property string colorprogress: "#00FF00" // Màu xanh lá sáng

    Row {
        anchors.fill: parent
        spacing: 3

        Repeater {
            model: 20
            Rectangle {
                // Tính toán chiều rộng chuẩn trừ đi khoảng cách spacing
                width: (root.width - (19 * 3)) / 20
                height: root.height
                radius: root.radiusprogress

                // Logic: Nếu phần trăm của vạch này nhỏ hơn hoặc bằng progress hiện tại thì sáng
                // index từ 0-19, nên (index + 1)/20 sẽ đại diện cho các mốc 5%, 10%... 100%
                color: ((index + 1) / 20) <= root.progress ? root.colorprogress : "#222222"

                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
        }
    }
}
