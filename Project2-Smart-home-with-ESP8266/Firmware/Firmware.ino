#include <ESP8266WiFi.h>
//    Ho3ein       ktyt5708    ARMAN.M    13833831

const char* ssid = "ARMAN.M"; // نام وای فای مورد نظر
const char* password = "13833831"; // پسوورد وای فای

WiFiServer server(80);

String header;

String inputString;
char incomingByte;

String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";
String relay5State = "off";
String relay6State = "off";

void setup() {

  WiFi.begin(ssid, password);
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  delay(1000);

  Serial.println("AT+CSMP=17,167,2,25\r");
  delay(100);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CNMI=1,2,0,0,0");
  delay(100);
  Serial.println("AT+CMGL=\"REC UNREAD\"");
  delay(100);
  Serial.println("AT+CMGS=\"+989923607269\"");
  delay(100);
  Serial.println("HELLO ARMAN IM READY");
  delay(100);
  Serial.write(26);
  delay(100);

  server.begin();
}
void loop() {

  int analog_value = analogRead(A0);
  int voltage = (analog_value * 3) / 10;

  String ds = String(voltage);
  delay(100);
  Serial.println(26);

  /* wifi  */
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //با توجه به درخواست های دریافت شده پین فعال میشود
            if (header.indexOf("GET /on1") >= 0)
            {
              relay1State = "on";
              digitalWrite(D1, HIGH);
            }
            else if (header.indexOf("GET /off1") >= 0)
            {
              relay1State = "off";
              digitalWrite(D1, LOW);
            }
            else if (header.indexOf("GET /on2") >= 0)
            {
              relay2State = "on";
              digitalWrite(D2, HIGH);
            }
            else if (header.indexOf("GET /off2") >= 0)
            {
              relay2State = "off";
              digitalWrite(D2, LOW);
            }
            else if (header.indexOf("GET /on3") >= 0)
            {
              relay3State = "on";
              digitalWrite(D8, HIGH);
            }
            else if (header.indexOf("GET /off3") >= 0)
            {
              relay3State = "off";
              digitalWrite(D8, LOW);
            }
            else if (header.indexOf("GET /on4") >= 0)
            {
              relay4State = "on";
              digitalWrite(D7, HIGH);
            }
            else if (header.indexOf("GET /off4") >= 0)
            {
              relay4State = "off";
              digitalWrite(D7, LOW);
            }
            else if (header.indexOf("GET /on5") >= 0)
            {
              relay5State = "on";
              digitalWrite(D5, HIGH);
            }
            else if (header.indexOf("GET /off5") >= 0)
            {
              relay5State = "off";
              digitalWrite(D5, LOW);
            }
            else if (header.indexOf("GET /on6") >= 0)
            {
              relay6State = "on";
              digitalWrite(D6, HIGH);
            }
            else if (header.indexOf("GET /off6") >= 0)
            {
              relay6State = "off";
              digitalWrite(D6, LOW);
            }
            else if (header.indexOf("GET /onall") >= 0)
            {
              digitalWrite(D1, HIGH); delay(100);
              digitalWrite(D2, HIGH); delay(100);
              digitalWrite(D8, HIGH); delay(100);
              digitalWrite(D7, HIGH); delay(100);
              digitalWrite(D5, HIGH); delay(100);
              digitalWrite(D6, HIGH);
              relay1State = "on";
              relay2State = "on";
              relay3State = "on";
              relay4State = "on";
              relay5State = "on";
              relay6State = "on";
            }
            else if (header.indexOf("GET /offall") >= 0)
            {
              digitalWrite(D1, LOW); delay(100);
              digitalWrite(D2, LOW); delay(100);
              digitalWrite(D8, LOW); delay(100);
              digitalWrite(D7, LOW); delay(100);
              digitalWrite(D5, LOW); delay(100);
              digitalWrite(D6, LOW);
              relay1State = "off";
              relay2State = "off";
              relay3State = "off";
              relay4State = "off";
              relay5State = "off";
              relay6State = "off";

            }


            // صفحه HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

            // میتوانید مقادیر را با توجه به نیاز خود شخصی سازی کنید

            client.println("<style>html, body {background-color: BLACK; font-family: Helvetica; display: block; color: black; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: red; border: none; color: black; padding: 8px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: green; border: none; color: black; padding: 8px 24px;}");
            client.println(".textbox {width: 60px; border: 1px  #333; padding: 16px 20px 0px 24px; background-image: linear-gradient(180deg, #fff, #ddd 40%, #ccc);}");
            client.println(".mytext {font-size: 16px; font-weight:bold; font-family:Arial ; text-align: justify;}");
            client.println("#container {width: 100%; height: 100%; margin-left: 5px; margin-top: 20px; padding: 10px; display: -webkit-flex; -webkit-justify-content: center; display: flex; justify-content: center;} ");
            client.println(".temp { background-color: black; border: none; color: ORANGE ; padding: 0px 24px;");

            client.println("</style></head>");


            client.println("<body><h1 class=\"temp\" > ARMAN</h1>");
            client.println("<body><h3 class=\"temp\" > SMART HOME</h3>");
            client.println("<body><h3 class=\"temp\" > TEMP  " +  ds  + "</h3>");

            // المان های داخل صفحه وب تعریف می شوند
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////1
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 1 </div> ");
            if (relay1State == "off") {
              client.println("<a href=\"/on1\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off1\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////2
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 2 </div> ");

            if (relay2State == "off") {
              client.println("<a href=\"/on2\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off2\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////3
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 3 </div>");

            if (relay3State == "off") {
              client.println("<a href=\"/on3\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off3\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////4
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 4 </div>");

            if (relay4State == "off") {
              client.println("<a href=\"/on4\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off4\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////5
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 5 </div>");

            if (relay5State == "off") {
              client.println("<a href=\"/on5\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off5\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////6
            client.println("<div id=\"container\">");
            client.println("<p><div class=\"textbox mytext\">REL 6 </div>");

            if (relay6State == "off") {
              client.println("<a href=\"/on6\"><button class=\"button\">OFF</button></a></p>");
            } else {
              client.println("<a href=\"/off6\"><button class=\"button button2\">ON</button></a></p>");
            }
            client.println("</div>");
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////6
            client.println("<div id=\"container\">");


            client.println("<a href=\"/onall\"><button class=\"button button2\">ON ALL</button></a></p>");

            client.println("<a href=\"/offall\"><button class=\"button \">OFF ALL</button></a></p>");

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

  if (Serial.available()) {
    inputString = "";

    while (Serial.available()) {
      incomingByte = Serial.read();
      inputString += incomingByte;
    }
    if (1) {
      if (inputString.indexOf("ON1") > 0) //d1 on
      {
        digitalWrite(D1, HIGH); relay1State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 1 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF1") > 0) //d1 oFF
      {
        digitalWrite(D1, LOW); relay1State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 1 = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ON2") > 0) //d1 on
      {
        digitalWrite(D2, HIGH); relay2State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 2 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF2") > 0) //d1 on
      {
        digitalWrite(D2, LOW);  relay2State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 2` = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ON3") > 0) //d1 on
      {
        digitalWrite(D8, HIGH); relay3State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 3 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF3") > 0) //d1 oFF
      {
        digitalWrite(D8, LOW);  relay3State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 3 = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ON4") > 0) //d1 on
      {
        digitalWrite(D7, HIGH); relay4State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 4 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF4") > 0) //d1 oFF
      {
        digitalWrite(D7, LOW);  relay4State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 4 = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ON5") > 0) //d1 on
      {
        digitalWrite(D5, HIGH); relay5State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 5 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF5") > 0) //d1 oFF
      {
        digitalWrite(D5, LOW);  relay5State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 5 = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ON6") > 0) //d1 on
      {
        digitalWrite(D6, HIGH);  relay6State = "on"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 6 = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFF6") > 0) //d1 oFF
      {
        digitalWrite(D6, LOW); relay6State = "off"; Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("REL 6 = OFF"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("ONALL") > 0) //d1 on
      {
        digitalWrite(D1, HIGH); delay(100);
        digitalWrite(D2, HIGH); delay(100);
        digitalWrite(D8, HIGH); delay(100);
        digitalWrite(D7, HIGH); delay(100);
        digitalWrite(D5, HIGH); delay(100);
        digitalWrite(D6, HIGH);
        relay1State = "on";
        relay2State = "on";
        relay3State = "on";
        relay4State = "on";
        relay5State = "on";
        relay6State = "on";
        Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("ALL REL = ON"); delay(100); Serial.write(26);
      }
      if (inputString.indexOf("OFFALL") > 0) //d1 oFF
      {
        digitalWrite(D1, LOW); delay(100);
        digitalWrite(D2, LOW); delay(100);
        digitalWrite(D8, LOW); delay(100);
        digitalWrite(D7, LOW); delay(100);
        digitalWrite(D5, LOW); delay(100);
        digitalWrite(D6, LOW);
        relay1State = "off";
        relay2State = "off";
        relay3State = "off";
        relay4State = "off";
        relay5State = "off";
        relay6State = "off";
        Serial.println("AT+CMGS=\"+989923607269\""); Serial.print("ALL REL = OFF"); delay(100); Serial.write(26);
      }

      if (inputString.indexOf("STATE") > 0)
      {
        Serial.println("AT+CMGS=\"+989923607269\"");
        Serial.print("TEMP "); Serial.println(ds);
        Serial.print("REL1 "); Serial.println(relay1State);
        Serial.print("REL2 "); Serial.println(relay2State);
        Serial.print("REL3 "); Serial.println(relay3State);
        Serial.print("REL4 "); Serial.println(relay4State);
        Serial.print("REL5 "); Serial.println(relay5State);
        Serial.print("REL6 "); Serial.println(relay6State);
        delay(100);
        Serial.write(26);


      }
    }

  }

}
