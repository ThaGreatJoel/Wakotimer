# 📟 HackaTime Live Tracker (ESP8266 + LCD)

This project connects an **ESP8266 (NodeMCU)** to [HackaTime](https://hackatime.hackclub.com) to display **real-time coding stats** on a **16x2 LCD screen** and alerts using a buzzer.

It shows your:
- 🔴 **Live project** name + seconds coded
- 🔁 **Most used project**
- 🕒 **Total coding time**

A button lets you switch between these 3 views.

---

## 🧰 Hardware

- ESP8266 NodeMCU
- 16x2 LCD (parallel, not I2C)
- Potentiometer (for contrast)
- Active buzzer
- Push button
- Breadboard and jumper wires
- Wi-Fi (with internet)

---

## ⚙️ Wiring

### LCD Pin Connections (using `LiquidCrystal lcd(D1, D2, D3, D4, D5, D6)`)

| LCD Pin | Function | Connect to |
|---------|----------|------------|
| 1 (VSS) | GND      | GND        |
| 2 (VDD) | VCC      | 5V         |
| 3 (V0)  | Contrast | Pot center |
| 4 (RS)  | Control  | D1         |
| 5 (RW)  | Write    | GND        |
| 6 (E)   | Enable   | D2         |
| 11–14   | D4–D7    | D3–D6      |
| 15 (A)  | Backlight| 5V (with 220Ω resistor) |
| 16 (K)  | Backlight| GND        |

### Other Pins

| Component | ESP8266 Pin |
|-----------|-------------|
| Buzzer    | D7          |
| Button    | D0          |

---

## 🔑 Configuration

In the `.ino` file:

```cpp
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

const char* apiKey = "your-hackatime-api-key";
const char* host = "hackatime.hackclub.com";
