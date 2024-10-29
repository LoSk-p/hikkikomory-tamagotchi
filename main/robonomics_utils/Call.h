#pragma once 

#include <vector>
#include <string>
#include "Encoder.h"
#include "Utils.h"
#include "address.h"

std::vector<uint8_t> callDatalogRecord (Data head, std::string str);
std::vector<uint8_t> callTransferBalance (Data head, std::string str, uint64_t fee );
std::vector<uint8_t> callLaunch (Data head, std::string robot, std::string param);
std::vector<uint8_t> callRws (Data head, RobonomicsPublicKey owner_key, Data param);
