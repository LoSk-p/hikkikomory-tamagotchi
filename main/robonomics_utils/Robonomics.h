#pragma once

#include "Call.h"
#include "Extrinsic.h"
#include "BlockchainUtils.h"

#include <Arduino.h>
#include "PayloadParamsUtils.h"

class Robonomics {
private:
    BlockchainUtils blockchainUtils;
    uint8_t publicKey_[KEYS_SIZE];
    uint8_t privateKey_[KEYS_SIZE];
    char ss58Address[SS58_ADDRESS_SIZE + 1];
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
    void setup();
    void disconnectWebsocket();
    void setPrivateKey(uint8_t *privateKey);
    void sendDatalogRecord(std::string data);
    void sendRWSDatalogRecord(std::string data, const char *owner_address);
    const char* getSs58Address() const;
};
