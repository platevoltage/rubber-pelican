
#include "server.h"


WebServer server(80);

#define STASSID "Can't stop the signal, Mal"
#define STAPSK "youcanttaketheskyfromme"
#define BONJOURNAME "test"

String ssid = "";
String password = "";
String bonjourName = "rubber-pelican";


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
struct TaskParameters {
  String string;
  int startOnLine;
};

void interpretDuckyScript(void *parameter) {

  // const char *param = (const char*)parameter;
  // String string = String((const char*)parameter);
  TaskParameters* params = (TaskParameters*) parameter;
  String string = params->string;
  int startOnLine = params->startOnLine;
  // sendHeaders();
  Serial1.println(string);
  string.replace("ELSE IF", "ELSE\nIF");
  int commands_t = 0;
  DuckyCommand * commands = splitByLine(string + '\n', &commands_t);
  for (int i = 0; i < commands_t; i++) {
    Serial1.print("Line ");
    Serial1.print(i + 1);
    Serial1.print(": ");
    Serial1.print(commands[i].instruction);
    Serial1.print(" () ");
    Serial1.println(commands[i].parameter);
  }

  DuckyCallbacks callbacks = {keyboardCallback, keyboardKeyPressCallback, keyboardKeyHoldCallback, keyboardShortcutCallback, delayCallback, ledCallback, buttonCallback, enableHIDCallback, enableFlashCallback, disableUSBCallback, setUSBPropertiesCallback, restartCallback}; //contains our callbacks
  duckyBlock(commands, commands_t, callbacks, startOnLine);


  server.send(200, "text/plain", string);
  vTaskDelete(NULL);
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  Serial1.println(upload.status);
  if (upload.status == UPLOAD_FILE_START) {
    // Start file upload
    Serial1.printf("Starting file upload: %s\n", upload.filename.c_str());
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // Write data to file
    Serial1.printf("Writing file data: %d bytes\n", upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    // End file upload
    Serial1.printf("Finished file upload: %s, size: %d\n", upload.filename.c_str(), upload.totalSize);
  }
}

void resumeDuckyScript(String string, int startOnLine) {
    int len = string.length() + 1;
    char *buf = new char[len];
    string.toCharArray(buf, len);
    TaskParameters params = {string, startOnLine};
    Serial1.print("STARTONLINE2 - ");
    Serial1.println(params.startOnLine);
    xTaskCreate(interpretDuckyScript, "duckyTask", 10000, &params, 2, NULL); // pass the address of param as pvParameters
}

void runOnBoot(String body) {
  writeFile( "/.cache.txt", body.c_str());
  int len = body.length() + 1;
  char *buf = new char[len];
  body.toCharArray(buf, len);

  Serial1.println(body);
  Serial1.println(buf);
  TaskParameters params = {buf, 0};
  xTaskCreate(interpretDuckyScript, "duckyTask", 10000, &params, 2, NULL); // pass the address of param as pvParameters
}


void startInterpretDuckyScript() {
  sendHeaders();

  String body = server.arg("plain");
  writeFile( "/.cache.txt", body.c_str());
  int len = body.length() + 1;
  char *buf = new char[len];
  body.toCharArray(buf, len);

  Serial1.println(body);
  Serial1.println(buf);
  TaskParameters params = {buf, 0};
  xTaskCreate(interpretDuckyScript, "duckyTask", 10000, &params, 2, NULL); // pass the address of param as pvParameters
  server.send(200, "text/plain", "success");
}
void handleSave() {
  sendHeaders();

  String body = server.arg("plain");
  writeFile( "/inject.txt", body.c_str());
  server.send(200, "text/plain", body);
}

void handleRecover() {
  sendHeaders();
  String body = readFile("/inject.txt");

  server.send(200, "text/plain", body);
}

void restart(void *parameter) {
  vTaskDelay(1000/portTICK_PERIOD_MS);
  esp_sleep_enable_timer_wakeup(5 * 100000);
  esp_deep_sleep_start();
}
void handleMountSystem() {
  sendHeaders();
  server.send(200, "text/plain", "Success");
  systemDriveActivated = true;
  // esp_sleep_enable_timer_wakeup(5 * 100000); 
  // esp_deep_sleep_start();
  xTaskCreate(restart, "restart", 1000, NULL, 3, NULL); // pass the address of param as pvParameters
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
    server.on(F("/duckyscript"), HTTP_POST, startInterpretDuckyScript);
    server.on(F("/save"), HTTP_POST, handleSave);
    server.on(F("/upload"), HTTP_POST, handleFileUpload);
    server.on(F("/recover"), HTTP_GET, handleRecover);
    server.on(F("/mountsystem"), HTTP_POST, handleMountSystem);

    server.onNotFound(handleNotFound);
    server.enableCORS(true);
    server.begin();
    Serial1.println(F("HTTP server started"));

}

void serverTask(void *pvParameters) {
  // String string = readFile( "/ducky.txt");
  // Serial1.println(string);
  // int startOnLine = 1;
  // continueDuckyScript(string, startOnLine);
  serverStart();
  while (true) {
    server.handleClient();
    delay(1);
  }
}




