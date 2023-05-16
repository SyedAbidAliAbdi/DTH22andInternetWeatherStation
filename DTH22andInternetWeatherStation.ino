#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid       = "***** WiFi ID******";
const char* password   = "***** WiFi PASSWORD *****";

// Create a web server on port 80
WiFiServer server(80);

// Initialize the DHT sensor
#define DHTPIN 2          // DHT sensor pin
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Prepare the HTTP response
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  String html = "<!DOCTYPE html>\n"
              "<html>\n"
              "<head>\n"
              "<script>\n"
              "  // Refresh the page after every 10 minutes (600000 milliseconds)\n"
              "  setInterval(function(){\n"
              "    location.reload();\n"
              "  }, 600000);\n"
              "</script>\n"
              "<title>Weather Report</title>\n"
              "<meta charset='UTF-8'>\n"
              "<meta http-equiv='refresh' content='900'>\n"
              "<meta name='author' content='Syed Abid Ali Abdi'>\n"
              "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>\n"
              "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.5.0/css/all.css' integrity='sha384-B4dIYHKNBt8Bc12p		+WXckhzcICo0wtJAoU8YZTY5qE0Id1GSseTk6S+L3BlXeVIU' crossorigin='anonymous'>\n"
              "<style>\n"
              "  body {\n"
              "    background-color: #f2f2f2;\n"
              "    font-family: Arial, sans-serif;\n"
              "    color: #333;\n"
              "  }\n"
              "  h1 {\n"
              "    text-align: center;\n"
              "    margin-top: 50px;\n"
              "    margin-bottom: 30px;\n"
              "    font-weight: bold;\n"
              "    font-size: 40px;\n"
              "    color: #0072c6;\n"
              "    text-shadow: 1px 1px #ddd;\n"
              "  }\n"
              "  table {\n"
              "    border-collapse: collapse;\n"
              "    width: 80%;\n"
              "    margin: 0 auto;\n"
              "    margin-bottom: 50px;\n"
              "    box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.2);\n"
              "  }\n"
              "  th, td {\n"
              "    text-align: left;\n"
              "    padding: 10px;\n"
              "    border-bottom: 1px solid #ddd;\n"
              "  }\n"
              "  tr:hover {\n"
              "    background-color:#f5f5f5;\n"
              "  }\n"
              "  .icon {\n"
              "    font-size: 24px;\n"
              "    width: 50px;\n"
              "    text-align: center;\n"
              "  }\n"
              "  .fa {\n"
              "    color: #0072c6;\n"
              "  }\n"
              "  .fas {\n"
              "    color: #0072c6;\n"
              "  }\n"
              "  #weather-data tr:nth-child(even) {\n"
              "    background-color: #f2f2f2;\n"
              "  }\n"
              "  #weather-data tr:nth-child(odd) {\n"
              "    background-color: #fff;\n"
              "  }\n"
              "  #weather-data td:first-child {\n"
              "    font-weight: bold;\n"
              "    color: #0072c6;\n"
              "    width: 35%;\n"
              "  }\n"
                 "#weather-data td:last-child {\n"
                 "  font-weight: bold;\n"
                 "  text-align: right;\n"
                 "  color: #333;\n"
                 "  width: 35%;\n"
                 "}\n"
                 "</style>\n"
                 "<h1>Weather Report</h1>\n"
                 "<table>\n"
                 "<tbody id='weather-data'>\n"
                 "<tr>\n"
                 "<td>Today</td>\n"
                 "<td></td>\n"
                 "<td id='time'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Description</td>\n"
                 "<td class='icon'></td>\n"
                 "<td id='description'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Temperature (Internal)</td>\n"
                 "<td class='icon'><i class='fa fa-thermometer-2'></i></td>\n"
                 "<td >\n"
                  + String(temperature,1) + 
                 " °C</td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Humidity (Internal)</td>\n"
                 "<td class='icon'><i class='fa fa-tint'></i></td>\n"
                 "<td >\n"
                 + String(humidity,1) + 
                 " %</td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Temperature</td>\n"
                 "<td class='icon'><i class='fa fa-thermometer-3'></i></td>\n"
                 "<td id='temperature'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Humidity</td>\n"
                 "<td class='icon'><i class='fa fa-tint'></i></td>\n"
                 "<td id='humidity'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Wind Speed</td>\n"
                 "<td class='icon'><i class='fas fa-wind'></i></td>\n"
                 "<td id='wind-speed'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Wind Direction</td>\n"
                 "<td class='icon'><i class='fa fa-compass'></i></td>\n"
                 "<td id='wind-direction'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Pressure</td>\n"
                 "<td class='icon'><i class='fa fa-line-chart'></i></td>\n"
                 "<td id='pressure'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Sunrise (UTC+5)</td>\n"
                 "<td class='icon'><i class='fa fa-sun-o'></i></td>\n"
                 "<td id='sunrise'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Sunset (UTC+5)</td>\n"
                 "<td class='icon'><i class='fa fa-moon-o'></i></td>\n"
                 "<td id='sunset'></td>\n"
                 "</tr>\n"
                 "<tr>\n"
                 "<td>Location</td>\n"
                 "<td class='icon'><i class='fa fa-map-marker'></i></td>\n"
                 "<td id='location'></td>\n"
                 "</tr>\n"
              "</tbody>\n"
              "</table>\n"
              "<script>\n"
              "  const api_key = '*********Your API Key **********';\n"
              "  const city_name = 'Karachi';\n"
              "  const api_url = `https://api.openweathermap.org/data/2.5/weather?q=${city_name}&appid=${api_key}&units=metric`;\n\n"
              "  function degToCompass(num) {\n"
              "    const val = Math.floor((num / 22.5) + 0.5);\n"
              "    const arr = ['N', 'NNE', 'NE', 'ENE', 'E', 'ESE', 'SE', 'SSE', 'S', 'SSW', 'SW', 'WSW', 'W', 'WNW', 'NW', 'NNW'];\n"
              "    return arr[(val % 16)];\n"
              "  }\n\n"
              "  fetch(api_url)\n"
              "    .then(response => response.json())\n"
              "    .then(data => {\n"
              "      console.log(data);\n\n"
              "      document.getElementById('description').innerText = data.weather[0].description;\n"
              "      document.querySelector('#temperature').innerText = `${data.main.temp} °C`;\n"
              "      document.querySelector('#humidity').innerText = `${data.main.humidity}%`;\n"
              "      document.querySelector('#wind-speed').innerText = `${data.wind.speed} m/s`;\n"
              "      document.querySelector('#wind-direction').innerText = `${degToCompass(data.wind.deg)} (${data.wind.deg}°)`;\n"
              "      document.querySelector('#pressure').innerText = `${data.main.pressure} hPa`;\n"
              "      document.querySelector('#sunrise').innerText = new Date(data.sys.sunrise * 1000).toLocaleTimeString('en-US', {timeZone: 'Asia/Karachi'});\n"
              "      document.querySelector('#sunset').innerText = new Date(data.sys.sunset * 1000).toLocaleTimeString('en-US', {timeZone: 'Asia/Karachi'});\n"
              "      document.querySelector('#location').innerText = `${data.name}, ${data.sys.country}`;\n"
              "      document.querySelector('.icon').innerHTML = `<img src='https://openweathermap.org/img/w/${data.weather[0].icon}.png' alt='${data.weather[0].description}'>`;\n"
              "    })\n"
              "    .catch(error => console.log(error));\n"
              "</script>\n\n"
              "<script>\n"
              "  function showTime() {\n"
              "    var date = new Date();\n"
              "    var options = {\n"
              "      timeZone: 'Asia/Karachi',\n"
              "      weekday: 'long',\n"
              "      year: 'numeric',\n"
              "      month: 'long',\n"
              "      day: 'numeric',\n"
              "      hour: 'numeric',\n"
              "      minute: 'numeric',\n"
              "      second: 'numeric',\n"
              "      hour12: true\n"
              "    };\n"
              "    var dateString = date.toLocaleString('en-US', options);\n"
              "    document.getElementById('time').innerHTML = dateString;\n"
              "    }\n"
              "    setInterval(showTime, 1000);\n"
              "    </script>\n"
              "    </body>\n"
              "    </html>";
  
  // Send the HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
  
  // Wait until the client disconnects
  while (client.connected()) {
    if (client.available()) {
      client.read();
    }
  }
}
