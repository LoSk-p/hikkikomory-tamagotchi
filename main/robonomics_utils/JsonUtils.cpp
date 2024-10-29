#include "JsonUtils.h"
#include "esp_log.h"
#include <inttypes.h> 

// typedef struct {
//    std::string ghash;      // genesis hash
//    std::string bhash;      // block_hash
//    uint32_t version = 0;   // transaction version 
//    uint64_t nonce;
//    uint64_t tip;           // uint256_t tip;    // balances::TakeFees   
//    uint32_t specVersion;   // Runtime spec version 
//    uint32_t tx_version;
//    uint32_t era;
// } FromJson;


//  -- genesis_hash, nonce, spec_version, tip, era, tx_version
//  ["0x631ccc82a078481584041656af292834e1ae6daab61d2875b4dd0c14bb9b17bc",0,16,0,"Immortal",1]
//  -- nonce, spec_version, tip, era, tx_version
// ["0x00","0x01000000","0x00","0x0000000000000000","0x0100000000000000"]

static const char *TAG = "ROBONOMICS";

FromJson parseJson (JSONVar val) {
   // Serial.println(val);
   FromJson fj;

   std::string nonce_ =  (const char*) (val[0]);
   std::string specVersion_ = (const char*)(val[1]);
   std::string tip_ =  (const char*)(val[2]);
   std::string era_ =  (const char*)(val[3]); 
   std::string tx_version_ = (const char*) (val[4]);

   std::string nonceS  = swapEndian (nonce_);
   fj.nonce =  strtoul(nonceS.c_str(), NULL, 16);

   std::string tipS = swapEndian (tip_);
   fj.tip =  strtoul(tipS.c_str(), NULL, 16);

   std::string specVer = swapEndian (specVersion_);
   fj.specVersion =  strtoul(specVer.c_str(), NULL, 16);

   std::string txVer = swapEndian (tx_version_);
   fj.tx_version = strtoul(txVer.c_str(), NULL, 16); 
  
   std::string eraS = swapEndian (era_);
   fj.era = strtoul(eraS.c_str(), NULL, 16);

   fj.ghash = "";
   fj.bhash = "";

// #ifdef DEBUG_PRINT
   ESP_LOGI(TAG, "nonce encoded & swapped: 0x%llx", (unsigned long long)fj.nonce); // For uint64_t
   ESP_LOGI(TAG, "specVersion: %" PRIu32, fj.specVersion); // For uint32_t
   ESP_LOGI(TAG, "tip: %llu", (unsigned long long)fj.tip); // For uint64_t
   ESP_LOGI(TAG, "era: %" PRIu32, fj.era); // For uint32_t
   ESP_LOGI(TAG, "tx_version: %" PRIu32, fj.tx_version); // For uint32_t
   ESP_LOGI(TAG, "genesis hash: %s", fj.ghash.c_str()); // For std::string
   ESP_LOGI(TAG, "block hash: %s", fj.bhash.c_str()); // For std::string
// #endif

   return fj;
}

String getPayloadJs (std::string account, uint64_t id_cnt) {
   String jsonString;
   JSONVar params;
   params [0] = account.c_str();

   JSONVar get_payload;    
   get_payload["jsonrpc"] = "2.0";
   get_payload["id"] = (double) id_cnt; // to increment
   get_payload["method"] = "get_payload";
   get_payload["params"] = params;
   jsonString = JSON.stringify(get_payload); 
   return jsonString;
}

String fillParamsJs(std::vector<uint8_t> data, uint64_t id_cnt) {
    String jsonString;
    JSONVar params;
    std::string param0;
    
    param0.append("0x");
    char ch [3];   
    for (int i = 0; i < data.size();i++) {
        snprintf(ch, sizeof(ch), "%02x", data[i]);
        param0.append(ch);
    }
    params [0] = param0.c_str();
          
    JSONVar extrinsic;        
    extrinsic["jsonrpc"] = "2.0";
    extrinsic["id"] = (double) id_cnt;
    extrinsic["method"] = "author_submitExtrinsic";
    extrinsic["params"] = params;
    jsonString = JSON.stringify(extrinsic);
 
    return jsonString;
}
