#pragma once

#include <vector>
#include <string>
#include <cstring>

std::vector<uint8_t> hex2bytes (std::string hex);
std::string swapEndian(std::string str);
bool getTypeUrl(std::string url);
std::string getBlockHash (bool is_remote);
