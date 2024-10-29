// #include "SubscribeStorageChange.h"
// #include "WebsocketUtils.h"
// #include <Arduino_JSON.h>
// #include "Secret.h"
// #include "BlockchainUtils.h"

// void subscribeStorageChange() {
//     setOnTextCallback(changeStorageCallback);
//     sendSubscribeStorageMessage();
// }

// void sendSubscribeStorageMessage() {
//     JSONVar paramsArray;
//     paramsArray[0][0] = STORAGE_KEY;
//     String message = createWebsocketMessage("state_subscribeStorage", paramsArray, requestId);
//     rpcRequest(message);
// }

// void changeStorageCallback(uint8_t *payload) {
//     JSONVar received_message = JSON.parse((char *)payload);
//     Serial.print("Received data: ");
//     Serial.println(JSON.stringify(received_message));
//     String changed_storage_data = JSON.stringify(received_message["params"]["result"]["changes"][0][1]);
//     Serial.print("Changed data: ");
//     Serial.println(changed_storage_data);
//     if (checkPublicKeyInMessage(&changed_storage_data) && checkPONGInMessage(&changed_storage_data)) {
//         disconnectWebSocket();
//         Serial.println("Got result: PONG");
//     }
// }

// bool checkPublicKeyInMessage(String *message) {
//     std::string message_std = message->c_str();
//     bool result = (message_std.find(PUB_KEY) != std::string::npos);
//     Serial.print("Public key in address: ");
//     Serial.println(result);
//     return result;
// }

// bool checkPONGInMessage(String *message) {
//     std::string message_std = message->c_str();
//     return (message_std.find(HEX_PONG) != std::string::npos);
// }

// void sendSubscribeToBlocksMessage() {
//     paramsArray[0][0] = "";
//     String message = createWebsocketMessage("chain_subscribeFinalizedHeads", paramsArray, requestId);
//     rpcRequest(message);
// }