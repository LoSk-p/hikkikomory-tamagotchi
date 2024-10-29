#pragma once

#include "Data.h"

enum TWSS58AddressType {
    TWSS58AddressTypePolkadot = 0,
    TWSS58AddressTypeKusama = 2,
};

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t extrinsicFormat = 4;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint32_t multiAddrSpecVersion = 28;
static constexpr uint32_t multiAddrSpecVersionKsm = 2028;
static constexpr TWSS58AddressType network = TWSS58AddressTypeKusama;

static constexpr size_t kMinUint16 = (1ul << 6u);
static constexpr size_t kMinUint32 = (1ul << 14u);
static constexpr size_t kMinBigInteger = (1ul << 30u);
static constexpr uint64_t kMaxBigInteger = (1ul << 31u) * (1ul << 31u);

bool encodeRawAccount(TWSS58AddressType network, uint32_t specVersion);
void encode32LE(uint32_t val, std::vector<uint8_t>& data);
// only up to uint64_t
Data encodeCompact(uint64_t value);
Data encodeAccountId(const Data& bytes, bool raw);
void encodeLengthPrefix(Data& data);
uint32_t swapU16 (uint32_t value);
uint32_t swapU32 (uint32_t value);
// to test decoder ref https://github.com/qdrvm/scale-codec-cpp/blob/master/test/scale_compact_test.cpp
uint32_t decodeU32 (uint32_t value, bool swap);
