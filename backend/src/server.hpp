
#include <WiFiClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "react.hpp"
#include "ducky.hpp"
#include "keyboard.hpp"

WebServer server(80);


#define STASSID "Can't stop the signal, Mal"
#define STAPSK "youcanttaketheskyfromme"
#define BONJOURNAME "test"

static String ssid = STASSID;
static String password = STAPSK;
static String bonjourName = BONJOURNAME;


void sendHeaders() {
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
}


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

void interpretDuckyScript() {
  sendHeaders();
  Serial1.println(server.arg("plain") + '\n');
  int commands_t = 0;
  DuckyCommand * commands = splitByLine(server.arg("plain") + '\n', &commands_t);
  for (int i = 0; i < commands_t; i++) {
    Serial1.print("Line ");
    Serial1.print(i + 1);
    Serial1.print(": ");
    Serial1.print(commands[i].instruction);
    Serial1.print(" () ");
    Serial1.println(commands[i].parameter);
  }
  duckyBlock(commands, commands_t, keyboardCallBack);
  server.send(200, "text/plain", server.arg("plain").c_str());
}


void serverStart() {

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

        server.on(F("/"), []() {
      server.send(200, "text/html", _index_html);
    });

    //-----begin generated paths


    server.on(F(MANIFEST_JSON_PATH), []() {
      server.send_P(200, "text/json", _manifest_json);
    });
    server.on(F(MAIN_JS_PATH), []() {
      server.send_P(200, "text/css", _main_css);
    });
    server.on(F(MAIN_CSS_PATH), []() {
      server.send_P(200, "text/javascript", _main_js);
    });
    server.on(F(CHUNK_JS_PATH), []() {
      server.send_P(200, "text/javascript", _chunk_js);
    });
    server.on(F(INDEX_HTML_PATH), []() {
      server.send_P(200, "text/html", _index_html);
    });


    // server.on(F("/typestring"), *typeString);
    server.on(F("/duckyscript"), interpretDuckyScript);


    server.onNotFound(handleNotFound);
    server.enableCORS(true);
    server.begin();
    Serial1.println(F("HTTP server started"));

}

void serverTask(void *pvParameters) {
  serverStart();

  while (true) {
    server.handleClient();
    delay(1);
  }
}




