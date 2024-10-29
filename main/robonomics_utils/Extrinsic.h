#pragma once

#include <vector>
#include <string>
#include "Encoder.h"
#include "Utils.h"
#include "Defines.h"

#include <Ed25519.h>

std::vector<uint8_t> doPayload (Data call, uint32_t era, uint64_t nonce, uint64_t tip, uint32_t sv, uint32_t tv, std::string gen, std::string block);
std::vector<uint8_t> doSign(Data data, uint8_t privateKey[32], uint8_t publicKey[32]);
std::vector<uint8_t> doEncode (Data signature, Data pubKey, uint32_t era, uint64_t nonce, uint64_t tip, Data call);
