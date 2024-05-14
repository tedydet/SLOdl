// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "aligned_malloc.h"

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 819678;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // One hour
        consensus.nPowTargetSpacing = 154; // 154 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x37;
        pchMessageStart[1] = 0xAF;
        pchMessageStart[2] = 0x82;
        pchMessageStart[3] = 0xE9;
        vAlertPubKey = ParseHex("8b397db20ec17e3f6bde0a589cf3fe076b95a7a15f61bd604a5a78f253a83ca1d2f8fde412fcab497cd60fe5dc8aa75edfeeff0bfc0827b7610a7fc1902718e6");
        nDefaultPort = 3377;
        nMinerThreads = 0;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database.
         *
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "The Economist 09/May/2024 The liberal international order is coming apart";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04f16b1014547958572b243adf297ad96286633cfc58b73fdd7576dad5fd590c487d3752c0710f5e9af2ed852d352d4419f2f326466e02937d95be1bf0e5034309") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1715286752; //date +%s
        //genesis.nBits    = 0x21000FFF;
        genesis.nBits    = 0x1e3fffff;
        genesis.nNonce   = 1820000;
        genesis.nStartLocation = 63450;
        genesis.nFinalCalculation = 3798797106;


        if(genesis.GetHash() != uint256S("00002faf3fb41b5f8f2ac542da50e7691bb07f905f6a30ff6279536958e8a7bb") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad = (char*)aligned_malloc(1<<30);
            while(true){
                int collisions=0;
		int tmpflag=0;
		thash = genesis.FindBestPatternHash(collisions,scratchpad,8,&tmpflag);
		printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    printf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            aligned_free(scratchpad);
            printf("block.nTime = %u \n", genesis.nTime);
            printf("block.nNonce = %u \n", genesis.nNonce);
            printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("block.nBits = %u \n", genesis.nBits);
            printf("block.nStartLocation = %u \n", genesis.nStartLocation);
            printf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }

        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == genesis.GetHash());
        //printf("genesis.GetHash() = %s \n", genesis.GetHash().ToString().c_str());
        //printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str());

        assert(consensus.hashGenesisBlock == uint256S("00002faf3fb41b5f8f2ac542da50e7691bb07f905f6a30ff6279536958e8a7bb"));
        assert(genesis.hashMerkleRoot == uint256S("93a83264288b7f4982d73b80112a00bfcd3e06e60dabf081d83d5180d2b548fc"));
	//printf("Mainnet genesis block verified. \n");

	vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("westcoast.hashbeans.com", "westcoast.hashbeans.com")); //West Coast

<<<<<<< HEAD
=======

        vSeeds.push_back(CDNSSeedData("westcoast.hodlcoin.com", "westcoast.hodlcoin.com")); //West Coast
        vSeeds.push_back(CDNSSeedData("eastcoast.hodlcoin.com", "eastcoast.hodlcoin.com"));//East Coast
        vSeeds.push_back(CDNSSeedData("europe.hodlcoin.com", "europe.hodlcoin.com")); //Europe
        vSeeds.push_back(CDNSSeedData("asia.hodlcoin.com", "asia.hodlcoin.com"));//Asia
        
	    vSeeds.push_back(CDNSSeedData("eu.hodlseed.amit.systems", "eu.hodlseed.amit.systems"));//Europe, Amit
	    vSeeds.push_back(CDNSSeedData("us.hodlseed.amit.systems", "us.hodlseed.amit.systems"));//US, Amit
        vSeeds.push_back(CDNSSeedData("hodlcoin.fi", "seed.hodlcoin.fi"));//Canada, DNS seed, Orava
        vSeeds.push_back(CDNSSeedData("hodlcoin.com", "seed.hodlcoin.com"));//For Future allocation

>>>>>>> 27d0cf5c (fix high cpu-usage of hasbeansd)
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,25+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("00002faf3fb41b5f8f2ac542da50e7691bb07f905f6a30ff6279536958e8a7bb")),
            1715286752, // * UNIX timestamp of last checkpoint block
            1,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1000     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.fPowAllowMinDifficultyBlocks = true;
        pchMessageStart[0] = 0x5C;
        pchMessageStart[1] = 0x71;
        pchMessageStart[2] = 0x1D;
        pchMessageStart[3] = 0xB4;
        vAlertPubKey = ParseHex("04f16b1014547958572b243adf297ad96286633cfc58b73fdd7576dad5fd590c487d3752c0710f5e9af2ed852d352d4419f2f326466e02937d95be1bf0e5034309");
        nDefaultPort = 33770;
        nMinerThreads = 0;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1715349276;
        genesis.nNonce = 2570000;

        genesis.nStartLocation = 211580;
        genesis.nFinalCalculation = 3195160961;

<<<<<<< HEAD
        if(false && genesis.GetHash() != uint256S("0000163cdef2334bcd36c265d87c4a870db5f7135ce46a8809210bd665239bf1") ){
=======

        if(genesis.GetHash() != uint256S("0000163cdef2334bcd36c265d87c4a870db5f7135ce46a8809210bd665239bf1") ){
>>>>>>> 27d0cf5c (fix high cpu-usage of hasbeansd)
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad=(char*)aligned_malloc(1<<30);
            while(true){
                int collisions=0;
		int tmpflag=0;
	   	thash = genesis.FindBestPatternHash(collisions,scratchpad,8,&tmpflag);
                LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            aligned_free(scratchpad);
            printf("Mining Testnet Genesisblock \n");
            printf("block.nTime = %u \n", genesis.nTime);
            printf("block.nNonce = %u \n", genesis.nNonce);
            printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("block.nBits = %u \n", genesis.nBits);
            printf("block.nStartLocation = %u \n", genesis.nStartLocation);
            printf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }
        consensus.hashGenesisBlock = genesis.GetHash();

	//assert(consensus.hashGenesisBlock == genesis.GetHash())
        //printf("genesis.GetHash() = %s \n", genesis.GetHash().ToString().c_str());
        //printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str());

	assert(consensus.hashGenesisBlock == uint256S("0000163cdef2334bcd36c265d87c4a870db5f7135ce46a8809210bd665239bf1"));
	//printf("Testnet genesis block verified. \n");

        //assert(consensus.hashGenesisBlock == uint256S("0x000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("westcoast.hashbeans.com", "westcoast.hashbeans.com")); //West Coast

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,43);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,7);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,43+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();


        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0000163cdef2334bcd36c265d87c4a870db5f7135ce46a8809210bd665239bf1")),
            1715286752,
            0,
            500
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0x9F;
        pchMessageStart[1] = 0x63;
        pchMessageStart[2] = 0x2A;
        pchMessageStart[3] = 0xD8;
        nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;
        nDefaultPort = 18444;
        //assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        nPruneAfterHeight = 1000;


        if(false && genesis.GetHash() != uint256S("0x01") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad=(char*)aligned_malloc(1<<30);
            while(true){
                int collisions=0;
                int tmpflag=0;
                thash = genesis.FindBestPatternHash(collisions,scratchpad,8,&tmpflag);
                LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            aligned_free(scratchpad);
            printf("Mining Testnet Genesisblock \n");
            printf("block.nTime = %u \n", genesis.nTime);
            printf("block.nNonce = %u \n", genesis.nNonce);
            printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("block.nBits = %u \n", genesis.nBits);
            printf("block.nStartLocation = %u \n", genesis.nStartLocation);
            printf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }

	//assert(consensus.hashGenesisBlock == genesis.GetHash());
        //printf("genesis.GetHash() = %s \n", genesis.GetHash().ToString().c_str());
        //printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str());

	consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("001bd2a5fa3104b924919b01ecfb5e6edb3723010951b40f42a5b491fa93db07"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("001bd2a5fa3104b924919b01ecfb5e6edb3723010951b40f42a5b491fa93db07")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}