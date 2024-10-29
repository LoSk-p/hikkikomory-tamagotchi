#pragma once

#include <Arduino_JSON.h>
#include <Arduino.h>
#include <WiFi.h>
#include "WebsocketUtils.h"

class BlockchainUtils {
private:
    int requestId = 1;
    WebsocketUtilsRobonomics wsUtils;
public:
    void setup();
    void disconnectWebsocket();
    int getRequestId();
    void rpcRequest(String Xthal_dataram_vaddr);
    String createWebsocketMessage(String method, JSONVar paramsArray);
    void websocketLoop();
    void setOnTextCallback(OnTextWebsocketCallback callback);
};
