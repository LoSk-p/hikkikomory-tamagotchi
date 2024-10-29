#pragma once

#include <vector>
#include <string>
#include <Arduino_JSON.h>
#include "Defines.h"
#include "Utils.h"

typedef struct {
   std::string ghash;      // genesis hash
   std::string bhash;      // block_hash
   uint32_t version = 0;   // transaction version 
   uint64_t nonce;
   uint64_t tip;           // uint256_t tip;    // balances::TakeFees   
   uint32_t specVersion;   // Runtime spec version 
   uint32_t tx_version;
   uint32_t era;
} FromJson;


//  -- genesis_hash, nonce, spec_version, tip, era, tx_version
//  ["0x631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc",0,16,0,"Immortal",1]
//  -- nonce, spec_version, tip, era, tx_version
// ["0x00","0x01000000","0x00","0x0000000000000000","0x0100000000000000"]

FromJson parseJson (JSONVar val);
String getPayloadJs (std::string account, uint64_t id_cnt);
String fillParamsJs (std::vector<uint8_t> data, uint64_t id_cnt);
