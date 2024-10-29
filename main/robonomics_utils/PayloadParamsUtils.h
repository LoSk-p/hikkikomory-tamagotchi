#pragma once

#include "JsonUtils.h"
#include "BlockchainUtils.h"

uint32_t getEra();
uint64_t getTip();
std::string getGenesisBlockHash();
uint32_t getSpecVersion(JSONVar runtimeInfo);
uint32_t getTransactionVersion(JSONVar runtimeInfo);

// Get Nonce

uint64_t getNonce(BlockchainUtils *blockchainUtils, char *ss58Address);
void getNonceCallback(uint8_t *payload);
void sendGetAccountNonceMessage(BlockchainUtils *blockchainUtils, char *ss58Address);

// Get Block Hash

std::string getBlockHash(BlockchainUtils *blockchainUtils);
void getBlockHashCallback(uint8_t *payload);
void sendGetBlockHashMessage(BlockchainUtils *blockchainUtils);

// Get Runtime Info

JSONVar getRuntimeInfo(BlockchainUtils *blockchainUtils);
JSONVar getRuntimeInfo(const char* parentBlockHash, BlockchainUtils *blockchainUtils);
void getRuntimeInfoCallback(uint8_t *payload);
void sendGetRuntimeInfoMessage(const char* parentBlockHash, BlockchainUtils *blockchainUtils);

// Get Chain Head

const char* getChainHead(BlockchainUtils *blockchainUtils);
void getChainHeadCallback(uint8_t *payload);
void sendGetChainHeadMessage(BlockchainUtils *blockchainUtils);

// Get Parent Block Hash

const char* getParentBlockHash(const char* chainHead, BlockchainUtils *blockchainUtils);
void getParentBlockHashCallback(uint8_t *payload);
void sendGetParentBlockHashMessage(const char* chainHead, BlockchainUtils *blockchainUtils);

// Get Metadata

void getChainMetadata(BlockchainUtils *blockchainUtils);
void getMetadataCallback(uint8_t *payload);
void sendGetMetadataMessage(BlockchainUtils *blockchainUtils);
