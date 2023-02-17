
#include <WiFiClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);


#define STASSID "Can't stop the signal, Mal"
#define STAPSK "youcanttaketheskyfromme"
#define BONJOURNAME "test"

static String ssid = STASSID;
static String password = STAPSK;
static String bonjourName = BONJOURNAME;

void handleNotFound() {
  // digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  // digitalWrite(LED_BUILTIN, 0);
}


void serverStart(void(*typeString)()) {

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.mode(WIFI_STA);
    String hostname = "rubber-pelican-";
    hostname.concat(bonjourName);

    if (ssid == "") {

      WiFi.hostname(hostname);
      WiFi.softAP("rubber-pelican");
      IPAddress myIP = WiFi.softAPIP();
      Serial1.print("AP IP address: ");
      Serial1.println(myIP);

    } else {
      const char* _ssid = ssid.c_str(); 
      const char* _password = password.c_str(); 
      WiFi.begin(_ssid, _password);
      //   Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial1.print(F("."));
      }
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial1.println(F("Connection Failed! Rebooting..."));
        delay(5000);
        ESP.restart();
      }
    }

    Serial1.println();
    Serial1.print(F("Connected to "));
    Serial1.println(ssid);
    Serial1.print(F("IP address: "));
    Serial1.println(WiFi.localIP());

    if (MDNS.begin(bonjourName.c_str())) {
      Serial1.println(F("MDNS responder started"));
      Serial1.println(bonjourName);
    }


    //-----begin generated paths





    //-----end generated paths

    server.on(F("/typestring"), *typeString);


    server.onNotFound(handleNotFound);
    server.enableCORS(true);
    server.begin();
    Serial1.println(F("HTTP server started"));

}


void sendHeaders() {
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
}




static unsigned long webClientPreviousMillis = 0;
void webClientTimer(uint16_t speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - webClientPreviousMillis >= speed) {
      webClientPreviousMillis = currentMillis;
      server.handleClient();
      yield();
    }
}

