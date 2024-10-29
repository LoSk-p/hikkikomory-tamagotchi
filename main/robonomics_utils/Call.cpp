#include "Call.h"

std::vector<uint8_t> callDatalogRecord (Data head, std::string str) {
    Data call;
    append(call, head);
    append(call, encodeCompact(str.length()));
    std::vector<uint8_t> rec(str.begin(), str.end());
    append(call, rec); 
    return call;
}

std::vector<uint8_t> callTransferBalance (Data head, std::string str, uint64_t fee ) {
    Data call;
    append(call, head); 
    append(call, 0); 
    std::vector<uint8_t> dst = hex2bytes (str.c_str()); // derived SS58KEY from SS58DST 
    append(call, dst); 
    append(call, encodeCompact(fee)); // value
    return call;
}

std::vector<uint8_t> callLaunch (Data head, std::string robot, std::string param) {
    Data call;
    append(call, head); 
    std::vector<uint8_t> dst = hex2bytes (robot.c_str()); // derived SS58KEY from SS58DST 
    append(call, dst);
    std::vector<uint8_t>  h256 = hex2bytes (param.c_str()); 
    append(call, h256); // add param as H256 data
    return call;
}

std::vector<uint8_t> callRws (Data head, RobonomicsPublicKey owner_key, Data param) {
    Data call;
    append(call, head); 
    std::vector<uint8_t> dst(owner_key.bytes, owner_key.bytes + PUBLIC_KEY_LENGTH);
    // std::vector<uint8_t> dst = hex2bytes (owner.c_str());
    append(call, dst);   // add owner pub key 
    append(call, param); // add nested call
    return call;
}