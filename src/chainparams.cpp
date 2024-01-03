// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2018 The Dash Core developers 
// Copyright (c) 2018-2018 The Vishai Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
#include <arith_uint256.h>



static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // TODO_EDIT: perlu edit public key
    const char* pszTimestamp = "Decentralised Service AI, VISHAI; 20 December 2023";
    // TODO_EDIT: perlu edit public key
    const CScript genesisOutputScript = CScript() << ParseHex("047c710e7564a5453701704289f25aa069b0da96e3c02dd735af20e7ac02433b82247624f70cb82c9f2550dfdeb36ae1ffefe06422d6dd49ac34c95cecd64b8c26") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


static void MiningGenesisBlock(uint32_t nTime, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    printf("mining... \n");

	CBlock block = CreateGenesisBlock(nTime, 0, nBits, nVersion, genesisReward);
    arith_uint256 cc;
	arith_uint256 bnTarget;
	bnTarget.SetCompact(block.nBits);
    
	for(uint32_t nNonce = 0; nNonce < UINT32_MAX; nNonce++){
		block.nNonce = nNonce;
		uint256 hash = block.GetHash();

		cc = UintToArith256(hash);
        // printf("\n n is %d\n", nNonce);
        if ((nNonce % 500) == 0) {
            printf("\nhash %s", block.ToString().c_str());
        }
        

		if(cc <= bnTarget) {
			printf("\ngenesis is %s\n", block.ToString().c_str());
			printf("\npow is %s\n", hash.GetHex().c_str());
			printf("\ngenesisNonce is %d\n", nNonce);
			std::cout << "Genesis Merkle " << block.hashMerkleRoot.GetHex() << std::endl;
			return;
		}
	}

	    assert(false);
}


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 262800;
        consensus.nMasternodePaymentsStartBlock = 14400; // masternode dibayar setelah
        consensus.nMasternodePaymentsIncreaseBlock = 10081;
        consensus.nMasternodePaymentsIncreasePeriod = 262800;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 99999999;
        consensus.nBudgetPaymentsCycleBlocks = 21600;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 99999999;
        consensus.nSuperblockCycle = 21600;
        consensus.nGovernanceMinQuorum = 10; // Active Masternode
        consensus.nGovernanceFilterElements = 1000000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f");
        consensus.powLimit = uint256S("00ffffffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 86400; // Vishai: 1 day
        consensus.nPowTargetSpacing = 60; // Block Tercipta / Minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 0;
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1508025600; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1539561600; // Oct 15th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // TODO_EDIT: perlu edit
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */

        pchMessageStart[0] = 0x76; //v
        pchMessageStart[1] = 0x69; //i
        pchMessageStart[2] = 0x73; //s
	    pchMessageStart[3] = 0x68; //h

        // TODO_EDIT: perlu edit
	    vAlertPubKey = ParseHex("047c710e7564a5453701704289f25aa069b0da96e3c02dd735af20e7ac02433b82247624f70cb82c9f2550dfdeb36ae1ffefe06422d6dd49ac34c95cecd64b8c26");
        nDefaultPort = 8083;
        
        nMaxTipAge = 21600;
        nDelayGetHeadersTime = 86400;
        nPruneAfterHeight = 100000;

        // MiningGenesisBlock(1703055901, 0x20001fff, 1, 20 * COIN);
        // TODO_EDIT: perlu edit----->epoch unix---------->
        genesis = CreateGenesisBlock(1703055901, 1473, 0x20001fff, 1, 20 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // TODO_EDIT: perlu edit
        assert(consensus.hashGenesisBlock == uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f"));
        assert(genesis.hashMerkleRoot == uint256S("0x5e4b2f27fd5bf8300c29e000ceda6b8e81e99254aad1acbd41d0f25c996dd0cb"));

        vSeeds.clear();

        // change aku
        // TODO_EDIT: perlu edit
        vSeeds.push_back(CDNSSeedData("38.242.244.19", "38.242.244.19"));
        vSeeds.push_back(CDNSSeedData("173.212.215.76", "173.212.215.76"));
        vSeeds.push_back(CDNSSeedData("173.212.215.76","173.212.215.76"));
        vSeeds.push_back(CDNSSeedData("38.242.244.19","38.242.244.19"));
        vSeeds.push_back(CDNSSeedData("38.242.244.19","38.242.244.19"));
        vSeeds.push_back(CDNSSeedData("173.212.215.76","173.212.215.76"));
        vSeeds.push_back(CDNSSeedData("45.32.176.249","45.32.176.249"));
        vSeeds.push_back(CDNSSeedData("173.212.215.76","173.212.215.76"));
        vSeeds.push_back(CDNSSeedData("38.242.244.19","38.242.244.19"));
        vSeeds.push_back(CDNSSeedData("1.14.104.202","1.14.104.202"));
        vSeeds.push_back(CDNSSeedData("1.163.71.32","1.163.71.32"));
        vSeeds.push_back(CDNSSeedData("1.169.155.231","1.169.155.231"));
        vSeeds.push_back(CDNSSeedData("1.170.217.199","1.170.217.199"));
        vSeeds.push_back(CDNSSeedData("1.188.100.50","1.188.100.50"));
        vSeeds.push_back(CDNSSeedData("1.198.209.97","1.198.209.97"));
        vSeeds.push_back(CDNSSeedData("1.203.117.250","1.203.117.250"));
        vSeeds.push_back(CDNSSeedData("1.204.250.229","1.204.250.229"));
        vSeeds.push_back(CDNSSeedData("1.55.95.81","1.55.95.81"));
        vSeeds.push_back(CDNSSeedData("101.44.80.236","101.44.80.236"));
        // change aku end

       


        // Vishai addresses start with 's'
        // TODO_EDIT: perlu edit-------------------------------------->
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,131);
        // Vishai script addresses start with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        // Vishai private keys start with 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,75);
        // Vishai BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Vishai BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Vishai BIP44 coin type is '5'
        nExtCoinType = 5;

        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04825df8a146091cd7527a55a648cf1c23efb3cc25bed2029eaa17a606918be747b9afa29e08da4df312f8cfc9823fb3707635e8744000390574b40f28c0d509a9";


        // checkpointData = (CCheckpointData) {
        //     boost::assign::map_list_of
        //     (     0, uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f")),
        //         1703055901, //1519569150 * UNIX timestamp of last checkpoint block
        //         0,          // * total number of transactions between genesis and last checkpoint
        //                     //   (the tx=... number in the SetBestChain debug.log lines)
        //         10          // * estimated number of transactions per day after checkpoint
        // };

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (     0, uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f"))
			    (13886, uint256S("0000000c31385a8bf5ec491729f3129c2b72f84a3e278df8b785bf9c96bfde1b")),
                1704258017, 
                25066,
                2000       
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 50; // mastermode dibayar setelah block
        consensus.nMasternodePaymentsIncreaseBlock = 100;
        consensus.nMasternodePaymentsIncreasePeriod = 100;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 4200;
        consensus.nSuperblockCycle = 24; 
        consensus.nGovernanceMinQuorum = 2; // masternode
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Vishai: 1 day
        consensus.nPowTargetSpacing = 1.0 * 60; // almost 1.0 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 0;
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f");

        pchMessageStart[0] = 0x76; //v
        pchMessageStart[1] = 0x69; //i
        pchMessageStart[2] = 0x73; //s
	    pchMessageStart[3] = 0x68; //h
        vAlertPubKey = ParseHex("047c710e7564a5453701704289f25aa069b0da96e3c02dd735af20e7ac02433b82247624f70cb82c9f2550dfdeb36ae1ffefe06422d6dd49ac34c95cecd64b8c26");
        nDefaultPort = 33445;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        // MiningGenesisBlock(1703055901, 890380, 0x20001fff, 1, 20 * COIN);
        genesis = CreateGenesisBlock(1703055901, 1473, 0x20001fff, 1, 20 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f"));
        assert(genesis.hashMerkleRoot == uint256S("0x5e4b2f27fd5bf8300c29e000ceda6b8e81e99254aad1acbd41d0f25c996dd0cb"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // change aku
        vSeeds.push_back(CDNSSeedData("172.16.238.11", "172.16.238.11"));
        vSeeds.push_back(CDNSSeedData("172.16.238.12", "172.16.238.12"));
        // vSeeds.push_back(CDNSSeedData("vishai.info", "test01.vishai.info"));

        // change aku end

		// vSeeds.push_back(CDNSSeedData("vishai.info", "test02.vishai.info"));

        // Testnet Vishai addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        // Testnet Vishai script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet Vishai BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Vishai BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet Vishai BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04ee3e3feb1e780b98327ece9579ff24991b6b6294f4bf53435e5feeb40ba166cbf71dc2b074c2703fd757b54befbe363b18bf56ec5a51be08ea6e912301afd1f2";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (     0, uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f")),
                1703055901, //1519569150 * UNIX timestamp of last checkpoint block
                0,          // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
                10          // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1; // minimal masternode aktif
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Vishai: 1 day
        consensus.nPowTargetSpacing = 2.0 * 60; // almost 2.0 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 0;
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x76; //v
        pchMessageStart[1] = 0x69; //i
        pchMessageStart[2] = 0x73; //s
	    pchMessageStart[3] = 0x68; //h
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 8334;
        nPruneAfterHeight = 1000;
        
        genesis = CreateGenesisBlock(1703055901, 1473, 0x20001fff, 1, 20 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // printf("%s\n", consensus.hashGenesisBlock.ToString().c_str());
        // printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
        
        assert(consensus.hashGenesisBlock == uint256S("0x00056ccc9f59bc9e189989e73582e9a51a5b7190a2f6529ff1c6c93f7555a35f"));
        assert(genesis.hashMerkleRoot == uint256S("0x5e4b2f27fd5bf8300c29e000ceda6b8e81e99254aad1acbd41d0f25c996dd0cb"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x7f6c3cd530b97a2f646ff8311e5ff79a5ebaec359dacea13d67a7169b3f4e897")),
            0,
            0,
            0
        };
        // Regtest Vishai addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 67);
        // Regtest Vishai script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 239);
        // Regtest Vishai BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Vishai BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest Vishai BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
