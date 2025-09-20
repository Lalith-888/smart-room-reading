# 🌡️ Smart Room Monitoring System (ESP8266 + DHT11)

This project is a simple web-based room environment monitor using an ESP8266 (e.g., NodeMCU), DHT11 temperature and humidity sensor, and a status LED. It serves real-time data over Wi-Fi and displays it on a beautiful HTML dashboard.

## 🚀 Features

* Wi-Fi-enabled room monitoring
* Real-time Temperature and Humidity readings
* Auto-refreshing web dashboard (every 2 seconds)
* LED indicator based on temperature threshold
* AC suggestion when temp > 30°C

---

## 🔧 Hardware Requirements

| Component     | Description               |
| ------------- | ------------------------- |
| ESP8266 Board | NodeMCU or similar        |
| DHT11 Sensor  | Temp & humidity sensor    |
| LED           | Status indicator          |
| 220Ω Resistor | For LED protection        |
| Jumper Wires  | For connections           |
| Breadboard    | Optional, for prototyping |

---

## ⚙️ Wiring Diagram

| Component  | ESP8266 Pin           |
| ---------- | --------------------- |
| DHT11 VCC  | 3.3V                  |
| DHT11 GND  | GND                   |
| DHT11 Data | D4 (GPIO2)            |
| LED (+)    | D2 (GPIO4)            |
| LED (–)    | GND via 220Ω resistor |

---

## 📡 How It Works

* Connect to Wi-Fi using provided SSID and password.
* DHT11 sensor reads temperature and humidity every 2 seconds.
* If temperature exceeds 30°C:

  * Web UI displays a "Turn ON AC!" warning.
  * LED is turned **OFF**.
* If temperature is normal (≤ 30°C):

  * Message shows "Temperature Normal".
  * LED is turned **ON**.
* Data is displayed on a web dashboard served from the ESP8266.

---

## 🌐 Access the Dashboard

1. Upload the code to your ESP8266 using Arduino IDE.
2. Open Serial Monitor to find the assigned IP address.
3. Open that IP in a web browser to view live data.

---

## 📸 Preview
![WhatsApp Image 2025-09-20 at 16 07 19_660a90ed](https://github.com/user-attachments/assets/a97c823b-2c51-41ea-abed-5ccd79e99d4a)
![WhatsApp Image 2025-09-20 at 15 10 55_21d3af7f](https://github.com/user-attachments/assets/3ed1a470-a0d2-47ab-9ba2-27f51c4ff331)

---

## 🛠️ Libraries Used

* `ESP8266WiFi.h`
* `ESPAsyncWebServer.h`
* `Adafruit_Sensor.h`
* `DHT.h`

Make sure you install these via the Arduino Library Manager or PlatformIO.

---

## 📌 Notes

* Make sure you power the DHT11 with **3.3V**, not 5V.
* Wi-Fi credentials are hardcoded — change them as needed.
* Works best with a stable 2.4GHz Wi-Fi network.

