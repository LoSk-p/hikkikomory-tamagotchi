// #include "Declares.h"
// #include "Defines.h"
#include "PayloadParamsUtils.h"
#include "esp_log.h"

static const char *TAG = "ROBONOMICS";

JSONVar emptyParamsArray;
JSONVar paramsArray;
// bool connectionClosed = false;
JSONVar received_message;
int nonce;
bool got_nonce = false;
std::string block_hash = "";
const char* chain_head;
bool got_chain_head = false;
const char* parent_block_hash;
bool got_parent_block_hash = false;
JSONVar runtime_info;
bool got_runtime_info = false;

uint32_t getEra() {
    return 0;
}

uint64_t getTip() {
    return 0;
}

std::string getGenesisBlockHash() {
    // return "525639f713f397dcf839bd022cd821f367ebcf179de7b9253531f8adbe5436d6"; // Vara
    return "631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc"; // Robonomics
}

uint32_t getSpecVersion(JSONVar runtimeInfo) {
    int specVersion_ = (int) (runtimeInfo["specVersion"]);
    return (uint32_t) specVersion_;
}

uint32_t getTransactionVersion(JSONVar runtimeInfo) {
    int tx_version_ = (int)(runtimeInfo["transactionVersion"]);
    return (uint32_t) tx_version_;
}

// Get Nonce

uint64_t getNonce(BlockchainUtils *blockchainUtils, char *ss58Address) {
    blockchainUtils->setOnTextCallback(getNonceCallback);
    sendGetAccountNonceMessage(blockchainUtils, ss58Address);
    while (!got_nonce) {
        blockchainUtils->websocketLoop();
    }
    // Serial.print("Nonce: ");
    // Serial.println(nonce);
    got_nonce = false;
    return (uint64_t) nonce;
}

void getNonceCallback(uint8_t *payload) {
    ESP_LOGI(TAG, "Nonce callback: %s", payload);
    received_message = JSON.parse((char *)payload);
    // Serial.println("After parse");
    nonce = (int) (received_message["result"]);
    got_nonce = true;
    // Serial.println("After getting nonce from JSON");
}

void sendGetAccountNonceMessage(BlockchainUtils *blockchainUtils, char *ss58Address) {
    paramsArray[0] = ss58Address;
    String message = blockchainUtils->createWebsocketMessage("system_accountNextIndex", paramsArray);
    blockchainUtils->rpcRequest(message);
}

// Get Block Hash

std::string getBlockHash(BlockchainUtils *blockchainUtils) {
    blockchainUtils->setOnTextCallback(getBlockHashCallback);
    sendGetBlockHashMessage(blockchainUtils);
    while (block_hash == "") {
        blockchainUtils->websocketLoop();
    }
    // Serial.printf("Block hash: %x\n", block_hash);
    return block_hash;
}

void getBlockHashCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    block_hash = (const char*) received_message["result"];
}

void sendGetBlockHashMessage(BlockchainUtils *blockchainUtils) {
    paramsArray[0] = 0;
    String message = blockchainUtils->createWebsocketMessage("chain_getBlockHash", paramsArray);
    blockchainUtils->rpcRequest(message);
}

// Get Runtime Info

JSONVar getRuntimeInfo(BlockchainUtils *blockchainUtils) {
    const char* chain_head_local = getChainHead(blockchainUtils);
    const char* parent_block_local = getParentBlockHash(chain_head_local, blockchainUtils);
    return getRuntimeInfo(parent_block_local, blockchainUtils);
}

JSONVar getRuntimeInfo(const char* parentBlockHash, BlockchainUtils *blockchainUtils) {
    blockchainUtils->setOnTextCallback(getRuntimeInfoCallback);
    sendGetRuntimeInfoMessage(parentBlockHash, blockchainUtils);
    while (!got_runtime_info) {
        blockchainUtils->websocketLoop();
    }
    got_runtime_info = false;
    return runtime_info;
}

void getRuntimeInfoCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    runtime_info = received_message["result"];
    got_runtime_info = true;
}

void sendGetRuntimeInfoMessage(const char* parentBlockHash, BlockchainUtils *blockchainUtils) {
    paramsArray[0] = parentBlockHash;
    String message = blockchainUtils->createWebsocketMessage("state_getRuntimeVersion", paramsArray);
    blockchainUtils->rpcRequest(message);
}

// Get Chain Head

const char* getChainHead(BlockchainUtils *blockchainUtils) {
    blockchainUtils->setOnTextCallback(getChainHeadCallback);
    sendGetChainHeadMessage(blockchainUtils);
    while (!got_chain_head) {
        blockchainUtils->websocketLoop();
    }
    // Serial.print("Chain head: ");
    // Serial.println(chain_head);
    got_chain_head = false;
    return chain_head;
}

void getChainHeadCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    chain_head = (const char *) (received_message["result"]);
    got_chain_head = true;
}

void sendGetChainHeadMessage(BlockchainUtils *blockchainUtils) {
    // paramsArray[0] = "";
    String message = blockchainUtils->createWebsocketMessage("chain_getHead", emptyParamsArray);
    blockchainUtils->rpcRequest(message);
}

// Get Parent Block Hash

const char* getParentBlockHash(const char* chainHead, BlockchainUtils *blockchainUtils) {
    blockchainUtils->setOnTextCallback(getParentBlockHashCallback);
    sendGetParentBlockHashMessage(chainHead, blockchainUtils);
    while (!got_parent_block_hash) {
        blockchainUtils->websocketLoop();
    }
    // Serial.print("Chain header: ");
    // Serial.println(parent_block_hash);
    got_parent_block_hash = false;
    return parent_block_hash;
}

void getParentBlockHashCallback(uint8_t *payload) {
    received_message = JSON.parse((char *)payload);
    parent_block_hash = (const char *) (received_message["result"]["parentHash"]);
    got_parent_block_hash = true;
}

void sendGetParentBlockHashMessage(const char* chainHead, BlockchainUtils *blockchainUtils) {
    paramsArray[0] = chainHead;
    String message = blockchainUtils->createWebsocketMessage("chain_getHeader", paramsArray);
    blockchainUtils->rpcRequest(message);
}

// Get Metadata

bool got_metadata = false;

void getChainMetadata(BlockchainUtils *blockchainUtils) {
    blockchainUtils->setOnTextCallback(getMetadataCallback);
    sendGetMetadataMessage(blockchainUtils);
    while (!got_metadata) {
        blockchainUtils->websocketLoop();
    }
    // Serial.print("Nonce: ");
    // Serial.println(nonce);
    // return (uint64_t) nonce;
}

void getMetadataCallback(uint8_t *payload) {
    // received_message = JSON.parse((char *)payload);
    // Serial.println("After parse");
    // nonce = (int) (received_message["result"]);
    got_metadata = true;
    // Serial.println("Got metadata");
    // Serial.println("After getting nonce from JSON");
}

void sendGetMetadataMessage(BlockchainUtils *blockchainUtils) {
    // paramsArray[0] = SS58_ADR;
    String message = blockchainUtils->createWebsocketMessage("state_getMetadata", emptyParamsArray);
    blockchainUtils->rpcRequest(message);
}