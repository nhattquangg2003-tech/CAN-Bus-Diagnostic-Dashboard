# Vehicle Health & Diagnostics (CAN Dashboard) 🚗

A high-performance automotive digital cluster simulation built with **Qt 6.10**, **C++**, and **QML**. This project features a custom hardware-to-software integration using a physical **CAN bus** network and an STM32-based Gateway.

## 🚀 Key Features
* **Real-time Monitoring:** Visualizes Speed, RPM, Fuel, and Engine Temperature with zero latency.
* **Hardware Integration:** Synchronized digital indicators with physical STM32 sensor nodes via a CAN-to-UART bridge.
* **Safety Logic:** Automated visual alerts triggered when Speed > 120 km/h or Temp > 105°C.
* **Fault Diagnostics:** Real-time identification and display of Diagnostic Trouble Codes (DTC).
* **Smooth UI:** High-fidelity gauge animations using QML's `NumberAnimation` and `Behavior on rotation`.

## 🛠 Technical Stack
* **Framework:** Qt 6.10.1 (QtQuick, QtSerialPort, QtCore).
* **Languages:** C++ (Backend Logic), QML & JavaScript (Frontend UI), C (Firmware).
* **Hardware:** STM32F103C8T6 (Blue Pill) & TJA1050 CAN Transceiver.
* **Protocols:** CAN 2.0A/B (500 kbit/s) & UART (115200 bps).
* **Build System:** CMake 3.16+.

## 📂 Project Structure
* **`/CAN_Dashboard`**: Qt/C++ Source code for the HMI Application.
* **`/Firmware_STM32`**: STM32 HAL-based C code for the CAN-to-UART Gateway.
* **`/assets`**: UI resources including fonts (Digital-7) and vehicle icons.

## 🏗 System Architecture
I implemented a modular **MVC + Service Layer** architecture:
* **View (QML):** Modern HMI dashboard inspired by the Honda Civic Type R cluster.
* **Controller (C++):** Bridges UI and hardware via Qt's Signal/Slot mechanism.
* **Service Layer:** Handles `QSerialPort` communication, buffer accumulation, and `KEY:VALUE` string parsing (SPD, RPM, TEMP, FUEL, DTC).

## 📅 Future Roadmap
* [ ] Implement **UDS (ISO 14229)** for professional-grade automotive diagnostics.
* [ ] Integrate **.DBC file parsing** for cross-vehicle compatibility.
* [ ] Remote diagnostics via Wi-Fi/4G and Cloud data synchronization.

---
