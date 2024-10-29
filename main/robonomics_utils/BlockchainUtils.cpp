#include "BlockchainUtils.h"

void BlockchainUtils::setup() {
    wsUtils.setupWebsocket();
}

void BlockchainUtils::disconnectWebsocket() {
    wsUtils.disconnect();
}

void BlockchainUtils::rpcRequest(String data) {
    // Serial.print("Sent to ws: ");
    // Serial.println(data);
    wsUtils.websocketSendMessage(data);
    // Serial.print("Sent to ws after: ");
    requestId++;
}

String BlockchainUtils::createWebsocketMessage(String method, JSONVar paramsArray) {
    JSONVar messageObject;
    messageObject["jsonrpc"] = "2.0";
    messageObject["method"] = method;
    messageObject["params"] = paramsArray;
    messageObject["id"] = requestId;
    String messageString = JSON.stringify(messageObject);
    return messageString;
}

int BlockchainUtils::getRequestId() {
    return requestId;
}

void BlockchainUtils::websocketLoop() {
    wsUtils.websocketLoop();
}

void BlockchainUtils::setOnTextCallback(OnTextWebsocketCallback callback) {
    wsUtils.setOnTextCallback(callback);
}