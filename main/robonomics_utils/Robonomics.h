#pragma once

#include "Call.h"
#include "Extrinsic.h"
#include "BlockchainUtils.h"

#include <Arduino.h>
#include "PayloadParamsUtils.h"

struct NetworkConfig {
    const char* wsHost;
    size_t addressSize;
    uint16_t addressPrefix;
    std::string genesisBlockHash;
};

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

class Robonomics {
private:
    NetworkConfig *networkConfig;
    BlockchainUtils blockchainUtils;
    uint8_t publicKey_[KEYS_SIZE];
    uint8_t privateKey_[KEYS_SIZE];
    // char ss58Address[SS58_ADDRESS_SIZE + 1];
    std::vector<char> ss58Address;
    String extrinsicResult;
    bool got_extrinsic_result = false;
    void createAndSendExtrinsic(Data call);
    Data createCall();
    Data createPayload(Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block);
    Data createSignature(Data data, uint8_t privateKey[32], uint8_t publicKey[32]);
    Data createSignedExtrinsic(Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call);
    void sendExtrinsic(Data extrinsicData, int requestId);
    void getExstrinsicResult();
    void getExstrinsicResultCallback(uint8_t *payload);
public:
    Robonomics(const NetworkConfig& config);
    void setup();
    void disconnectWebsocket();
    void setPrivateKey(uint8_t *privateKey);
    void sendDatalogRecord(std::string data);
    void sendRWSDatalogRecord(std::string data, const char *owner_address);
    void sendCustomCall();
    const char* getSs58Address() const;
};
