#include "Robonomics.h"
#include "esp_log.h"
#include <inttypes.h>
#include "address.h"

static const char *TAG = "ROBONOMICS";


Robonomics::Robonomics(const NetworkConfig& config) : networkConfig(&config) {
    ss58Address.resize(networkConfig->addressSize + 1); // +1 for null terminator
}

void Robonomics::setup() {
    blockchainUtils.setup(networkConfig->wsHost);
}

void Robonomics::disconnectWebsocket() {
    blockchainUtils.disconnectWebsocket();
}

void Robonomics::setPrivateKey(uint8_t *privateKey) {
    memcpy(privateKey_, privateKey, KEYS_SIZE);
    char* tempAddress = getAddrFromPrivateKey(privateKey_, networkConfig->addressPrefix);
    if (tempAddress != nullptr) {
        strncpy(ss58Address.data(), tempAddress, networkConfig->addressSize);
        ss58Address[networkConfig->addressSize] = '\0'; // Ensure null termination
        delete[] tempAddress;
        ESP_LOGI(TAG, "Robonomics Address: %s", ss58Address.data());
    } else {
        ESP_LOGE(TAG, "Failed to get address from public key.");
    }
}

const char* Robonomics::getSs58Address() const {
        return ss58Address.data();
}

void Robonomics::sendCustomCall() {
    Data call = createCall();

    createAndSendExtrinsic(call);
}

void Robonomics::sendDatalogRecord(std::string data) {
    Data head_dr_  = Data{0x33,0};
    Data call = callDatalogRecord(head_dr_, data);

    createAndSendExtrinsic(call);
}

void Robonomics::sendRWSDatalogRecord(std::string data, const char *owner_address) {
    Data head_dr_ = Data{0x33,0};
    Data head_rws_ = Data{0x37,0};
    Data call_nested = callDatalogRecord(head_dr_, data);
    RobonomicsPublicKey ownerKey = getPublicKeyFromAddr(owner_address);
    Data call = callRws(head_rws_, ownerKey, call_nested);

    createAndSendExtrinsic(call);
}

void Robonomics::createAndSendExtrinsic(Data call) {
    Ed25519::derivePublicKey(publicKey_, privateKey_);

    uint64_t payloadNonce = getNonce(& blockchainUtils, ss58Address.data());
    // std::string payloadBlockHash = getGenesisBlockHash();
    std::string payloadBlockHash = networkConfig->genesisBlockHash;
    uint32_t payloadEra = getEra();
    uint64_t payloadTip = getTip();
    JSONVar runtimeInfo = getRuntimeInfo(& blockchainUtils);
    uint32_t payloadSpecVersion = getSpecVersion(runtimeInfo);
    uint32_t payloadTransactionVersion = getTransactionVersion(runtimeInfo);
    ESP_LOGI(TAG, "Spec version: %" PRIu32 ", tx version: %" PRIu32 ", nonce: %llu, era: %" PRIu32 ", tip: %llu, genesis block: %s", payloadSpecVersion, payloadTransactionVersion, (unsigned long long)payloadNonce, payloadEra, (unsigned long long)payloadTip, payloadBlockHash.c_str());
    Data data_ = createPayload(call, payloadEra, payloadNonce, payloadTip, payloadSpecVersion, payloadTransactionVersion, payloadBlockHash, payloadBlockHash);
    Data signature_ = createSignature(data_, privateKey_, publicKey_);
    std::vector<std::uint8_t> pubKey( reinterpret_cast<std::uint8_t*>(std::begin(publicKey_)), reinterpret_cast<std::uint8_t*>(std::end(publicKey_)));
    Data edata_ = createSignedExtrinsic(signature_, pubKey, payloadEra, payloadNonce, payloadTip, call);
    int requestId = blockchainUtils.getRequestId();
    sendExtrinsic(edata_, requestId);
}

Data Robonomics::createCall() {
    Data call;
    std::vector<uint8_t> callStr = hex2bytes(CALL_ENCODED);
    append(call, callStr);
    ESP_LOGI(TAG, "Call size: %zu", call.size());
    for (int k = 0; k < call.size(); k++) 
        printf("%02x", call[k]);
    printf("\r\n");
    return call;
}

Data Robonomics::createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block) {
    Data data_ = doPayload (call, era, nonce, tip, sv, tv, gen, block);
    ESP_LOGI(TAG, "Payload size: %zu", data_.size());
    for (int k = 0; k < data_.size(); k++) 
        printf("%02x", data_[k]);
    printf("\r\n");
    return data_;
}

Data Robonomics::createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]) {
    Data signature_ = doSign (data, privateKey, publicKey);
    ESP_LOGI(TAG, "Signature size: %zu", signature_.size());
    for (int k = 0; k < signature_.size(); k++) 
        printf("%02x", signature_[k]);
    printf("\r\n");
    return signature_;
}

Data Robonomics::createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call) {
    Data edata_ = doEncode (signature, pubKey, era, nonce, tip, call);
    ESP_LOGI(TAG, "Extrinsic %s: size %zu", "Datalog", edata_.size());
    for (int k = 0; k < edata_.size(); k++) 
        printf("%02x", edata_[k]);
    printf("\r\n");
    return edata_;
}

void Robonomics::sendExtrinsic(Data extrinsicData, int requestId) {
    String extrinsicMessage = fillParamsJs(extrinsicData, requestId);
    ESP_LOGI(TAG, "After to string: %s", extrinsicMessage.c_str());
    // Serial.print(extrinsicMessage);
    blockchainUtils.rpcRequest(extrinsicMessage);
    getExstrinsicResult();
}

void Robonomics::getExstrinsicResult() {
    blockchainUtils.setOnTextCallback([this](uint8_t *payload) {getExstrinsicResultCallback(payload);});
    while (!got_extrinsic_result) {
        blockchainUtils.websocketLoop();
    }
    got_extrinsic_result = false;
    // Serial.print("Extrinsic result: ");
    // Serial.println(extrinsicResult);
}

void Robonomics::getExstrinsicResultCallback(uint8_t *payload) {
    ESP_LOGI(TAG, "Extrinsic result: %s", (char *)payload);
    JSONVar received_message = JSON.parse((char *)payload);
    extrinsicResult = JSON.stringify(received_message["result"]);
    got_extrinsic_result = true;
}

NetworkConfig robonomicsConfig = {
    "kusama.rpc.robonomics.network",
    48,
    32,
    "631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc"
};

NetworkConfig varaConfig = {
    "testnet.vara.network",
    49,
    137,
    "525639f713f397dcf839bd022cd821f367ebcf179de7b9253531f8adbe5436d6" 
};