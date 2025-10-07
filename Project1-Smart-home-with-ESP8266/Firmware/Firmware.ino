#include "DHTesp.h"
#include <ESP8266WiFi.h>
//    Ho3ein       ktyt5708      Yasin   00000000
const char* ssid = "Ho3ein"; // نام وای فای مورد نظر
const char* password = "ktyt5708"; // پسوورد وای فای

WiFiServer server(80);

String header;

String inputString;
char incomingByte;

String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";
DHTesp dht;

String phoneNumber = "+989137947315"; // شماره پیش فرض که قابل تغییر است

void setup() {

  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D7, OUTPUT);

  WiFi.begin(ssid, password);
  dht.setup(12, DHTesp::DHT11);

  delay(1000);
  server.begin();
  Serial.begin(9600);

  delay(2000);
  digitalWrite(D2, HIGH);
  delay(1000);
  digitalWrite(D2, LOW);

  Serial.println("AT+CSMP=17,167,2,25\r");
  delay(100);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CNMI=1,2,0,0,0");
  delay(100);
  Serial.println("AT+CMGL=\"REC UNREAD\"");
  delay(100);

  // ارسال پیام آماده بودن با شماره پیش فرض
  Serial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(100);
  Serial.println("Hello HO3EIN Im Ready");
  delay(100);
  Serial.write(26);
  delay(100);

}

void loop() {

  float d = dht.getTemperature();
  float r = dht.getHumidity();
  String ds = String(d);
  String rs = String(r);

  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {

            // دریافت شماره جدید اگر درخواست ست شماره باشد
            if (header.indexOf("GET /setPhone?num=") >= 0) {
              int start = header.indexOf("GET /setPhone?num=") + 18;
              int end = header.indexOf(" ", start);
              phoneNumber = header.substring(start, end);
              //Serial.println("New phone number set: " + phoneNumber);
              //Serial.println(" \n \n");
            }

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // کنترل رله‌ها بر اساس درخواست
            if (header.indexOf("GET /on1") >= 0)
            {
              relay1State = "on";
              digitalWrite(D7, HIGH);
            }
            else if (header.indexOf("GET /off1") >= 0)
            {
              relay1State = "off";
              digitalWrite(D7, LOW);
            }
            else if (header.indexOf("GET /on2") >= 0) {
              relay2State = "on";
              digitalWrite(D5, HIGH);
            }
            else if (header.indexOf("GET /off2") >= 0) {
              relay2State = "off";
              digitalWrite(D5, LOW);
            }
            else if (header.indexOf("GET /on3") >= 0)
            {
              relay3State = "on";
              digitalWrite(D0, HIGH);
            }
            else if (header.indexOf("GET /off3") >= 0)
            {
              relay3State = "off";
              digitalWrite(D0, LOW);
            }
            else if (header.indexOf("GET /on4") >= 0)
            {
              relay4State = "on";
              digitalWrite(D1, HIGH);
            }
            else if (header.indexOf("GET /off4") >= 0)
            {
              relay4State = "off";
              digitalWrite(D1, LOW);
            }

            // صفحه HTML به همراه فرم تغییر شماره
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

            client.println("<style>html, body {background-color: #566573; font-family: Helvetica; display: block; color: black; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: red; border: none; color: black; padding: 8px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: green; border: none; color: black; padding: 8px 24px;}");
            client.println(".textbox {width: 60px; border: 1px  #333; padding: 16px 20px 0px 24px; background-image: linear-gradient(180deg, #fff, #ddd 40%, #ccc);}");
            client.println(".mytext {font-size: 16px; font-weight:bold; font-family:Arial ; text-align: justify;}");
            client.println("#container {width: 100%; height: 100%; margin-left: 5px; margin-top: 20px; padding: 10px; display: -webkit-flex; -webkit-justify-content: center; display: flex; justify-content: center;} ");
            client.println(".temp { background-color: #566573; border: none; color: ORANGE ; padding: 0px 0px;");
            client.println("</style></head>");

            client.println("<body><h1 class=\"temp\" > HO3EIN</h1>");
            client.println("<body><h3 class=\"temp\" > SMART HOME</h3>");
            client.println("<body><h3 class=\"temp\" > Temperature: " +  ds  + "</h3>");
            client.println("<body><h3 class=\"temp\" > Humidity:  " +  rs  + "</h3>");

            // فرم تغییر شماره
            client.println("<h2>Set Phone Number</h2>");
            client.println("<form action=\"/setPhone\" method=\"GET\">");
            client.println("Phone: <input type=\"text\" name=\"num\" value=\"" + phoneNumber + "\"><br>");
            client.println("<input type=\"submit\" value=\"Set\">");
            client.println("</form>");

            // دکمه‌های کنترل رله‌ها
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">Relay 1 </div> ");
            if (relay1State == "off") {
              client.println("<a href=\"/on1\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off1\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");

            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">Relay 2 </div> ");

            if (relay2State == "off") {
              client.println("<a href=\"on2\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"off2\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");

            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">Relay 3 </div>");

            if (relay3State == "off") {
              client.println("<a href=\"on3\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"off3\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");

            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">Relay 4 </div>");

            if (relay4State == "off") {
              client.println("<a href=\"on4\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"off4\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");

            client.println("</body></html>");
            client.println();

            break;

          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();

  }

  if ( Serial.available() ) {
    inputString = "";

    while (Serial.available()) {
      incomingByte = Serial.read();
      inputString += incomingByte;
    }
    if (1)
    {
      if (inputString.indexOf("ON1") > 0) //d1 on
      {
        digitalWrite(D7, HIGH); relay1State = "on"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay1= on"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("OFF1") > 0) //d1 oFF
      {
        digitalWrite(D7, LOW); relay1State = "off"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay1= off"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("ON2") > 0) //d1 on
      {
        digitalWrite(D5, HIGH);  relay2State = "on"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay2= on"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("OFF2") > 0) //d1 on
      {
        digitalWrite(D5, LOW);  relay2State = "off"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay2= off"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("ON3") > 0) //d1 on
      {
        digitalWrite(D0, HIGH); relay3State = "on"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay3= on"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("OFF3") > 0) //d1 oFF
      {
        digitalWrite(D0, LOW); relay3State = "off"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay3= off"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("ON4") > 0) //d1 on
      {
        digitalWrite(D1, HIGH); relay4State = "on"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay4= on"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("OFF4") > 0) //d1 oFF
      {
        digitalWrite(D1, LOW); relay4State = "off"; digitalWrite(D2, HIGH); Serial.println("AT+CMGS=\"" + phoneNumber + "\""); Serial.print("relay4= off"); delay(300); digitalWrite(D2, LOW); Serial.write(26);
      }
      if (inputString.indexOf("Onb") > 0) //d1 on
      {
        digitalWrite(D2, HIGH);
        digitalWrite(D1, HIGH);
        delay(1000);
        digitalWrite(D2, LOW);
        digitalWrite(D1, LOW);
      }
      if (inputString.indexOf("ONALL") > 0) //d1 on
      { Serial.println("AT+CMGS=\"" + phoneNumber + "\"");
        Serial.print("ONALL=OK");
        digitalWrite(D2, HIGH);
        digitalWrite(D7, HIGH); delay(100);
        digitalWrite(D5, HIGH); delay(100);
        digitalWrite(D0, HIGH); delay(100);
        digitalWrite(D1, HIGH); delay(1000);
        digitalWrite(D2, LOW);
        relay1State = "on";
        relay2State = "on";
        relay3State = "on";
        relay4State = "on";
        Serial.write(26);
      }
      if (inputString.indexOf("OFFALL") > 0) //d1 oFF
      { Serial.println("AT+CMGS=\"" + phoneNumber + "\"");
        Serial.print("OFFALL=OK");
        digitalWrite(D2, HIGH);
        digitalWrite(D7, LOW); delay(100);
        digitalWrite(D5, LOW); delay(100);
        digitalWrite(D0, LOW); delay(100);
        digitalWrite(D1, LOW); delay(1000);
        digitalWrite(D2, LOW);
        relay1State = "off";
        relay2State = "off";
        relay3State = "off";
        relay4State = "off";
        Serial.write(26);
      }
      if (inputString.indexOf("state") > 0)
      {
        digitalWrite(D2, HIGH);
        Serial.println("AT+CMGS=\"" + phoneNumber + "\"");
        delay(100);
        Serial.print("humid="); Serial.println(rs);
        Serial.print("tempe="); Serial.println(ds);
        delay(100);
        Serial.print("r1="); Serial.println(relay1State);
        Serial.print("r2="); Serial.println(relay2State);
        Serial.print("r3="); Serial.println(relay3State);
        Serial.print("r4="); Serial.println(relay4State);
        delay(100);
        Serial.write(26);
        delay(100);
        digitalWrite(D2, LOW);

      }

    }
    //Serial.println(inputString);
    delay(100);//
  }
}
