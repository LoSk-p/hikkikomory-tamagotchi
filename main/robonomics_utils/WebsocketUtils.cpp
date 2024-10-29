#include "WebsocketUtils.h"
#include "esp_log.h"

static const char *TAG = "ROBONOMICS";

void WebsocketUtilsRobonomics::websocketLoop() {
    if (!connectionClosed) {
        webSocket.loop();
    }
}

void WebsocketUtilsRobonomics::websocketSendMessage(String text) {
    webSocket.sendTXT(text);
}

void WebsocketUtilsRobonomics::setupWebsocket() {
    webSocket.beginSSL(WS_SERVER_HOST, WS_SERVER_PORT, "/");
    connectWebscoket();
}

void WebsocketUtilsRobonomics::disconnect() {
    webSocket.disconnect();
}

void WebsocketUtilsRobonomics::disconnectWebSocket() {
    ESP_LOGI(TAG, "Disconnected from websocket");
    webSocket.disconnect();
    connectionClosed = true;
}

void WebsocketUtilsRobonomics::connectWebscoket() {
    connectionClosed = false;
    // webSocket.onEvent(this->mainWebsocketCallback);
    webSocket.onEvent([&](WStype_t t, uint8_t * p, size_t l) {
        mainWebsocketCallback(t, p, l);
    });
    while (!websocketConnected) {
        websocketLoop();
    }
    setOnTextCallback(& defaultOnTextCallback);
    // setOnTextCallback(this->defaultOnTextCallback);
}
void WebsocketUtilsRobonomics::setOnTextCallback(OnTextWebsocketCallback callback) {
    onTextCallback = callback;
}

void WebsocketUtilsRobonomics::mainWebsocketCallback(WStype_t type, uint8_t *payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            ESP_LOGI(TAG, "[WSc] Disconnected!");
            websocketConnected = false;
            break;
        case WStype_CONNECTED: {
            ESP_LOGI(TAG, "[WSc] Connected to url: %s", payload);
            websocketConnected = true;
        }
            break;
        case WStype_TEXT:
            ESP_LOGI(TAG, "[WSc] RESPONSE: %s", (char *)payload);
            onTextCallback(payload);
            break;
        case WStype_BIN:
        case WStype_ERROR:
            ESP_LOGE(TAG, "[WSc] Error occurred!");
            // Handle error if necessary
            break;
        case WStype_PING:
        case WStype_PONG:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        default:
            // ESP_LOGW(TAG, "[WSc] Unknown websocket event type");
            break;
    }
}

void defaultOnTextCallback(uint8_t *payload) {
    ESP_LOGI(TAG, "Default on text callback");
}