#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "shubhajaani";
const char* password = "druvilalu27";

// DHT setup
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED
#define LED_PIN D2
bool ledState = false;

// Async Web Server on port 80
AsyncWebServer server(80);

// HTML Dashboard
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Smart Room Reading</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Montserrat:wght@600&display=swap');

    body {
      font-family: 'Montserrat', sans-serif;
      background: linear-gradient(135deg, #6b73ff 0%, #000dff 100%);
      color: #fff;
      margin: 0;
      padding: 30px 10px;
      display: flex;
      flex-direction: column;
      align-items: center;
      min-height: 100vh;
    }

    h1 {
      font-size: 3rem;
      margin-bottom: 40px;
      letter-spacing: 2px;
      text-shadow: 2px 2px 8px rgba(0, 0, 0, 0.5);
    }

    .container {
      display: flex;
      gap: 30px;
      flex-wrap: wrap;
      justify-content: center;
      width: 90%;
      max-width: 900px;
    }

    .card {
      background: linear-gradient(145deg, #1f1f9f, #3434e0);
      border-radius: 15px;
      box-shadow: 0 6px 15px rgba(0, 0, 0, 0.3);
      width: 280px;
      height: 180px;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      transition: background 0.4s ease;
    }

    .card h2 {
      margin: 0;
      font-size: 2.5rem;
      letter-spacing: 1.5px;
    }

    .card p {
      margin-top: 10px;
      font-size: 1.1rem;
      font-weight: 600;
      color: #cfcfff;
      text-shadow: 1px 1px 4px rgba(0, 0, 0, 0.4);
    }

    /* Special styling for message card */
    .message {
      font-size: 1.3rem;
      font-weight: 700;
      color: #ff4444;
      text-shadow: 1px 1px 4px rgba(0, 0, 0, 0.6);
    }
    .normal {
      color: #44ff88;
    }
  </style>
</head>
<body>

  <h1>Smart Room Reading</h1>

  <div class="container">
    <div class="card" id="tempCard">
      <h2 id="temperature">-- °C</h2>
      <p>Temperature</p>
    </div>

    <div class="card" id="humCard">
      <h2 id="humidity">-- %</h2>
      <p>Humidity</p>
    </div>

    <div class="card" id="msgCard">
      <p id="message" class="message normal">Waiting for data...</p>
    </div>
  </div>

  <script>
    async function fetchSensorData() {
      try {
        const response = await fetch('/sensor');
        const data = await response.json();

        if(data.temperature !== null && data.humidity !== null) {
          // Update temperature
          document.getElementById('temperature').textContent = data.temperature.toFixed(1) + ' °C';

          // Update humidity
          document.getElementById('humidity').textContent = data.humidity.toFixed(1) + ' %';

          // Update message and LED status based on temperature
          const msgEl = document.getElementById('message');
          if (data.temperature > 30) {
            msgEl.textContent = "Turn ON AC!";
            msgEl.classList.remove('normal');
            msgEl.style.color = '#ff4444';
            // Tell ESP to turn LED off by calling endpoint
            fetch('/ledoff');
          } else {
            msgEl.textContent = "Temperature Normal";
            msgEl.classList.add('normal');
            msgEl.style.color = '#44ff88';
            // Tell ESP to turn LED on by calling endpoint
            fetch('/ledon');
          }
        } else {
          document.getElementById('message').textContent = "Sensor Error!";
        }
      } catch (error) {
        document.getElementById('message').textContent = "Connection Error!";
      }
    }

    setInterval(fetchSensorData, 2000);
    fetchSensorData();
  </script>

</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Serve dashboard page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Serve JSON sensor data
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      request->send(200, "application/json", "{\"temperature\":null,\"humidity\":null}");
    } else {
      String json = "{\"temperature\":" + String(temp, 1) + ",\"humidity\":" + String(hum, 1) + "}";
      request->send(200, "application/json", json);
    }
  });

  // LED ON endpoint
  server.on("/ledon", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_PIN, HIGH);
    ledState = true;
    request->send(200, "text/plain", "LED ON");
  });

  // LED OFF endpoint
  server.on("/ledoff", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_PIN, LOW);
    ledState = false;
    request->send(200, "text/plain", "LED OFF");
  });

  server.begin();
}

void loop() {
  // Nothing needed here, everything handled asynchronously
}
