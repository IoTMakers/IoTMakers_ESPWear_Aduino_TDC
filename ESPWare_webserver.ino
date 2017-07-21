/*
The MIT License (MIT)
Copyright (c) 2015 Pedro Minatel
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <ESP8266WiFi.h>

const char* ssid = "YOURSSID";
const char* password = "YOURPASSWD";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // prepare GPIO4
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  
  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // prepare GPI14
  pinMode(14, OUTPUT);
  digitalWrite(14, 0);
  
  // prepare GPI16
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  buf += "<html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<title>ESPWear ESP8266 Web Server</title>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "</head>";
  buf += "<h3>ESPWeare</h3>";
    buf += "<p>GPIO2 <a href=\"?function=led2_on\"><button>ON</button></a>&nbsp;<a href=\"?function=led2_off\"><button>OFF</button></a></p>";
  buf += "<p>GPIO4 <a href=\"?function=led4_on\"><button>ON</button></a>&nbsp;<a href=\"?function=led4_off\"><button>OFF</button></a></p>";
  buf += "<p>GPI14 <a href=\"?function=led14_on\"><button>ON</button></a>&nbsp;<a href=\"?function=led14_off\"><button>OFF</button></a></p>";
  buf += "<p>GPI16 <a href=\"?function=led16_on\"><button>ON</button></a>&nbsp;<a href=\"?function=led16_off\"><button>OFF</button></a></p>";
  buf += "<h4>Criado por Pedro Minatel</h4>";
  buf += "<h4>IoTMakers - 2017 </h4>";
  buf += "</html>\n";

  client.print(buf);
  client.flush();

  if (req.indexOf("led2_on") != -1)
    digitalWrite(2, 1);
  else if (req.indexOf("led2_off") != -1)
    digitalWrite(2, 0);
  else if (req.indexOf("led4_on") != -1)
    digitalWrite(4, 1);
  else if (req.indexOf("led4_off") != -1)
    digitalWrite(4, 0);
  else if (req.indexOf("led14_on") != -1)
    digitalWrite(14, 1);
  else if (req.indexOf("led14_off") != -1)
    digitalWrite(14, 0);
  else if (req.indexOf("led16_on") != -1)
    digitalWrite(16, 1);
  else if (req.indexOf("led16_off") != -1)
    digitalWrite(16, 0);
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("Client disonnected");
}
