#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>

using byte = std::uint8_t;
using Data = std::vector<byte>;

inline void pad_left(Data& data, const uint32_t size) {
    data.insert(data.begin(), size - data.size(), 0);
}

inline Data data(const std::string& data) {
    return std::vector<byte>(data.begin(), data.end());
}

inline Data data(const byte* data, size_t size) {
    return std::vector<byte>(data, data + size);
}

inline void append(Data& data, std::string suffix, size_t offset) {
    std::vector<uint8_t> vec;
    vec.assign(suffix.begin() + offset, suffix.end());
    data.insert(data.end(), suffix.begin(), suffix.end());
}

inline void append(Data& data, const Data& suffix) {
    data.insert(data.end(), suffix.begin(), suffix.end());
}

inline void append(Data& data, const byte suffix) {
    data.push_back(suffix);
}

/// Return a part (subdata) from the requested start position and size of the input data.
Data subData(const Data& data, size_t startIndex, size_t length);

/// Return the tail part (subdata) from the requested start position of the input data.
Data subData(const Data& data, size_t startIndex);

/// Determines if a byte array has a specific prefix.
template <typename T>
inline bool has_prefix(const Data& data, T& prefix) {
    return std::equal(prefix.begin(), prefix.end(), data.begin(), data.begin() + std::min(data.size(), prefix.size()));
}
