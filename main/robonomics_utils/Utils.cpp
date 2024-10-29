#include "Utils.h"

std::vector<uint8_t> hex2bytes (std::string hex) {
    std::vector<uint8_t> bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
      std::string byteString = hex.substr(i, 2);
      uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
      bytes.push_back(byte);
    }
    return bytes;
}

std::string swapEndian(std::string str) {
    std::string hex = str.c_str();
    std::string bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
      std::string s = hex.substr(i, 2);
      if ( !(strstr(s.c_str(),"0x")) && !(strstr(s.c_str(),"0X")) ) {
        std::string byteString = hex.substr(i, 2);
        bytes.insert(0,byteString);
      }
    }
    return bytes;
}

bool getTypeUrl(std::string url) {
    if (url.find("http://kusama.rpc.robonomics.network/rpc/") != std::string::npos)
      return true;
    return false;
}

std::string getBlockHash (bool is_remote) {
    if (is_remote)
      return "631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc";
    else 
      return "60ac92592dd51574a8bfa8094deae863f2e2ffe86889bef466d9768c625467ed";
}