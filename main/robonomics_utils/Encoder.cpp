#include "Encoder.h"

bool encodeRawAccount(TWSS58AddressType network, uint32_t specVersion) {
    if ((network == TWSS58AddressTypePolkadot && specVersion >= multiAddrSpecVersion) ||
        (network == TWSS58AddressTypeKusama && specVersion >= multiAddrSpecVersionKsm)) {
            return false;
        }
    return true;
}

void encode32LE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
}

// only up to uint64_t
Data encodeCompact(uint64_t value) {
    auto data = Data{};
    if (value < kMinUint16) {
        auto v = static_cast<uint8_t>(value) << 2u;
        data.push_back(static_cast<uint8_t>(v));
        return data;
    } else if (value < kMinUint32) {
        auto v = static_cast<uint16_t>(value) << 2u; // Initialize v with value
        v += 0x01; // set 0b01 flag
        auto minor_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(minor_byte);
        v >>= 8u;
        auto major_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(major_byte); 
        return data;
    } else if (value < kMinBigInteger) {
        uint32_t v = static_cast<uint32_t>(value) << 2u; // Initialize v with value
        v += 0x02; // set 0b10 flag
        encode32LE(v, data);
        return data;
    } else if (value < kMaxBigInteger) {
        auto length = sizeof(uint64_t);
        uint8_t header = (static_cast<uint8_t>(length) - 4) * 4;
        header += 0x03; // set 0b11 flag;
        data.push_back(header);
        auto v = value; // Initialize v with value
        for (size_t i = 0; i < length; ++i) {
            data.push_back(static_cast<uint8_t>(v & 0xff)); // push back least significant byte
            v >>= 8;
        }
        return data;
    } else { // too big
        return data;
    }
}

Data encodeAccountId(const Data& bytes, bool raw) {
    auto data = Data{};
    if (!raw) {
        // MultiAddress::AccountId
        // https://github.com/paritytech/substrate/blob/master/primitives/runtime/src/multiaddress.rs#L28
        append(data, 0x00);
        
        //std::string id = "12D3KooWBTxFpuyLJ7MpjxNTogMTa8nSLCQV7BAhQiFSkvuuiM8E";
        //std::vector<uint8_t> vec; 
        //vec.assign(id.begin(), id.end());
        //append(data, vec);
        
       }
    append(data, bytes);
    return data;
}

void encodeLengthPrefix(Data& data) {
    size_t len = data.size();
    auto prefix = encodeCompact(len);
    data.insert(data.begin(), prefix.begin(), prefix.end());
}


uint32_t swapU16 (uint32_t value) {
    uint32_t low  = value % 256;
    uint32_t high = value / 256;
    return low * 256 + high;
}

uint32_t swapU32 (uint32_t value) {
    uint32_t low  =  value & 0x000000ff;
    uint32_t mid1 = (value & 0x0000ff00 ) / 256;
    uint32_t mid2 = (value & 0x00ff0000 ) / (256 * 256);
    uint32_t high = (value & 0xff000000 ) / (256 * 256 * 256);
    return low * 256 * 256 * 256 +  mid1 * 256 * 256 +  mid2 * 256 + high;
}

// to test decoder ref https://github.com/qdrvm/scale-codec-cpp/blob/master/test/scale_compact_test.cpp
uint32_t decodeU32 (uint32_t value, bool swap) {
    uint32_t decoded = 0;
    uint32_t mode  = 0;
    uint32_t swapped = 0;

    if (value < 0x100) {
        mode = value % 4;
    }

    if (value > 0xFF && value <= 0xFFFF) {
      if (swap) 
         swapped = swapU16 (value);
      else
         swapped = value;
      mode = swapped % 4;
    }

    if (value > 0xFFFF && value <= 0xFFFFFFFF) {
      if (swap) 
         swapped = swapU32 (value);
      else
         swapped = value;
      mode = swapped % 4;
    }

    switch(mode) {
        case 0: 
            decoded = value / 4;
            break;
        case 1:
            if (swap)
              decoded =  swapU16 (value) / 4;
            else
              decoded =  value / 4;
            break;
        case 2: 
            if (swap)
              decoded =  swapU32 (value) / 4;
            else
              decoded =  value / 4;
            break;
        default: // TODO BigInt https://docs.substrate.io/reference/scale-codec/#fn-1
            break;
    }

    return decoded;
}
