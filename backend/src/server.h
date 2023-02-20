#ifndef SERVER_H
#define SERVER_H

#include <WiFiClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "ducky.h"
#include "react.h"
#include "keyboard.h"




void serverTask(void *pvParameters);

#endif