#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>

#include <ESPAsyncWebServer.h>

#include "settings.h"
#include "loopInterval.h"
#include "clicker.h"
#include "htmlPages.h"

// Номер пина для первого тумблера. Должен быть подключен к пину в режиме INPUT_PULLUP и к GND
const int clicker1ToggleSwitcherPin = 16;

// Второй тумблер
const int clicker2ToggleSwitcherPin = 17;

static struct Clicker clicker1(clicker1ToggleSwitcherPin);
static struct Clicker clicker2(clicker2ToggleSwitcherPin);

// Флаг, что нужно отправить события на клиент через SSE
bool needSendUpdate = false;

const String eventNameKeyState = "keyState";

// timer for print debug messages
static struct LoopInterval statePrintInterval(5000);

// timer for send update keys
static struct LoopInterval sendKeysInterval(20);

// sse ping interval
static struct LoopInterval ssePingInterval(5000);

// Web server

static const size_t htmlContentLength = strlen_P(htmlContent);

static AsyncWebServer server(80);
static AsyncCorsMiddleware cors;
static AsyncEventSource events("/events");

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Web server end

// helpers

void updateClickers() {
  if (updateClicker(clicker1) || updateClicker(clicker2)) {
    needSendUpdate = true;
  }
}

void sendEventsIfKeysChanged() {
  if (!needSendUpdate) {
    return;
  }

  events.send(
    "k1:" + String(clicker1.lastSwitcherState) + " k2:" + String(clicker2.lastSwitcherState),
    eventNameKeyState,
    millis());

  needSendUpdate = false;  
}

void setupWiFiServer() {
  // Handle Web Server
  server.on("/debug", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", (uint8_t*)htmlContent, htmlContentLength);
  });

  server.onNotFound([](AsyncWebServerRequest* request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient* client) {
    client->client()->setNoDelay(true);  // Убираем задержку для частых SSE

    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Private-Network", "true");
  cors.setMethods("POST, GET, OPTIONS, DELETE");
  cors.setAllowCredentials(false);

  server.addMiddleware(&cors);
  server.addHandler(&events);
}
// helpers end

void setup() {
  Serial.begin(115200);
  initWiFi();

  clicker1.setup();
  clicker2.setup();

  updateClickers();
  updateClickers();

  setupWiFiServer();
  server.begin();
}

static uint32_t lastHeap = 0;

void loop() {
  unsigned long now = millis();

  updateClickers();

  if (sendKeysInterval.loopInterval(now) > 0) {
    sendEventsIfKeysChanged();
  }

  if (ssePingInterval.loopInterval(now) > 0) {
    events.send(String("ping-") + now, "heartbeat", now);
  }

  // debug
  if (statePrintInterval.loopInterval(now) > 0) {
    Serial.print("Clicker1 State = ");
    Serial.print(clicker1.switcher.getButtonState());

    Serial.print(" Clicker1 Click count = ");
    Serial.print(clicker1.clickCount);

    Serial.println("");
  }

#ifdef ESP32
  if (now - lastHeap >= 5000) {
    Serial.printf("Free heap: %" PRIu32 "\n", ESP.getFreeHeap());
    lastHeap = now;
  }
#endif
}
