/*#############################################################

					   		ConfigParser.h
					
##############################################################
 Name: ConfigParser.h
 Creat: gary
 Desc:配置文件分析模块
***************************************************************/
#pragma once
#ifndef  __CONFIGPARSER_H__
#define __CONFIGPARSER_H__

#include "ParserBase.h"
#include "ConfigFile.h"
//#include "AnalyseDef.h"
#include "WTConfig.h"
#include "DutBase.h"
#include "ITesterCommon.h"

enum{
	eumParserCheckMode_None			=0x00,
	eumParserCheckMode_Max				=0x01,
	eumParserCheckMode_Min				=0x02,
	eumParserCheckMode_Enum			=0x08,
	eumParserCheckMode_StringHex		=0x10,
	eumParserCheckMode_StringLength	=0x20,
};
enum{
	eumParserTesterPort_RF1 = 1,
	eumParserTesterPort_RF2,
	eumParserTesterPort_RF3,
	eumParserTesterPort_RF4,
	eumParserTesterPort_RF5,
	eumParserTesterPort_RF6,
	eumParserTesterPort_RF7,
	eumParserTesterPort_RF8,
};
enum{
	eumTesterConnectMode_Normal=1,		//单独模式，只有1台PC连接测试仪
	eumTesterConnectMode_Queue,		//队列模式，当前一台PC断开连接后，下一台PC才会连接测试仪
};

//解释器的基本功能
class WTCONFIG_API cParserParm
{
	public:
		cParserParm(string strFileName, int core_id=0);
		cParserParm(string strFileName,string strCommentToken, int core_id=0);
		cParserParm(string strFileName, int cfgType, int core_id);
		int iGetCoreId(){return m_coreID;};
		//eumCheckMode的枚举在cFetchState中,当其值为eumFetchCheck_Min/Max时,输入参数dMin和dMax只有对应的一个生效
		//Fetch int
		bool bFetchIntAndCheck(string strName,int *pRetValue,int eumParserCheckMode=eumParserCheckMode_None,double dMin=0,double dMax=0);

		//Fetch Double
		bool bFetchDoubleAndCheck(string strName,double *pRetValue,int eumParserCheckMode=eumParserCheckMode_None,double dMin=0,double dMax=0);

		//Fetch String 
		//pFetchState中的eumCheckMode,Max.Min.针对的是string的长度
		bool bFetchStringAndCheck(string strName,string *pRetString,int eumParserCheckMode=eumParserCheckMode_None,int nMinLength=0,int nMaxLength=0);

		//Fetch Int Array
		bool bFetchIntArrayAndCheck(string strName,int *pRetBuf,int nSz,int eumParserCheckMode=eumParserCheckMode_None,double dMin=0,double dMax=0);

		//Fetch Int Array
		bool bFetchIntArrayAndCheck(string strName,int *pRetBuf,int nSz, int *actualCount, int eumParserCheckMode=eumParserCheckMode_None,double dMin=0,double dMax=0);

		//Fetch Double Array
		bool bFetchDoubleArrayAndCheck(string strName,double *pRetBuf,int nSz,int eumParserCheckMode=eumParserCheckMode_None,double dMin=0,double dMax=0);

		//Fetch bool
		bool bFetchBoolean(string strParmName,bool *pRetValue);


		void vDisableErrorPrint(){m_bIsNeedPrintErr = false;};
		void vEnableErrorPrint(){m_bIsNeedPrintErr = true;};

		string strGetError(){return m_strError+"\n";};

		cConfigFile *pGetParser(){return &m_imConfigFile;};

		cContentProcessing *pGetContentProcessing(){return &m_contentProcessing;};
	//protected:
		//用于解释以下格式
		/*							54M	48M	36M	24M	18M	12M	9M	6M	11M	5.5M	2M	1M
			WT_TX_EVM_LEGACY_LIMIT	-26	-22	-19	-16	-13	-10	-8	-5	-10	-10	-10	-10

									65M	58.5M	52M	39M	26M	19.5M	13M	6.5M
			WT_TX_EVM_HT20_1S_LIMIT	-26	-25	-22	-19	-16	-13	-10	-5

									130M	117M	104M	78M	52M	39M	26M	13M
			WT_TX_EVM_HT20_2S_LIMIT	-26	-25	-22	-19	-16	-13	-10	-5

									135M	121.5M	108M	81M	54M	40.5M	27M	13.5M
			WT_TX_EVM_HT40_1S_LIMIT	-26	-25	-22	-19	-16	-13	-10	-5

									270M	243M	216M	162M	108M	81M	54M	27M
			WT_TX_EVM_HT40_2S_LIMIT	-26	-25	-22	-19	-16	-13	-10	-5*/
		//bool bGetMcsParm(string strParmName,int nDemode,int nMcs,double *pRetValue,cFetchState *pFetchState=NULL);
		bool bGetMcsParm(string strParmName,int nDemode,int nMcs,double *pRetValue,int eumCheckMode,double dMin,double dMax);

		static const int m_n11nMcsParmCountInOneLine = 8;
		static const int m_nAC20McsParmCountInOneLine = 14;
		static const int m_nAC40McsParmCountInOneLine = 14;
		static const int m_nAC80McsParmCountInOneLine = 14;
		static const int m_nAXMcsParmCountInOneLine = 14; //AX支持MCS12,MCS13
		static const int m_nAX80McsParmCountInOneLineOld = 12;
	protected:
		string strGetErrString_MissToken(string strFile,string strName);
		string strGetErrString_MissContent(string strFile,string strName,int nLineInFile);
		string strGetErrString_IntFormatErr(string strFile,string strName,int nLineInFile);
		string strGetErrString_OutOfValidRange_Int(string strFile,string strName,int nLineInFile,int iMin,int iMax);
		string strGetErrString_OutOfValidRange_Double(string strFile,string strName,int nLineInFile,double iMin,double iMax);
		string strGetErrString_OutOfValidRange_Enum(string strFile,string strName,int nLineInFile,double dMin,double dMax);
		string strGetErrString_StringLength(string strFile,string strName,int nLineInFile,int nMinLength,int nMaxLength);
		string strGetErrString_HexFormat(string strFile,string strName,int nLineInFile);
		string strGetErrString_MissElement(string strFile,string strName,int nLineInFile, int nMissElementCount);

		cConfigFile m_imConfigFile;
		bool m_bIsNeedPrintErr;
		int m_coreID;
		string m_strError;
		int m_cfgFileType;

		cContentProcessing m_contentProcessing;
};

// wifi limit 解释器
class WTCONFIG_API cParserWifiLimit
	: public cParserParm
{
public:
	cParserWifiLimit(string configFile="./WT_SETUP/WT_WIFI_LIMIT.txt",int core_id=0)
		:cParserParm(configFile, eumWifiLmtEncrypt ,core_id){};
	virtual ~cParserWifiLimit(){};
public:

	bool bGetTargetPowDataRateDiff( int nDemode,int nMcs,double *pRetValue,int nFreqGroup= eumFreqGroupInvalid);
	bool bGetTargetPow(int nFreqGroup,int nDemode,double *pRetValue);

	bool bGetFreqCalErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_FREQ_CAL_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000);
	};
	bool bGetTargetFreqErr(double *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchDoubleAndCheck(	"WT_FREQ_CAL_TARGET",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-1000,1000);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetPowCalTolerance_Up(double *pRetValue,int iFreqGroup)
	{
		string strName = "";
		if(eumFreqGroup2_4G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_UP_2G";
		}
		else if(eumFreqGroup5G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_UP_5G";
		}
		else if(eumFreqGroup6G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_UP_6G";
		}
		else
		{
			return false;
		}

		return bFetchDoubleAndCheck(strName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	};
	bool bGetPowCalTolerance_Low(double *pRetValue,int iFreqGroup)
	{
		string strName = "";
		if(eumFreqGroup2_4G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_LOW_2G";
		}
		else if(eumFreqGroup5G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_LOW_5G";
		}
		else if(eumFreqGroup6G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_TOLERANCE_LOW_6G";
		}
		else
		{
			return false;
		}

		return bFetchDoubleAndCheck(strName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	};

	bool bGetFreqVerifyErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_TX_FREQ_ERR_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000);
	}
	bool bGetMaskErrThreshold(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_SPA_FAIL_POINTS_LIMIT",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	bool bGetOBWErrThreshold(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_SPECTRUM_OBW_99_LIMIT",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}

	bool bGetCarrierLeakageThreshold(int nFreqGroup, int nDemode, double *pRetValue)  
	{
		string strParmName="";

		if (nFreqGroup == eumFreqGroup2_4G)
		{
			switch(nDemode)
			{
			case eumDemod_11b:
				{
					strParmName = "WT_CARRIERLEAKAGE_11B_2_4G";
					break;
				}
			case eumDemod_11ag:
				{
					strParmName = "WT_CARRIERLEAKAGE_OFDM_2_4G";
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT20_2_4G";
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT40_2_4G";
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC20_2_4G";
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC40_2_4G";
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC80_2_4G";
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC160_2_4G";
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC8080_2_4G";
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX20_2_4G";
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX40_2_4G";
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX80_2_4G";
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX160_2_4G";
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX8080_2_4G";
					break;
				}
			}			
		}
		else if (nFreqGroup == eumFreqGroup5G)
		{
			switch(nDemode)
			{
			case eumDemod_11ag:
				{
					strParmName = "WT_CARRIERLEAKAGE_OFDM_5G";
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT20_5G";
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT40_5G";
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC20_5G";
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC40_5G";
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC80_5G";
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC160_5G";
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC8080_5G";
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX20_5G";
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX40_5G";
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX80_5G";
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX160_5G";
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX8080_5G";
					break;
				}
			}
		}

		else
		{
			switch(nDemode)
			{
			case eumDemod_11ag:
				{
					strParmName = "WT_CARRIERLEAKAGE_OFDM_6G";
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT20_6G";
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_HT40_6G";
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC20_6G";
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC40_6G";
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC80_6G";
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC160_6G";
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AC8080_6G";
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX20_6G";
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX40_6G";
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX80_6G";
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX160_6G";
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_CARRIERLEAKAGE_AX8080_6G";
					break;
				}
			}
		}
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-99,0);
	}

	//Add for B30 添加Symbol Clock Error指标
	bool bGetSymbolClkErrLimitUpOrLow(int nFreqGroup, int nDemode, double *pRetValue, string strUpOrLow)  
	{
		string strParmName="";

		if (nFreqGroup == eumFreqGroup2_4G)
		{
			switch(nDemode)
			{
			case eumDemod_11b:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_11B_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ag:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_11G_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT40_2_4G_TOLERANCE_" + strUpOrLow;;
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC40_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC80_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC160_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC8080_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX40_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX80_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX160_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX8080_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			}			
		}
		else if (nFreqGroup == eumFreqGroup5G)
		{
			switch(nDemode)
			{
			case eumDemod_11ag:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_11A_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT20_5G_TOLERANCE_" + strUpOrLow;;
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC20_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC80_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC160_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC8080_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX20_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX80_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX160_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX8080_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			}
		}

		else
		{
			switch(nDemode)
			{
			case eumDemod_11ag:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_11A_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT20_6G_TOLERANCE_" + strUpOrLow;;
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_HT40_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC20_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC40_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC80_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC160_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AC8080_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX20_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX40_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX80_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX160_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_SYMBOL_CLOCK_ERR_AX8080_6G_TOLERANCE_" + strUpOrLow;
					break;
				}
			}
		}
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-1000,1000);
	}

	//Add for B30 T添加Symbol Clock Error指标
	bool bGetOBW99LimitUpOrLow(int nFreqGroup, int nDemode, double *pRetValue, string strUpOrLow)  
	{
		string strParmName="";

		if (nFreqGroup == eumFreqGroup2_4G)
		{
			switch(nDemode)
			{
			case eumDemod_11b:
				{
					strParmName = "WT_OBW_11B_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ag:
				{
					strParmName = "WT_OBW_11G_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_OBW_HT20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_OBW_HT40_2_4G_TOLERANCE_" + strUpOrLow;;
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_OBW_AC20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_OBW_AC40_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_OBW_AC80_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_OBW_AC160_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_OBW_AC8080_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_OBW_AX20_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_OBW_AX40_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_OBW_AX80_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_OBW_AX160_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_OBW_AX8080_2_4G_TOLERANCE_" + strUpOrLow;
					break;
				}
			}			
		}
		else if (nFreqGroup == eumFreqGroup5G)
		{
			switch(nDemode)
			{
			case eumDemod_11ag:
				{
					strParmName = "WT_OBW_11A_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11n_20M:
				{
					strParmName = "WT_OBW_HT20_5G_TOLERANCE_" + strUpOrLow;;
					break;
				}
			case eumDemod_11n_40M:
				{
					strParmName = "WT_OBW_HT40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_20M:
				{
					strParmName = "WT_OBW_AC20_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_40M:
				{
					strParmName = "WT_OBW_AC40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M:
				{
					strParmName = "WT_OBW_AC80_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_160M:
				{
					strParmName = "WT_OBW_AC160_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ac_80M_80M:
				{
					strParmName = "WT_OBW_AC8080_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_20M:
				{
					strParmName = "WT_OBW_AX20_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_40M:
				{
					strParmName = "WT_OBW_AX40_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80M:
				{
					strParmName = "WT_OBW_AX80_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_160M:
				{
					strParmName = "WT_OBW_AX160_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			case eumDemod_11ax_80_80M:
				{
					strParmName = "WT_OBW_AX8080_5G_TOLERANCE_" + strUpOrLow;
					break;
				}
			}
		}

		else
		{
			switch(nDemode)
			{
				case eumDemod_11ag:
					{
						strParmName = "WT_OBW_11A_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11n_20M:
					{
						strParmName = "WT_OBW_HT20_6G_TOLERANCE_" + strUpOrLow;;
						break;
					}
				case eumDemod_11n_40M:
					{
						strParmName = "WT_OBW_HT40_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ac_20M:
					{
						strParmName = "WT_OBW_AC20_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ac_40M:
					{
						strParmName = "WT_OBW_AC40_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ac_80M:
					{
						strParmName = "WT_OBW_AC80_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ac_160M:
					{
						strParmName = "WT_OBW_AC160_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ac_80M_80M:
					{
						strParmName = "WT_OBW_AC8080_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ax_20M:
					{
						strParmName = "WT_OBW_AX20_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ax_40M:
					{
						strParmName = "WT_OBW_AX40_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ax_80M:
					{
						strParmName = "WT_OBW_AX80_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ax_160M:
					{
						strParmName = "WT_OBW_AX160_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
				case eumDemod_11ax_80_80M:
					{
						strParmName = "WT_OBW_AX8080_6G_TOLERANCE_" + strUpOrLow;
						break;
					}
			}
		}
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-1000,1000);
	}

	bool bGetPERMinPassLimit(int nDemode,double *pRetValue)
	{
		string strParmName="";
		if (nDemode == eumDemod_11b)
		{
			strParmName = "PER_MIN_PASS_LIMIT_DSSS";
		}
		else
		{
			strParmName = "PER_MIN_PASS_LIMIT_OFDM";
		}
		
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	// Alex 20190605, added for retrieving RSSI spec.
	bool bGetRSSILimit(double *pRetValue)
	{
		string strParmName="RSSI_SPEC";
		
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}

	bool bGetPowVerifyErrRange_Up(int nDemode,int nMcs,double *pRetValue)
	{
		return bGetPowVerifyErrRange(1,nDemode,nMcs,pRetValue);
	};
	bool bGetPowVerifyErrRange_Low(int nDemode,int nMcs,double *pRetValue)
	{
		return bGetPowVerifyErrRange(0,nDemode,nMcs,pRetValue);
	};

	//增加分开2.4G，5G上下限标准接口
	bool bGetPowVerifyErrRangeWithBand_Up(int iBand, int nDemode,int nMcs,double *pRetValue)
	{
		return bGetPowVerifyErrRange(1,iBand, nDemode,nMcs,pRetValue);
	};
	bool bGetPowVerifyErrRangeWithBand_Low(int iBand,int nDemode,int nMcs,double *pRetValue)
	{
		return bGetPowVerifyErrRange(0,iBand, nDemode,nMcs,pRetValue);
	};

	bool bGetEvmThreshold(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				strParmName = "WT_TX_EVM_LEGACY_LIMIT";
				break;
			}
		case eumDemod_11n_20M:
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_HT20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_HT20_2S_LIMIT";
				}
				break;
			}
		case eumDemod_11n_40M:
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_HT40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_HT40_2S_LIMIT";
				}
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_AC20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_AC20_2S_LIMIT";
				}
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_AC40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_AC40_2S_LIMIT";
				}
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_AC80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_AC80_2S_LIMIT";
				}
				break;
			}
		}
		return bGetMcsParm(	strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-45,0);
	};

	bool bGetEvmThreshold(int iBand,int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_TX_EVM_LEGACY_LIMIT";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_TX_EVM_5G_11A_LIMIT";
				}
				else if (eumFreqGroup6G == iBand)
				{
					strParmName = "WT_TX_EVM_6G_11A_LIMIT";
				}
				break;
			}
		case eumDemod_11n_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_HT20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_HT20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_HT20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11n_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_HT40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_HT40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_HT40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AC20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AC20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AC20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AC40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AC40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AC40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AC80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AC80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{	
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AC80_2S_LIMIT";
					}
				}
				break;
			}

		case eumDemod_11ac_160M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AC160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AC160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AC160_2S_LIMIT";
					}
				}
				break;
			}

		case eumDemod_11ac_80M_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AC8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AC8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AC8080_2S_LIMIT";
					}
				}
				break;
			}

		case eumDemod_11ax_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AX20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AX20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AX20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AX40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AX40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AX40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AX80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AX80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AX80_2S_LIMIT";
					}
				}
				break;
			}

		case eumDemod_11ax_160M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AX160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AX160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AX160_2S_LIMIT";
					}
				}
				break;
			}

		case eumDemod_11ax_80_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_2_4G_AX8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_2_4G_AX8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_5G_AX8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_5G_AX8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_6G_AX8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_6G_AX8080_2S_LIMIT";
					}
				}
				break;
			}
		}
		return bGetMcsParm(	strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-45,0);
	};

	//Add for B30 Evm Percent 指标
	bool bGetEvmPercentThreshold(int iBand,int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_TX_EVM_PERCENT_LEGACY_LIMIT";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_TX_EVM_PERCENT_5G_11A_LIMIT";
				}
				else if (eumFreqGroup6G == iBand)
				{
					strParmName = "WT_TX_EVM_PERCENT_6G_11A_LIMIT";
				}
				break;
			}
		case eumDemod_11n_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_HT20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_HT20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_HT20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_HT20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11n_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_HT40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_HT40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_n11nMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_HT40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_HT40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC20McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC40McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC80_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_160M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC160_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ac_80M_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AC8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AC8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAC80McsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AC8080_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_20M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX20_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX20_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX20_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_40M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX40_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX40_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX40_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX80_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX80_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX80_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_160M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX160_2S_LIMIT";
					}
				}
				else if (eumFreqGroup6G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX160_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_6G_AX160_2S_LIMIT";
					}
				}
				break;
			}
		case eumDemod_11ax_80_80M:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_2_4G_AX8080_2S_LIMIT";
					}
				}
				else if (eumFreqGroup5G == iBand)
				{
					if (nMcs < m_nAXMcsParmCountInOneLine)
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX8080_1S_LIMIT";
					}
					else
					{
						strParmName = "WT_TX_EVM_PERCENT_5G_AX8080_2S_LIMIT";
					}
				}

				break;
			}
		}
		return bGetMcsParm(	strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	};

	bool bGetEvmPeakPercentThreshold(int iBand, int nDemode, int nMcs, double *pRetValue)
	{
		string strParmName = "";
		switch (nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				strParmName = "WT_TX_EVM_PK_PERCENT_LEGACY_LIMIT";
			}
			else if (eumFreqGroup5G == iBand)
			{
				strParmName = "WT_TX_EVM_PK_PERCENT_5G_11A_LIMIT";
			}
			else if (eumFreqGroup6G == iBand)
			{
				strParmName = "WT_TX_EVM_PK_PERCENT_6G_11A_LIMIT";
			}
			break;
		}
		case eumDemod_11n_20M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_HT20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_HT20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_HT20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_HT20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_HT20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_HT20_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11n_40M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_HT40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_HT40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_HT40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_HT40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_HT40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_HT40_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ac_20M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC20_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ac_40M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC40_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ac_80M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC80_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC80_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC80_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ac_160M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC160_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC160_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC160_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ac_80M_80M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC8080_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AC8080_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC8080_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AC8080_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC8080_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AC8080_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ax_20M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX20_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX20_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX20_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ax_40M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX40_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX40_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX40_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ax_80M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX80_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX80_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX80_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX80_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ax_160M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX160_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX160_2S_LIMIT";
				}
			}
			else if (eumFreqGroup6G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX160_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_6G_AX160_2S_LIMIT";
				}
			}
			break;
		}
		case eumDemod_11ax_80_80M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX8080_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_2_4G_AX8080_2S_LIMIT";
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX8080_1S_LIMIT";
				}
				else
				{
					strParmName = "WT_TX_EVM_PK_PERCENT_5G_AX8080_2S_LIMIT";
				}
			}

			break;
		}
		}
		return bGetMcsParm(strParmName, nDemode, nMcs, pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			0, 100);
	};

	bool bGetRssiPow(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				strParmName = "WT_PER_SENS_MIN_LEGACY";
				break;
			}
		case eumDemod_11n_20M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				strParmName = "WT_PER_SENS_MIN_HT20_1S";
				break;
			}
		case eumDemod_11n_40M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				strParmName = "WT_PER_SENS_MIN_HT40_1S";
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (nMcs >= m_nAC20McsParmCountInOneLine)
				{
					nMcs -= m_nAC20McsParmCountInOneLine;
				}
				strParmName = "WT_PER_SENS_MIN_AC20_1S";
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (nMcs >= m_nAC40McsParmCountInOneLine)
				{
					nMcs -= m_nAC40McsParmCountInOneLine;
				}
				strParmName = "WT_PER_SENS_MIN_AC40_1S";
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}
				strParmName = "WT_PER_SENS_MIN_AC80_1S";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};
	//增加分开2.4G，5G PER功率接口
	bool bGetRssiPow(int iBand,int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_LEGACY";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_11A";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_11A";
				}
				break;
			}
		case eumDemod_11n_20M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_HT20_1S";
				}
				else if (eumFreqGroup5G == iBand) 
				{
					strParmName = "WT_PER_SENS_MIN_5G_HT20_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_HT20_1S";
				}
				break;
			}
		case eumDemod_11n_40M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_HT40_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_HT40_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_HT40_1S";
				}
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (nMcs >= m_nAC20McsParmCountInOneLine)
				{
					nMcs -= m_nAC20McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AC20_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AC20_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AC20_1S";
				}
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (nMcs >= m_nAC40McsParmCountInOneLine)
				{
					nMcs -= m_nAC40McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AC40_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AC40_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AC40_1S";
				}
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}
				
				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AC80_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AC80_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AC80_1S";
				}
				break;
			}
		case eumDemod_11ac_160M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AC160_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AC160_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AC160_1S";
				}
				break;
			}
		case eumDemod_11ac_80M_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AC8080_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AC8080_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AC8080_1S";
				}
				break;
			}
		case eumDemod_11ax_20M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AX20_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AX20_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AX20_1S";
				}
				break;
			}
		case eumDemod_11ax_40M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AX40_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AX40_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AX40_1S";
				}
				break;
			}
		case eumDemod_11ax_80M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AX80_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AX80_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AX80_1S";
				}
				break;
			}
		case eumDemod_11ax_160M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AX160_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AX160_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AX160_1S";
				}
				break;
			}
		case eumDemod_11ax_80_80M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_2_4G_AX8080_1S";
				}
				else if (eumFreqGroup5G == iBand)
				{
					strParmName = "WT_PER_SENS_MIN_5G_AX8080_1S";
				}
				else
				{
					strParmName = "WT_PER_SENS_MIN_6G_AX8080_1S";
				}
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};

protected:
		bool bGetPowVerifyErrRange(bool b1Up0Low,int nDemode,int nMcs,double *pRetValue);
		//增加2.4G,5G上下限分开读取接口
		bool bGetPowVerifyErrRange(bool b1Up0Low,int iBand, int nDemode,int nMcs,double *pRetValue);
};

// BT limit 解释器
class WTCONFIG_API cParserBtLimit
	: public cParserParm
{
public:
	cParserBtLimit(string configFile="./WT_SETUP/WT_BT_LIMIT.txt",int core_id=0)
		:cParserParm(configFile, eumBtLmtEncrypt, core_id){};
	virtual ~cParserBtLimit(){};
public:

	bool bGetBTFreqCalErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_BT_FREQ_CAL_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000);
	};

	bool bGetBTTargetFreqErr(double *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchDoubleAndCheck(	"WT_BT_FREQ_CAL_TARGET",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-1000,1000);
		vEnableErrorPrint();
		return bRslt;
	};

	//蓝牙功率校准功率的上下限
	bool bGetBTPowCalTolerance_Up(double *pRetValue,int iFreqGroup)
	{
		string strName = "";

		strName = "WT_POWER_CAL_TOLERANCE_UP_BT";

		return bFetchDoubleAndCheck(strName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	};
	bool bGetBTPowCalTolerance_Low(double *pRetValue,int iFreqGroup)
	{
		string strName = "";
		strName = "WT_POWER_CAL_TOLERANCE_LOW_BT";

		return bFetchDoubleAndCheck(strName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	};

	bool bGetPERMinPassLimit(int nDemode,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		
		case eumDemod_BT:
			{
				strParmName = "WT_PER_MIN_PASS_LIMIT_BT";
				break;
			}
		}
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}

	bool bGetBERRssiPow(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_BT:
			{
				//nMcs  = 0;
				strParmName = "WT_BER_SENS_MIN_BT";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};
	bool bGetRssiPow(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_BT:
			{
				//nMcs  = 0;
				strParmName = "WT_PER_SENS_MIN_BT";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};

	//BT 参数
    bool bGetBTFreqDriftLimit(double *pRetValue,int iMcs)
    {
		string strName = "WT_BT_FREQ_DRIFT_LIMIT_RATE";
		if (enumBT_LE1M == iMcs || enumBT_LE2M == iMcs)
		{
		strName = "WT_BT_FREQ_DRIFT_LIMIT_BLE";
		}
		if (iMcs == enumBT_DH1)
		{
		strName = "WT_BT_FREQ_DRIFT_LIMIT_1DH1";
		}
		if (iMcs == enumBT_DH3)
		{
		strName = "WT_BT_FREQ_DRIFT_LIMIT_1DH3";
		}
		if (iMcs == enumBT_DH5)
		{
		strName = "WT_BT_FREQ_DRIFT_LIMIT_1DH5";
		}
        return bFetchDoubleAndCheck(strName,pRetValue,eumParserCheckMode_Min,0);
    }
	//此指标为BR专属
	bool bGetBTBRFreqDriftRateLimit(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_BR_FREQ_DRIFT_RATE_LIMIT",pRetValue,eumParserCheckMode_Min,0);
	}
    bool bGetBTIcftLimit(double *pRetValue)
    {
        return bFetchDoubleAndCheck("WT_BT_ICFT_LIMIT",pRetValue,eumParserCheckMode_Min,0);
    }
    bool bGetBTDeltaF1AvgLimit(double *pLimitLow, double *pLimitUp)
    {
        return bFetchDoubleAndCheck("WT_BT_DELTA_F1_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
               &&  bFetchDoubleAndCheck("WT_BT_DELTA_F1_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
    }
	bool bGetBTLEDeltaF1AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_DELTA_F1_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_DELTA_F1_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBLE125KDeltaF1MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F1_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F1_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBTLE2MDeltaF1AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F1_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F1_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBTLE_125KDeltaF1AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F1_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F1_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
    bool bGetBTDeltaF2AvgLimit(double *pLimitLow, double *pLimitUp)
    {
        return bFetchDoubleAndCheck("WT_BT_DELTA_F2_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
            &&  bFetchDoubleAndCheck("WT_BT_DELTA_F2_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
    }
	bool bGetBTLEDeltaF2AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_DELTA_F2_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_DELTA_F2_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBTLE2MDeltaF2AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F2_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F2_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBTLE500KDeltaF2AvgLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F2_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F2_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
    bool bGetBTDeltaF2F1Ratio(double *pLimitLow, double *pLimitUp)
    {
        return bFetchDoubleAndCheck("WT_BT_DELTA_F2_F1_RATIO_UP",pLimitUp,eumParserCheckMode_Min,0)
            &&  bFetchDoubleAndCheck("WT_BT_DELTA_F2_F1_RATIO_LOW",pLimitLow,eumParserCheckMode_Min,0);
    }
	bool bGetBLE500KDeltaF0F3Limit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F0F3_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F0F3_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBLE125KDeltaF0F3Limit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F0F3_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F0F3_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBLE500KDeltaF0Fn3Limit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F0FN3_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F0FN3_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBLE125KDeltaF0Fn3Limit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F0FN3_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_DELTA_F0FN3_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
    bool bGetBTDeltaF2MaxLimit(double *pLimitLow, double *pLimitUp)
    {
        return bFetchDoubleAndCheck("WT_BT_DELTA_F2_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
            &&  bFetchDoubleAndCheck("WT_BT_DELTA_F2_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
    }
	bool bGetBTLEDeltaF2MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_DELTA_F2_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_DELTA_F2_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBTLE2MDeltaF2MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F2_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F2_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	bool bGetBLE500KDeltaF2MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F2_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_DELTA_F2_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,0);
	}
	////
	bool bGetBTLE_FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	
	bool bGetBTLE2M_FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	bool bGetBLE500K_FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	bool bGetBLE125K_FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}

	bool bGetBTLE_F0FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_F0Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_F0Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	
	bool bGetBTLE2M_F0FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_F0Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_F0Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	bool bGetBLE125K_F0FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_F0Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_125K_F0Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	bool bGetBLE500K_F0FnMaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_F0Fn_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BLE_500K_F0Fn_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}

	bool bGetBTLE_F1F0DeltaLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_DELTA_F1F0_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_DELTA_F1F0_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}

	bool bGetBTLE2M_F1F0DeltaLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F1F0_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_DELTA_F1F0_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}

	bool bGetBTLE_FnFn5MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE_FnFn5_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE_FnFn5_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}

	bool bGetBTLE2M_FnFn5MaxLimit(double *pLimitLow, double *pLimitUp)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_FnFn5_MAX_TOLERANCE_UP",pLimitUp,eumParserCheckMode_Min,0)
			&&  bFetchDoubleAndCheck("WT_BTLE2M_FnFn5_MAX_TOLERANCE_LOW",pLimitLow,eumParserCheckMode_Min,-500);
	}
	//////
	bool bGetBTPowVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBTPowVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBTPowVerifyErrRange_2M_3M_OFFSET(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_2M_3M_OFFSET",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}
	//Add for B30 POWER PEAK
	bool bGetBTPowPeakVerifyErrRange_2M_3M_OFFSET(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_PEAK_2M_3M_OFFSET",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBTBLEPowVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE_TX_POWER_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBTBLEPowVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE_TX_POWER_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	//Add for BLE 2M
	bool bGetBTBLE2MPowVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_TX_POWER_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBTBLE2MPowVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_TX_POWER_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBTBLE2MPowPeakVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_TX_POWER_PEAK_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBTBLE2MPowPeakVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE2M_TX_POWER_PEAK_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBLE125K_PowVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_TX_POWER_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBLE125K_PowVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_TX_POWER_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBLE500K_PowVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_TX_POWER_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBLE500K_PowVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_TX_POWER_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

	bool bGetBLE125K_PowPeakVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_TX_POWER_PEAK_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBLE_125KPowPeakVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_125K_TX_POWER_PEAK_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}
	bool bGetBLE_500KPowPeakVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_TX_POWER_PEAK_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBLE_500KPowPeakVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BLE_500K_TX_POWER_PEAK_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}
	//End

	//Add for B30 PowerPeak 
	bool bGetBTBLEPowPeakVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE_TX_POWER_PEAK_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
	}
	bool bGetBTBLEPowPeakVerifyErrRange_Low(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BTLE_TX_POWER_PEAK_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
	}

    bool bGetBTPeakPowVerifyErrRange_Up(double *pRetValue)
    {
        return bFetchDoubleAndCheck("WT_BT_TX_POWER_PEAK_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50);
    }
    bool bGetBTPeakPowVerifyErrRange_Low(double *pRetValue)
    {
        return bFetchDoubleAndCheck("WT_BT_TX_POWER_PEAK_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-50,50);
    }
	bool bGetBTPowDiffVerifyErrRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_DIFF_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-200,200);
	}
	bool bGetBTPowDiffVerifyErrRange_Low(double  *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_POWER_DIFF_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-200,200);
	}
	bool bGetBTDevmAvgLimit(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_DEVM_AVG_LIMIT",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,100);
	}
	bool bGetBTDevmPeakLimit(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_DEVM_PEAK_LIMIT",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,100);
	}
	bool bGetBTMaskErrThreshold(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_BT_SPA_FAIL_POINTS_LIMIT",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	bool bGetBTEdrOmegaIVerifyErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_BT_TX_EDR_OMEGA_I_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	bool bGetBTEdrOmegaOVerifyErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_BT_TX_EDR_OMEGA_O_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	bool bGetBTEdrOmegaIOVerifyErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_BT_TX_EDR_OMEGA_IO_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
	bool bGetBTDevmPctLimit(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_TX_DEVM_PASS_PCT_LIMIT",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,100);
	}
	bool bGetBTPerMinPassLimit(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BER_MIN_PASS_LIMIT_BT",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,100);
	}

	//Add for B30 Bandwidth20DB
	bool bGetBTBandwidth20DBRange_Up(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_BANDWIDTH_20DB_TOLERANCE_UP",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-100000,100000);
	}
	//Add for B30 Bandwidth20DB
	bool bGetBTBandwidth20DBRange_Low(double  *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_BT_BANDWIDTH_20DB_TOLERANCE_LOW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-100000,100000);
	}
	//BT
	bool bGetBTRxBitNum(int nMcs,int *pRxBitNum)
	{
		string strKeyword;
		//if (nMcs == 0)	//1M
		//{
		//	strKeyword = "WT_BT_BER_BIT_NUM_1M";
		//}
		//if (nMcs == 1)
		//{
		//	strKeyword = "WT_BT_BER_BIT_NUM_2M";
		//}
		//if (nMcs == 2)
		//{
		//	strKeyword = "WT_BT_BER_BIT_NUM_3M";
		//}
		if (enumBT_LE1M == nMcs)	//1M
		{
			strKeyword = "WT_BT_BER_BIT_NUM_LE";
		}
		else if (enumBT_LE2M == nMcs)	//1M
		{
			strKeyword = "WT_BT_BER_BIT_NUM_LE_2M";
		}
		else if (enumBT_DH1 == nMcs || enumBT_DH3 == nMcs || enumBT_DH5 == nMcs)	//1M
		{
			strKeyword = "WT_BT_BER_BIT_NUM_1M";
		}
		else if (enumBT_2DH1 == nMcs || enumBT_2DH3 == nMcs || enumBT_2DH5 == nMcs)
		{
			strKeyword = "WT_BT_BER_BIT_NUM_2M";
		}
		else if (enumBT_3DH1 == nMcs || enumBT_3DH3 == nMcs || enumBT_3DH5 == nMcs)
		{
			strKeyword = "WT_BT_BER_BIT_NUM_3M";
		}

		return bFetchIntAndCheck(strKeyword,pRxBitNum,
			eumParserCheckMode_Min|eumParserCheckMode_Max,1,10000000);
	}
	bool bGetBTRxTestTimeout(int *pSec)
	{
		return bFetchIntAndCheck("WT_BT_BER_TIMEOUT",pSec,
			eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}

	bool bGetBTLE_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BTLE_ACP_UP",pLimitUp,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999)
			&&  bFetchDoubleArrayAndCheck("WT_BTLE_ACP_LOW",pLimitLow,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999);
	}
	bool bGetBTLE2M_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BTLE2M_ACP_UP",pLimitUp,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999)
			&&  bFetchDoubleArrayAndCheck("WT_BTLE2M_ACP_LOW",pLimitLow,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999);
	}

	bool bGetBTLE125K_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BTLE125K_ACP_UP",pLimitUp,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999)
			&&  bFetchDoubleArrayAndCheck("WT_BTLE125K_ACP_LOW",pLimitLow,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999);
	}

	bool bGetBTLE500K_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BTLE500K_ACP_UP",pLimitUp,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999)
			&&  bFetchDoubleArrayAndCheck("WT_BTLE500K_ACP_LOW",pLimitLow,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999);
	}

	bool bGetBT_BR_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BT_BR_ACP_UP",pLimitUp,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999)
			&&  bFetchDoubleArrayAndCheck("WT_BT_BR_ACP_LOW",pLimitLow,size, eumParserCheckMode_Min|eumParserCheckMode_Max,-999,999);
	}

	bool bGetBT_EDR_ACPLimit(double *pLimitLow, double *pLimitUp, int size)
	{
		return bFetchDoubleArrayAndCheck("WT_BTEDR_ACP_UP", pLimitUp, size, eumParserCheckMode_Min | eumParserCheckMode_Max, -999, 999)
			&& bFetchDoubleArrayAndCheck("WT_BTEDR_ACP_LOW", pLimitLow, size, eumParserCheckMode_Min | eumParserCheckMode_Max, -999, 999);
	}
};

// ZIGBEE limit 解释器
class WTCONFIG_API cParserZigbeeLimit
	: public cParserParm
{
public:
	cParserZigbeeLimit(string configFile="./WT_SETUP/WT_ZIGBEE_LIMIT.txt",int core_id=0)
		:cParserParm(configFile, eumZgbLmtEncrypt, core_id){};
	virtual ~cParserZigbeeLimit(){};
public:
	bool bGetPERMinPassLimit(int nDemode,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
			case eumDemod_Zigbee:
			{
				strParmName = "WT_PER_MIN_PASS_LIMIT_ZIGBEE";
				break;
			}
		}
		return bFetchDoubleAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}

	bool bGetZigbeeEVMThreshold(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_TX_ZIGBEE_EVM_LIMIT",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}

	bool bGetRssiPow(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_Zigbee:
			{
				nMcs  = 0;
				strParmName = "WT_PER_SENS_MIN_ZIGBEE";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};
	//增加分开2.4G，5G PER功率接口
	bool bGetRssiPow(int iBand,int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_Zigbee:
			{
				nMcs = 0;
				strParmName = "WT_PER_SENS_MIN_ZIGBEE";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};

	bool bGetFreqVerifyErrRange(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_TX_FREQ_ERR_TOLERANCE",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000);
	}
	bool bGetMaskErrThreshold(double *pRetValue)
	{
		return bFetchDoubleAndCheck(	"WT_SPA_FAIL_POINTS_LIMIT",pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,100);
	}
};

//dut 解释器
class WTCONFIG_API cParserDut
	:public cParserParm
{
public:
	cParserDut(string configFile="./WT_SETUP/WT_DUT_MIMO.txt",int core_id=0)
		:cParserParm(configFile, eumDutMimoEncrypt, core_id){};
	virtual ~cParserDut(){};

public:
	/*enum TESTRE_TYPE{
		eum_Unknow,
		eum_WT160,
		eum_WT200,
		eum_WT208,
		eum_WT208C,
	};*/
	//连接参数
	bool bGetConnType(string *type)
	{
		string strTemp = "";
		if (bFetchStringAndCheck("WT_DUT_CONN_TYPE", &strTemp))
		{
			transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::toupper);
			*type = strTemp;
			return true;
		}
		return false;
	}

	bool bGetIP(string *pIP, int id,int esterType = TESTER_UNKNOW)
	{
		string strParmName_sub = "WT_DUT_IP_ADDRESS";
		string strParmName_dut = "WT_DUT_IP_ADDRESS";
		char tmp[128] = {0};
		char tmp1[128] = {0};
		
		if (esterType >= TESTER_WT208)
		{
			sprintf(tmp,"WT_SUB_IP_ADDRESS_%d", id);
			strParmName_sub = string(tmp);
		}

		if (bFetchStringAndCheck(strParmName_sub,pIP) == false)
		{
			if (bFetchStringAndCheck(strParmName_dut,pIP) == false)
			{
				//*pIP = "192.168.1.1";
				*pIP = "";	//设置为空，利于出错调试
				return false;
			}
		}
		return true;
	}

	bool bGetTesterShellCmd(string *pCmd, int id)
	{
		string strParmName = "WT_TESTER_SHELL_CMD";
		char tmp[128] = {0};
		sprintf(tmp,"WT_TESTER_SHELL_CMD_%d", id);
		strParmName = string(tmp);
		if (bFetchStringAndCheck(strParmName,pCmd) == false)
		{
			return false;
		}
		return true;
	}

	bool bGet208NetWorkIP(string *pIP, int id=-1)
	{
		string strParmName = "WT_DUT_IP_ADDRESS";
		char tmp[128] = {0};
		if (id == 0)
		{
			//
		}
		else if (id > 0)
		{
			sprintf(tmp,"WT_SUB_IP_ADDRESS_%d", id);
			strParmName = string(tmp);
		}
		else if (iGetCoreId() > 0)
		{
			sprintf(tmp,"WT_SUB_IP_ADDRESS_%d", iGetCoreId());
			strParmName = string(tmp);
		}

		if (bFetchStringAndCheck(strParmName,pIP) == false)
		{
			return false;
		}
		return true;
	}

	bool bGet208CNetWorkIP(string *pIP, int id=-1)
	{
		string strParmName = "WT_DUT_IP_ADDRESS";
		char tmp[128] = {0};
		if (id == 0)
		{
			//
		}
		else if (id > 0)
		{
			sprintf(tmp,"WT_SUB_IP_ADDRESS_%d", id);
			strParmName = string(tmp);
		}
		else if (iGetCoreId() > 0)
		{
			sprintf(tmp,"WT_SUB_IP_ADDRESS_%d", iGetCoreId());
			strParmName = string(tmp);
		}

		if (bFetchStringAndCheck(strParmName,pIP) == false)
		{
			return false;
		}
		return true;
	}

	//bool bGetPort(int* pPort)
	//{
	//	//MARK:当无法获取DUT的端口号时，默认使用Telnet的默认端口：23，不报错
	//	vDisableErrorPrint();
	//	bool bRslt = bFetchIntAndCheck("WT_DUT_CONN_PORT",
	//		pPort,
	//		eumParserCheckMode_Min,
	//		0);
	//	vEnableErrorPrint();
	//	return bRslt;
	//}

	bool bGetPort(int* pPort, int id)
	{
		//MARK:当无法获取DUT的端口号时，默认使用Telnet的默认端口：23，不报错
		vDisableErrorPrint();

		string strParmName;
		bool ret = true;
		strParmName = cStringBase::strPrintf("WT_DUT_CONN_PORT_%d", id);

		if (bFetchIntAndCheck(strParmName,pPort) == false)
		{
			if (bFetchIntAndCheck("WT_DUT_CONN_PORT",pPort) == false)
			{
				*pPort = 1;
				ret = false;
			}
		}
		vEnableErrorPrint();
		return ret;
	}

	bool bGetTelnetUsrName(string *pUsrName)
	{
		//MARK:当无法获取UserName字符串时，视为无需UserName，不报错
		vDisableErrorPrint();
		string strParmName = "WT_DUT_LOGIN_NAME";
		//char tmp[128] = {0};
		//if (id > 0)
		//{
		//	sprintf(tmp,"WT_DUT%d_LOGIN_NAME",id);
		//	strParmName = string(tmp);
		//}
		if (bFetchStringAndCheck(strParmName,pUsrName) == false)
		{
			*pUsrName = "";
		}
		vEnableErrorPrint();
		return true;
	}

	bool bGetTelnetPassword(string *pPassword)
	{
		//MARK:当无法获取Password字符串时，视为无需密码，不报错
		vDisableErrorPrint();
		string strParmName = "WT_DUT_LOGIN_PWD";
	/*	char tmp[128] = {0};
		if (id > 0)
		{
			sprintf(tmp,"WT_DUT%d_LOGIN_PWD",id);
			strParmName = string(tmp);
		}*/
		if (bFetchStringAndCheck(strParmName,pPassword) == false)
		{
			*pPassword = "";
		}
		vEnableErrorPrint();
		return true;
	}

	bool bGetDut_AS_TftpServer_Addr(string *ipaddr, int id = -1)
	{
		string strParmName = "WT_DUT_AS_TFTP_SERVER_ADDR";
		char tmp[128] = {0};
		if (id == 0)
		{
			//
		}
		else if (id > 0)
		{
			sprintf(tmp,"WT_DUT_AS_TFTP_SERVER_ADDR_%d", id);
			strParmName = string(tmp);
		}
		else if (iGetCoreId() > 0)
		{
			sprintf(tmp,"WT_DUT_AS_TFTP_SERVER_ADDR_%d", iGetCoreId());
			strParmName = string(tmp);
		}

		if (bFetchStringAndCheck(strParmName,ipaddr) == false)
		{
			return false;
		}
		return true;
	}

	bool bGetDut_As_TftpClient_Addr(string *ipaddr, int id = -1)
	{
		string strParmName = "WT_DUT_AS_TFTP_CLIENT_ADDR";

		char tmp[128] = {0};
		if (id == 0)
		{
			//
		}
		else if (id > 0)
		{
			sprintf(tmp,"WT_DUT_AS_TFTP_CLIENT_ADDR_%d", id);
			strParmName = string(tmp);
		}
		else if (iGetCoreId() > 0)
		{
			sprintf(tmp,"WT_DUT_AS_TFTP_CLIENT_ADDR_%d", iGetCoreId());
			strParmName = string(tmp);
		}

		if (bFetchStringAndCheck(strParmName,ipaddr) == false)
		{
			return false;
		}
		return true;
	}

	bool bGetPC_AS_TftpServer_Addr(string *ipaddr)
	{
		string strParmName = "WT_PC_AS_TFTP_SERVER_ADDR";
		if (bFetchStringAndCheck(strParmName,ipaddr) == false)
		{
			return false;
		}
		return true;
	}

	bool bGetDut_AS_TftpServer_Addr_208(string *ipaddr,int id =-1)
	{
		string strParmName = "WT_DUT_AS_TFTP_SERVER_ADDR";

		if (bFetchStringAndCheck(strParmName,ipaddr) == false)
		{
			return false;
		}
		return true;
	}
	bool bGetDut_As_TftpClient_Addr_208(string *ipaddr,int id =-1)
	{
		string strParmName = "WT_DUT_AS_TFTP_CLIENT_ADDR";

		if (bFetchStringAndCheck(strParmName,ipaddr) == false)
		{
			return false;
		}
		return true;
	}


	bool bGetCommunicateTimeout(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_COMMUNICATE_TIMEOUT",
			pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			1,
			60);
	}

	bool bGetDutLib(string *name)
	{
		return bFetchStringAndCheck("WT_DUT_LIB_NAME", name);
	}

	bool bGetDutCtrlLib(string *name)
	{
		return bFetchStringAndCheck("WT_DUT_CTRL_LIB_NAME", name);
	}

	bool bGetMapFile(string *name)
	{
		return bFetchStringAndCheck("WT_MAP_FILE", name);
	}

	bool bGetDutBandFlag(int band, bool *flag)
	{
		int nEnable;
		string token;
		if (band == eumFreqGroup2_4G)
			token = "WT_DUT_BAND_2G";
		else if(band == eumFreqGroup5G)
			token = "WT_DUT_BAND_5G";
		else if(band == eumFreqGroup6G)
			token = "WT_DUT_BAND_6G";
		else
			return false;

		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck(token,
			&nEnable,
			eumParserCheckMode_Min,
			0,1000);
		vEnableErrorPrint();

		if (bRslt)
			*flag = (nEnable != 0);
		else
			*flag = false;

		return bRslt;
	}

	bool bGetDutAntNum(int band, int *pVal)
	{
		string token;
		if (band == eumFreqGroup2_4G)
			token = "WT_DUT_ANT_2G";
		else if(band == eumFreqGroup5G)
			token = "WT_DUT_ANT_5G";
		else if(band == eumFreqGroup6G)
			token = "WT_DUT_ANT_6G";
		else
			return false;

		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck(token,
			pVal,
			eumParserCheckMode_Min,
			0,1000);
		vEnableErrorPrint();
		return bRslt;
	}

	bool bGetDutIsTSSITest(int band, int *pVal)
	{
		string token;
		if (band == eumFreqGroup2_4G)
			token = "WT_DUT_IS_TSSI_TEST_2G";
		else if(band == eumFreqGroup5G)
			token = "WT_DUT_IS_TSSI_TEST_5G";
		else if(band == eumFreqGroup6G)
			token = "WT_DUT_IS_TSSI_TEST_6G";
		else
		{
			*pVal = 0;
			return false;
		}
		bool bRslt = bFetchIntAndCheck(token,
			pVal,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
 			0,1);
		if (bRslt == false)
		{
			*pVal = 0;
		}
		return bRslt;
	}

	bool bGetDutIfName(int band, string *pname)
	{
		*pname = "";
		string token;
		if (band == eumFreqGroup2_4G)
			token = "WT_DUT_IF_2G";
		else if(band == eumFreqGroup5G)
			token = "WT_DUT_IF_5G";
		else if(band == eumFreqGroup6G)
			token = "WT_DUT_IF_6G";
		else
			return false;

		return bFetchStringAndCheck(token, pname);
	}

	bool bGetDutFlashOfsset(int band ,int *pRetValue)
	{
		string strParmName = "";
		if (band == eumFreqGroup2_4G)
			strParmName = "WT_DUT_FLASH_OFFSET_2_4G";
		else if(band == eumFreqGroup5G)
			strParmName = "WT_DUT_FLASH_OFFSET_5G";
		else if(band == eumFreqGroup6G)
			strParmName = "WT_DUT_FLASH_OFFSET_6G";
		else
			return false;
		return bFetchIntAndCheck(	strParmName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000);
	}

	bool bGetDutHas11AC(bool *flag)
	{
		int nEnable;
		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck("WT_DUT_HAS_11AC",
			&nEnable,
			eumParserCheckMode_Min,
			0,1000);
		vEnableErrorPrint();

		if (bRslt)
			*flag = (nEnable != 0);
		else
			*flag = false;

		return bRslt;
	}

	bool bGetDutIs5GFreqRegSeparate(bool *flag)
	{
		int nEnable;
		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck("WT_DUT_5GFREQRER_SEPARATE",
			&nEnable,
			eumParserCheckMode_Min,
			0);
		vEnableErrorPrint();

		if (bRslt)
			*flag = (nEnable != 0);
		else
			*flag = false;

		return true;
	}

	bool bGetFreqRegRange(int band, int *pRegBuf)
	{
		string name = (band == eumFreqGroup2_4G)
			? "WT_DUT_FREQ_REG_RANGE" : "WT_DUT_FREQ_REG_RANGE_5G";

		return bFetchIntArrayAndCheck(name, pRegBuf, 2, eumParserCheckMode_Min|eumParserCheckMode_Max, 0,1000);
	}
	bool bGetPowerRegRange(int band, int *pRegBuf)
	{
		string name = (band == eumFreqGroup2_4G)
			? "WT_DUT_POWER_REG_RANGE" : "WT_DUT_POWER_REG_RANGE_5G";

		if(band == eumFreqGroup6G)
			name = "WT_DUT_POWER_REG_RANGE_6G";
			
		return bFetchIntArrayAndCheck(name, pRegBuf, 2, eumParserCheckMode_Min|eumParserCheckMode_Max, 0,1000);
	}
	bool bGetTSSIOffsetPowerRegRange(int band, int *pRegBuf)
	{
		string name = (band == eumFreqGroup2_4G)
			? "WT_DUT_POWER_REG_RANGE" : "WT_DUT_POWER_REG_RANGE_5G";

		if(band == eumFreqGroup6G)
			name = "WT_DUT_POWER_REG_RANGE_6G";
			
		return bFetchIntArrayAndCheck(name, pRegBuf, 2, eumParserCheckMode_Min|eumParserCheckMode_Max, -1000,1000);
	}
	bool bGetFreqRegDefault(int *pRetValue, int nMin, int nMax)
	{
		string strParmName = "WT_FREQ_CAL_DEFAULT_REG";
		bool bRslt = bFetchIntAndCheck( strParmName, pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			(double)nMin, (double)nMax);

		if (bRslt == false)
		{
			//if (m_bIsNeedPrintErr)
			//{
			//  char u8arErr[128]={0};
			//  sprintf_s(u8arErr,"\n   %s will use 0.\n\n",strParmName.c_str());
			//  //pGetMtPrinter(id)->vPrintErr(u8arErr);
			//}
			*pRetValue = 0;
		}

		return bRslt;
	};
	bool bWriteFreqRegDefault(int nRegValue)
	{
		string strParmName = "WT_FREQ_CAL_DEFAULT_REG";
		return pGetParser()->iWriteInt(strParmName, nRegValue) == eumParserErr_OK;
	}
	bool bGet5GSeparateFreqRegDefault(int *pRetValue, int nMin, int nMax)
	{
		string strParmName = "WT_FREQ_CAL_DEFAULT_REG_5G";
		bool bRslt = bFetchIntAndCheck( strParmName, pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			(double)nMin, (double)nMax);

		if(bRslt == false)
		{
			*pRetValue = 0;
		}
		return bRslt;
	};
	bool bWrite5GSeparateFreqRegDefault(int nRegValue)
	{
		string strParmName = "WT_FREQ_CAL_DEFAULT_REG_5G";
		return pGetParser()->iWriteInt(strParmName, nRegValue) == eumParserErr_OK;
	}

	bool bGetCheckNewEFuseOption(bool *pRetValue)
	{
		int nEnable;
		bool bRslt =  bFetchIntAndCheck(	"WT_CHECK_EFUSE_IS_NEW",&nEnable,
			eumParserCheckMode_Enum,
			0,1);
		if (bRslt)
		{
			if (nEnable)
			{
				*pRetValue = true;
			}
			else
			{
				*pRetValue = false;
			}
		}
		return bRslt;
	};

	bool bGetMacLogEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_MAC_LOG_ENABLE", pRetValue);
	};

	bool bGetTotalLogEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_TOTAL_LOG_ENABLE", pRetValue);
	};

	bool bGetCSVLogEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_CSV_LOG_ENABLE", pRetValue);
	};

	bool bGetCsvLogLibName(string *name)
	{
		return bFetchStringAndCheck("WT_CSV_LOG_LIB_NAME", name);
	}

	bool bGetWriteEfuseRequest(bool *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_WRITE_EFUSE_REQUEST", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bSetWriteEfuseRequest(bool bEnable)
	{
		return pGetParser()->iWriteInt("WT_WRITE_EFUSE_REQUEST",(int)bEnable) == eumParserErr_OK;
	}

	string strGetTestStatisticStatusParmName(){return "WT_SAVE_TEST_STATISTIC";};

	bool bGetTestStatisticStatus(bool *pRetValue)
	{
		return bFetchBoolean(strGetTestStatisticStatusParmName(), pRetValue);
	}

	bool bSetTestStatisticStatus(bool bEnable)
	{
		return pGetParser()->iWriteInt(strGetTestStatisticStatusParmName(),(int)bEnable) == eumParserErr_OK;
	}

	bool bGetTestClearFail(bool *pRetValue)
	{
		return bFetchBoolean("WT_TEST_CLEAR_FAIL", pRetValue);
	}

	bool bGetPowOffset2_4G(int demod, double *pRetValue)
	{
		string strName;
		if (demod == eumDemod_11b)
		{
			strName = "WT_POWER_OFFSET_11B_DIFF";
		}
		else if (demod == eumDemod_11n_20M)
		{
			strName = "WT_POWER_OFFSET_HT201S_DIFF";
		}
		else if (demod == eumDemod_11n_40M)
		{
			strName = "WT_POWER_OFFSET_HT401S_DIFF";
		}
		else
		{
			*pRetValue = 0;
			return true;
		}
		return bFetchDoubleAndCheck(strName,pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-100,100);
	}

	bool bGetPowOffset5G(int demod, double *pRetValue)
	{
		string strName;
		if (demod == eumDemod_11n_20M)
		{
			strName = "WT_POWER_OFFSET_HT201S_DIFF_5G";
		}
		else if (demod == eumDemod_11n_40M)
		{
			strName = "WT_POWER_OFFSET_HT401S_DIFF_5G";
		}
		else if(demod == eumDemod_11ac_80M)
		{
			strName = "WT_POWER_OFFSET_AC80_DIFF_5G";
		}
		else if(demod == eumDemod_11ac_20M)
		{
			strName = "WT_POWER_OFFSET_AC20_DIFF_5G";
		}
		else if(demod == eumDemod_11ac_40M)
		{
			strName = "WT_POWER_OFFSET_AC40_DIFF_5G";
		}
		else
		{
			*pRetValue = 0;
			return true;
		}
		return bFetchDoubleAndCheck(strName,pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-100,100);
	}

	bool bGetFreeBlockSurvireMax(int *pFreeBlockSurvire)
	{
		return bFetchIntAndCheck("WT_FREE_BOLCK_SURVIRE_MAX", pFreeBlockSurvire, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 1024);
	}

	bool bGetDutCommandAckToken(string *pAck)
	{
		return bFetchStringAndCheck("WT_DUT_ACK_TOKEN", pAck);
	}

	bool bGetDutCommandEnd(string *pStrEnd)
	{
		string strRetString = "";
		int nLine;
		int iErrCode = pGetParser()->iReadString("WT_DUT_COMMAND_END", &strRetString, &nLine);

		switch(iErrCode)
		{
		case eumParserErr_OK:
			{
				break;
			}
		case eumParserErr_MissToken:
			{
				return false;
			}
		case eumParserErr_MissContent:
			{
				*pStrEnd  = "";
				return true;
			}
		default:
			{
				return false;
			}
		}
		*pStrEnd  = strRetString;
		return true;
	}

	bool bGetDutDriverServicesName(string *pServicesName)
	{
		string strParmName = "WT_DRIVER_SERVICES";
		if (bFetchStringAndCheck(strParmName, pServicesName) == false)
		{
			*pServicesName = "";
		}
		return true;
	}

	bool bGetAutoTestWhenDutReady(bool *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_AUTO_TEST_WHEN_DUT_READY", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetAutoTestWhenPopUpOk(bool *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_START_TEST_WHEN_POPUP_OK", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetTestCounts(int *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck(
			"WT_TEST_COUNTS",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,50000);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetTestDelay(int *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck(
			"WT_TEST_DELAY",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,10000);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetStopAfterFail(bool *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_STOP_AFTER_FAIL", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	};

	bool bGetTotalEfuseNumOfNewDUT(int *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchIntAndCheck("WT_TOTAL_EFUSE_NUMBER_OF_NEWDUT",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,0,2048);
		vEnableErrorPrint();
		return bRslt;
	}
	//add by lirunting 新岸线NL6621M对功率值的插补开关
	bool bGetInterpolatePower(bool *pRetValue)
	{
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_INTERPOLATE_POWER", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	}
	// Ralink 
	bool bGetSendAtedCommandEnable(bool *pRetValue)
	{
		//MARK:当无法获取该参数时，视为获取到false，不报错
		vDisableErrorPrint();
		if (bFetchBoolean("WT_OPEN_ATED_SERVICE",pRetValue) == false)
		{
			*pRetValue = false;
		}
		vEnableErrorPrint();
		return true;
	};
	bool bGetAtedCommand(string *pstrCommand)
	{
		//MARK:当无法获取Command字符串时，视为获取到空字符串，不报错
		vDisableErrorPrint();
		string strParmName = "WT_ATED_COMMAND";
		if (bFetchStringAndCheck(strParmName,pstrCommand) == false)
		{
			*pstrCommand = "";
		}
		vEnableErrorPrint();
		return true;
	}
	bool bGetAlcCalibration(bool *pRetValue)
	{
		return bFetchBoolean("WT_ALC_CALIBRATION",pRetValue);
	};
	bool bGetAlcBeforeVerify(bool *pRetValue)
	{
		return bFetchBoolean("WT_ALC_TX_VERIFY_ENABLE", pRetValue);
	};
	bool bGetAlcVerifyDelay(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_ALC_TX_VERIFY_DELAY", pRetValue, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 15);
	};
	bool bGetAlcRefValueCust(bool *pRetValue)
	{
		return bFetchBoolean("WT_ALC_FIX_REF_VALUE_ENABLE", pRetValue);
	};
	bool bGetAlcDeltaTableCust(bool *pRetValue)
	{
		return bFetchBoolean("WT_ALC_DELTA_TABLE_ENABLE", pRetValue);
	};
	bool bGetAlcRefValue(int* pRefValue)
	{
		return bFetchIntAndCheck("WT_ALC_REF_VALUE", pRefValue, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 200);
	};
	bool bGetAlcDeltaValue(int* pDeltaArray, int cnt)
	{
		return bFetchIntArrayAndCheck("WT_ALC_DELTA_TABLE", pDeltaArray, cnt, eumParserCheckMode_Min|eumParserCheckMode_Max, -8, 7);
	};
	bool bGetUartPort(int *pUartPort)
	{
		return bFetchIntAndCheck("WT_COM_PORT", pUartPort, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 100);
	}
	bool bGetUartBaud(int *pBaud)
	{
		return bFetchIntAndCheck("WT_COM_BAUD", pBaud, eumParserCheckMode_Min|eumParserCheckMode_Max, 2400, 921600);
	}
	bool bGetUartRxFrameGap_ms(int *pxFrameGap)
	{
		return bFetchIntAndCheck("WT_COM_RX_FRAM_GAP", pxFrameGap, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 100);
	}
	bool bGetUartByteInterval_ms(int *pBaud)
	{
		return bFetchIntAndCheck("WT_COM_BYTE_INTER", pBaud, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 50);
	}
	bool bGetLocalNetCardIndex(int *pNetCardIndex)
	{
		return bFetchIntAndCheck("WT_LOCAL_NET_CART_NUMBER", pNetCardIndex, eumParserCheckMode_Min|eumParserCheckMode_Max, 1, 100);
	}
	bool bWriteLocalNetCardIndex(int nNetCardIndex)
	{
		return pGetParser()->iWriteInt("WT_LOCAL_NET_CART_NUMBER",nNetCardIndex) == eumParserErr_OK;
	}
	bool bGetEepromFileName(string *pEepromFile)		//用于Ralink AP,读取eeprom模板的文件名称
	{
		return bFetchStringAndCheck("WT_EEPROM_FILE",pEepromFile);
	}
	bool bGetEepromCorrectionEnable(bool *pRetVal)
	{
		return bFetchBoolean("WT_EEPROM_CORRECTION",pRetVal);
	}
	bool bGetRestoreFactorySettingsEnable(bool *pRetVal)		// 是否恢复出厂设置
	{
		vDisableErrorPrint();
		if (false == bFetchBoolean("WT_RESTORE_FACTORY_SETTINGS",pRetVal))
		{
			*pRetVal = false;
		}
		vEnableErrorPrint();
		return true;
	}

	bool bGetPowRegDefault_Ralink(int nFreqGroup,int nAnt,int nDemode,int *pRegBuf,int nMin,int nMax);
	bool bWritePowRegDefault_Ralink(int nFreqGroup,int nAnt,int nDemode,int nChannel, int nRegValue);

	bool bWritePowRegDefault_AC_Ralink(int nFreqGroup,int nAnt,int nDemode,int nChannel, int nRegValue);
	bool bGetPowRegDefault_AC_Ralink(int nFreqGroup,int nAnt,int nDemode,int *pRegBuf,int nMin,int nMax);

	bool bGetPowRegDefault_MTK_TSSI(int nFreqGroup,int nAnt,int nDemode,int nGroupCount,int *pRegBuf,int nMin,int nMax);
	bool bWritePowRegDefault_MTK_TSSI(int nFreqGroup,int nAnt,int nDemode,int nGroupIndex, int nRegValue);
	// Ralink

	//Realtek
	bool bGetDutFlashInitEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_DUT_FLASH_INIT",pRetValue);
	};
	bool bGetDutShEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_DUT_SH_ENABLE",pRetValue);
	};
	bool bGetOpenDutTimeout(int *pRetValue_sec)
	{
		return bFetchIntAndCheck("WT_OPEN_DUT_TIMEOUT",
			pRetValue_sec,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			1,
			120);
	};
	bool bGetBatFile(string *strBatFile)
	{
		return bFetchStringAndCheck("WT_BAT_FILE",strBatFile);
	}
	bool bGetDutDeviceClassGUIDName(string *pDeviceClassGUIDName)
	{
		string strParmName = "WT_DEVICE_CLASSGUID";
		if (bFetchStringAndCheck(strParmName, pDeviceClassGUIDName) == false)
		{
			*pDeviceClassGUIDName = "";
		}
		return true;
	}
	bool bGetDutDeviceDescriptionName(string *pDeviceDescriptionName)
	{
		string strParmName = "WT_DEVICE_DESCRIPTION";
		if (bFetchStringAndCheck(strParmName, pDeviceDescriptionName) == false)
		{
			*pDeviceDescriptionName = "";
		}
		return true;
	}

	//SDIO版卡
	bool bGetPcieClassGUIDName(string *pPcieClassGUIDName)
	{
		string strParmName = "WT_PCIE_CLASSGUID";
		if (bFetchStringAndCheck(strParmName, pPcieClassGUIDName) == false)
		{
			*pPcieClassGUIDName = "";
		}
		return true;
	}
	bool bGetPcieDescriptionName(string *pPcieDescriptionName)
	{
		string strParmName = "WT_PCIE_DESCRIPTION";
		if (bFetchStringAndCheck(strParmName, pPcieDescriptionName) == false)
		{
			*pPcieDescriptionName = "";
		}
		return true;
	}


	bool bGetDutEfuseType(int *pRetValue);
	bool bGetPowRegOffSetDefault_2_4G_Realtek(int nAnt,int nDemode,int *pRegBuf,int nRegCount,int nMin,int nMax);
	bool bGetPowRegOffSetDefault_5G_Realtek(int nAnt,int nDemode,int *pRegBuf,int nRegCount,int nMin,int nMax);

	bool bGetPowRegDefault_2_4G_Realtek(int nAnt,int nDemode,int *pRegBuf,int nRegCount,int nMin,int nMax);
	bool bGetPowRegDefault_5G_Realtek(int n5GSubChannelGroup,int nAnt,int nDemode,int *pRegBuf,int nRegCount,int nMin,int nMax);
	bool bWritePowRegDefault_2_4G_Realtek(int nAnt,int nDemode,int nRegIndex,int nRegValue);
	bool bWritePowRegDefault_5G_Realtek(int n5GSubChannelGroup,int nAnt,int nDemode,int nRegIndex,int nRegValue);
	bool bGetCheckEnable(bool *pRetValue);
	//Realtek

	//MTK
	bool bGetMultiCmdEnable(bool *pRetValue)
	{
		return bFetchBoolean("WT_MULTI_CMD_ENABLE",pRetValue);
	};

	bool bGetTcpApprove(bool *pRetValue)
	{
		return bFetchBoolean("WT_TCP_APPROVE",pRetValue);
	};

	bool bGetIsNeedLinkMes(bool *flag)
	{
		return bFetchBoolean("WT_IS_NEED_LINKMES",flag);
	}
	bool bGetMesLibName(string *name)
	{
		return bFetchStringAndCheck("WT_MES_LIB_NAME", name);
	}
	bool bGetFailLogNameFormat(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_FAIL_LOG_NAME_FORMAT", pRetValue, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 100);
	}
	bool bGetPassLogNameFormat(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_PASS_LOG_NAME_FORMAT", pRetValue, eumParserCheckMode_Min|eumParserCheckMode_Max, 0, 100);
	}

	//BCM

	//是否在测试完成后关闭BOOT_WAIT
	bool bGetCloseBootWait(bool *pRetValue)
	{
		return bFetchBoolean("WT_CLOSE_BOOT_WAIT",pRetValue);
	};

	bool bGetWlCommandFormat(string* pRetValue)
	{
		return bFetchStringAndCheck("WT_WL_COMMAND_FORMAT",pRetValue);

	};

	bool bGetTxVerifyMode(bool *pRetValue)
	{
		return bFetchBoolean("WT_TX_VERIFY_MODE",pRetValue);
	};

	bool bGetDelayBeforeNvramCommit(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_DELAY_BEFORE_NVRAM_COMMIT",
			pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			0,
			5);
	};

	bool bGetPERTestKeyWord(string* pRetValue)
	{
		return bFetchStringAndCheck("WT_PER_TEST_KEYWORD",pRetValue);
	};
	//zxm
	bool bGetCmdSendMode(bool *pRetValue)
	{
		return bFetchBoolean("WT_CMD_SEND_MODE",pRetValue);
	}

	bool bGetOprateHeadCmd(string *pRetValue)
	{
		return bFetchStringAndCheck("WT_DUT_OPRATE_HEAD",pRetValue);
	}

	bool bGetCliAck(string *pRetValue)
	{
		return bFetchStringAndCheck("WT_DUT_CLI_ACK_TOKEN",pRetValue);
	}

	bool bGetInterActiveAck(string *pRetValue)
	{
		return bFetchStringAndCheck("WT_ACK_INTERACTIVE",pRetValue);
	}

	//RTL_Card
	bool bGetSwitchTxPwrTracking(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_SWITCH_TX_PWR_TRACKING",
			pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			0,
			10);
	};

protected:
	bool bGetPowRegDefault_2_4G_Ralink(int nAnt,int *pRegBuf,int nMin,int nMax);
	bool bGetPowRegDefault_5G_Ralink(int nAnt,int *pRegBuf,int nMin,int nMax);

	bool bWritePowRegDefault_2_4G_Ralink(int nAnt,int nChannel,int nRegValue);
	bool bWritePowRegDefault_5G_Ralink(int nAnt,int nChannel,int nRegValue);

	bool bGetPowRegDefault_5G_AC_Ralink(int nAnt,int *pRegBuf,int nMin,int nMax);
	bool bWritePowRegDefault_5G_AC_Ralink(int nAnt,int nChannel,int nRegValue);

	bool bGetPowRegDefault_2_4G_MTK_TSSI(int nAnt,int *pRegBuf,int nGroupCont,int nMin,int nMax);
	bool bGetPowRegDefault_5G_MTK_TSSI(int nAnt,int *pRegBuf,int nGroupCont,int nMin,int nMax);

	bool bWritePowRegDefault_2_4G_MTK_TSSI(int nAnt,int nGroupIndex,int nRegValue);
	bool bWritePowRegDefault_5G_MTK_TSSI(int nAnt,int nGroupIndex,int nRegValue);
private:
	ThreadSection m_SetPassFailCritSection;
};


//tester 解释器
class WTCONFIG_API cParserTester
	:public cParserParm
{
public:
	cParserTester(string configFile="./WT_SETUP/WT_TESTER.txt",int core_id=0)
		:cParserParm(configFile, eumTesterEncrypt, core_id){};
	virtual ~cParserTester(){};

public:
	bool bGetRunLogEnable(bool *pIsAllow)
	{
		return bFetchBoolean("WT_RECORD_RUN_LOG",pIsAllow);
	}
	bool bGetRunTxRxLockEnable(bool *pIsEable)
	{
		return bFetchBoolean("WT_TX_RX_LOCK",pIsEable);
	}
    bool startTestByExtInput()
    {
        bool enable = false;
        bFetchBoolean("WT_START_TEST_BY_INPUT", &enable);
        return enable;
    }
	bool bGetParallelThreadNum(int* sum)
	{
		int nCount=0;
		string strParmName = "WT_DUT_PARALLEL_NUM";
		bool bRslt = bFetchIntAndCheck(strParmName,&nCount,
			eumParserCheckMode_Enum,
			1,
			8);
		if (bRslt == false)
		{
			*sum = 1;
			bRslt = true;
		}
		else
		{
			*sum = nCount;
		}
		return bRslt;
	};
	bool bGetStartTestNum(int* num)
	{
		int nCount=0;
		string strParmName = "WT_DUT_START_NUM";
		bool bRslt = bFetchIntAndCheck(strParmName,&nCount,
			eumParserCheckMode_Enum,
			1,
			8);
		if (bRslt == false)
		{
			*num = 1;
			bRslt = true;
		}
		else
		{
			*num = nCount;
		}
		return bRslt;
	}
	bool bGetIP(string *pIP)
	{
		string strParmName = "WT_IP_ADDRESS";
		if (bFetchStringAndCheck(strParmName,pIP) == false)
		{
			*pIP = "192.168.10.254";
			return false;
		}
		return true;
	};
	bool bGetTesterConnectMode(int *pTesterConnectMode)
	{
		return bFetchIntAndCheck(
			"WT_TESTER_CONNECT_MODE",
			pTesterConnectMode,
			eumParserCheckMode_Enum,
			eumTesterConnectMode_Normal,
			eumTesterConnectMode_Queue);
	}
	bool bGetRfPort(int *pPort)
	{
		int nPort=0;
		string strParmName = "WT_TX_RX_PORT_ENUM";
		bool bRslt = bFetchIntAndCheck(strParmName,&nPort,
			eumParserCheckMode_Enum,
			eumParserTesterPort_RF1,
			eumParserTesterPort_RF4);
		if (bRslt == false)
		{
			*pPort = TESTER_PORT_RF1;
			return false;
		}
		switch(nPort)
		{
		case eumParserTesterPort_RF1:
			nPort = TESTER_PORT_RF1;
			break;
		case eumParserTesterPort_RF2:
			nPort = TESTER_PORT_RF2;
			break;
		case eumParserTesterPort_RF3:
			nPort = TESTER_PORT_RF3;
			break;
		case eumParserTesterPort_RF4:
			nPort = TESTER_PORT_RF4;
			break;
		default:
			nPort = TESTER_PORT_RF1;
			break;
		}
		*pPort = nPort;
		return true;
	}

	bool bGetConnectTesterRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_CONNECT_TESTER_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}

	bool bGetConnectDutRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_CONNECT_DUT_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}

	bool bGetFreqOffset(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_RX_MOD_SHIFT_FREQ_HZ",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-10000000,10000000);
	}
	bool bGetContinueWaveFreqOffset(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_RX_CW_SHIFT_FREQ_HZ",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-10000000,10000000);
	}
	bool bGetCalibrationRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_CALIBRATION_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}
	//蓝牙功率校准重试次数
	bool bGetCalibrationBTRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_CALIBRATION_BT_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}
	bool bGetTxVerifyRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_TX_VERIFY_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}
	bool bGetRxVerifyRetry(int *pRetry)
	{
		return bFetchIntAndCheck("WT_RX_VERIFY_RETRY",pRetry,eumParserCheckMode_Min|eumParserCheckMode_Max,1,100);
	}
	bool bGetShowVerifyRetryProcess(bool *pIsShow)
	{
		return bFetchBoolean("WT_PRINT_RETRY_PROCESS",pIsShow);
	}
	bool bGetRunAllVerify(bool *pIsRunAllFlow)
	{
		return bFetchBoolean("WT_RUN_ALL_TEST_FLOW",pIsRunAllFlow);
	}
	bool bGetCalPowOffset(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_CAL_POWER_OFFSET",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,-100,100);
	}
	bool bGetPERTotalPacket(int nDemode,int nMcs,int *pPacketCount)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				strParmName = "WT_PER_PACKETS_NUM_LEGACY";
				break;
			}
		case eumDemod_11n_20M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				strParmName = "WT_PER_PACKETS_NUM_HT20_1S";
				break;
			}
		case eumDemod_11n_40M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}
				strParmName = "WT_PER_PACKETS_NUM_HT40_1S";
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (nMcs >= m_nAC20McsParmCountInOneLine)
				{
					nMcs -= m_nAC20McsParmCountInOneLine;
				}
				strParmName = "WT_PER_PACKETS_NUM_AC20_1S";
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (nMcs >= m_nAC40McsParmCountInOneLine)
				{
					nMcs -= m_nAC40McsParmCountInOneLine;
				}
				strParmName = "WT_PER_PACKETS_NUM_AC40_1S";
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}
				strParmName = "WT_PER_PACKETS_NUM_AC80_1S";
				break;
			}

		case eumDemod_BT:
			{
				//nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_BT";
				break;
			}
			
		case eumDemod_Zigbee:
			{
				nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_ZIGBEE";
				break;
			}

		}

		double dPacketCount=0;
		if (bGetMcsParm(strParmName,nDemode,nMcs,&dPacketCount,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			1,1000000) == false)
		{
			return false;
		}
		*pPacketCount = (int)dPacketCount;
		return true;
	};
	bool bGetPERTotalPacket(int band,int nDemode,int nMcs,int *pPacketCount)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_11ag:
		case eumDemod_11b:
			{
				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_LEGACY";
				}
				else if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_11A";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_11A";
				}
				break;
			}
		case eumDemod_11n_20M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_HT20_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_HT20_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_HT20_1S";
				}
				break;
			}
		case eumDemod_11n_40M:
			{
				if (nMcs >= m_n11nMcsParmCountInOneLine)
				{
					nMcs -= m_n11nMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_HT40_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_HT40_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_HT40_1S";;
				}
				break;
			}
		case eumDemod_11ac_20M:
			{
				if (nMcs >= m_nAC20McsParmCountInOneLine)
				{
					nMcs -= m_nAC20McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AC20_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AC20_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AC20_1S";
				}
				
				break;
			}
		case eumDemod_11ac_40M:
			{
				if (nMcs >= m_nAC40McsParmCountInOneLine)
				{
					nMcs -= m_nAC40McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AC40_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AC40_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AC40_1S";
				}
				break;
			}
		case eumDemod_11ac_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AC80_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AC80_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AC80_1S";
				}
				break;
			}
		case eumDemod_11ac_160M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AC160_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AC160_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AC160_1S";
				}
				break;
			}
		case eumDemod_11ac_80M_80M:
			{
				if (nMcs >= m_nAC80McsParmCountInOneLine)
				{
					nMcs -= m_nAC80McsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AC8080_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AC8080_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AC8080_1S";
				}
				break;
			}

		case eumDemod_11ax_20M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AX20_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AX20_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AX20_1S";
				}
				break;
			}
		case eumDemod_11ax_40M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AX40_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AX40_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AX40_1S";
				}
				break;
			}
		case eumDemod_11ax_80M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AX80_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AX80_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AX80_1S";
				}
				break;
			}
		case eumDemod_11ax_160M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AX160_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AX160_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AX160_1S";
				}
				break;
			}
		case eumDemod_11ax_80_80M:
			{
				if (nMcs >= m_nAXMcsParmCountInOneLine)
				{
					nMcs -= m_nAXMcsParmCountInOneLine;
				}

				if (eumFreqGroup2_4G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_2_4G_AX8080_1S";
				}
				else if (eumFreqGroup5G == band)
				{
					strParmName = "WT_PER_PACKETS_NUM_5G_AX8080_1S";
				}
				else
				{
					strParmName = "WT_PER_PACKETS_NUM_6G_AX8080_1S";
				}
				break;
			}

		case eumDemod_BT:
			{
				//nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_BT";
				break;
			}

		case eumDemod_Zigbee:
			{
				nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_ZIGBEE";
				break;
			}

		case eumDemod_GPS:
			{
				nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_GPS";
				break;
			}

		case eumDemod_BDS:
			{
				nMcs = 0;
				strParmName = "WT_PER_PACKETS_NUM_BDS";
				break;
			}

		}

		double dPacketCount=0;
		if (bGetMcsParm(strParmName,nDemode,nMcs,&dPacketCount,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			1,1000000) == false)
		{
			return false;
		}
		*pPacketCount = (int)dPacketCount;
		return true;
	};
	bool bGetPERFrameGap(int *pGap_us)
	{
		return bFetchIntAndCheck("WT_PER_FRAME_GAP",pGap_us,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,1000000);
	}

	//bool bGetBTPERFrameGap(int *pGap_us)
	//{
	//	return bFetchIntAndCheck("WT_BT_PER_FRAME_GAP",pGap_us,
	//		eumParserCheckMode_Min|eumParserCheckMode_Max,
	//		0,1000000);
	//}

	bool bGetBTPERFrameGap(int btMcs, int *pGap_us)
	{
		string strKeyword;
		if (enumBT_LE1M == btMcs)	
		{
			strKeyword = "WT_BT_LE_FRAME_GAP";
		}
		else if (enumBT_LE2M == btMcs)	
		{
			strKeyword = "WT_BT_LE2M_FRAME_GAP";
		}
		else if ((btMcs >= enumBT_DH1) && (btMcs <= enumBT_DH5))
		{
			strKeyword = "WT_BT_BR_PER_FRAME_GAP";
		}
		else if ((btMcs >= enumBT_2DH1) && (btMcs <= enumBT_3DH5))
		{
			strKeyword = "WT_BT_EDR_PER_FRAME_GAP";
		}
		else if (enumBT_LE_125K == btMcs)
		{
			strKeyword = "WT_BT_LE_125K_FRAME_GAP";
		}
		else if (enumBT_LE_500K == btMcs)
		{
			strKeyword = "WT_BT_LE_500K_FRAME_GAP";
		}

		return bFetchIntAndCheck(strKeyword, pGap_us,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			0, 1000000);
	}

	/*bool bGetPhaseTracking(int *pEnum)
	{
		int iEnum=WT_PH_CORR_SYM_BY_SYM;
		string strParmName = "WT_EVM_PH_CORR_MODE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_PH_CORR_OFF,
			WT_PH_CORR_MOVING_AVG);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetChannelEstimate(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}

	bool bGetChannelEstimate11G(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_11G_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetChannelEstimate11A(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_11A_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetChannelEstimate11N2_4G(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_2_4G_11N_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetChannelEstimate11N5G(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_5G_11N_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetAcChannelEstimate(int *pEnum)
	{
		int iEnum=WT_CH_EST_RAW_LONG;
		string strParmName = "WT_EVM_AC_CH_ESTIMATE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_CH_EST_RAW_LONG,
			WT_CH_EST_RAW_FULL);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetSymbolTimingTracking(int *pEnum)
	{
		int iEnum=WT_SYM_TIM_ON;
		string strParmName = "WT_EVM_SYM_TIM_CORR";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_SYM_TIM_OFF,
			WT_SYM_TIM_ON);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetFreqSync(int *pEnum)
	{
		int iEnum=WT_FREQ_SYNC_LONG_TRAIN;
		string strParmName = "WT_EVM_FREQ_SYNC";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_FREQ_SYNC_SHORT_TRAIN,
			WT_FREQ_SYNC_FULL_PACKET);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetAmplTracking(int *pEnum)
	{
		int iEnum=WT_AMPL_TRACK_OFF;
		string strParmName = "WT_EVM_AMPL_TRACK";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_AMPL_TRACK_OFF,
			WT_AMPL_TRACK_ON);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetEqualizerType(int *pEnum)
	{
		int iEnum=WT_EQ_OFF;
		string strParmName = "WT_EVM_11B_EQUALIZER";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_EQ_OFF,
			WT_EQ_9_TAPS);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetDCRemove(int *pEnum)
	{
		int iEnum=WT_DC_REMOVAL_OFF;
		string strParmName = "WT_EVM_11B_DC_REMOVAL";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_DC_REMOVAL_OFF,
			WT_DC_REMOVAL_ON);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}
	bool bGetPhaseTracking_11B(int *pEnum)
	{
		int iEnum=WT_PH_CORR_11b_ON;
		string strParmName = "WT_EVM_11B_PH_CORR_MODE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			WT_PH_CORR_11b_OFF,
			WT_PH_CORR_11b_ON);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}*/
	bool bGetDataCaptureMode(int *pEnum)
	{
		int iEnum=eumDataCaptureMode_FreeRun;
		string strParmName = "WT_DATA_CAPTURE_MODE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			eumDataCaptureMode_FreeRun,
			eumDataCaptureMode_Triger
			);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	};
	bool bGetVSASampleRateMode(int *pEnum)
	{
		int iEnum = 0;
		string strParmName = "WT_VSA_SAMPLE_RATE_MODE";
		bool bRslt = bFetchIntAndCheck(strParmName,&iEnum,
			eumParserCheckMode_Enum,
			0,
			1);
		if(bRslt)
		{
			*pEnum = iEnum;
		}
		return bRslt;
	}

	bool bGetDataCaptureTime(int nDemode, double *pRetValue ,int mcs=0)
	{
		string parmName = "";
		int rangeMin = 50;

		switch(nDemode)
		{
		case eumDemod_11b:
			parmName = "WT_DATA_CAPTURE_TIME_11B";
			rangeMin = 50;
			break;
		case eumDemod_11ag:
			parmName = "WT_DATA_CAPTURE_TIME_11AG";
			rangeMin = 50;
			break;
		case eumDemod_11n_20M:
		case eumDemod_11n_40M:
			parmName = "WT_DATA_CAPTURE_TIME_11N";
			rangeMin = 50;
			break;
		case eumDemod_11ac_20M:
		case eumDemod_11ac_40M:
		case eumDemod_11ac_80M:
		case eumDemod_11ac_160M:
		case eumDemod_11ac_80M_80M:
			parmName = "WT_DATA_CAPTURE_TIME_11AC";
			rangeMin = 50;
			break;
		case eumDemod_11ax_20M:
		case eumDemod_11ax_40M:
		case eumDemod_11ax_80M:
		case eumDemod_11ax_160M:
		case eumDemod_11ax_80_80M:
			parmName = "WT_DATA_CAPTURE_TIME_11AX";
			rangeMin = 50;
			break;
		case eumDemod_BT:
			switch(mcs)
			{
			case enumBT_LE2M:
				parmName = "WT_DATA_CAPTURE_TIME_BT_LE_2M";
				break;
			case enumBT_LE_125K:
				parmName = "WT_DATA_CAPTURE_TIME_BT_LE_125K";
				break;
			case enumBT_LE_500K:
				parmName = "WT_DATA_CAPTURE_TIME_BT_LE_500K";
				break;
			default:
				parmName = "WT_DATA_CAPTURE_TIME_BT";
				break;
			}			
			rangeMin = 500;
			break;
		case eumDemod_Zigbee:
			parmName = "WT_DATA_CAPTURE_TIME_ZIGBEE";
			rangeMin = 50;
			break;
		default:
			parmName = "WT_DATA_CAPTURE_TIME";
			rangeMin = 50;
			break;

		}
		return bFetchDoubleAndCheck(parmName, pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max, rangeMin, 10000);
	}

	bool bGetDataCaptureTimeWithCW(double *pRetValue)
	{
		return bFetchDoubleAndCheck("WT_DATA_CAPTURE_TIME_CW",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,100,10000);
	}

	bool bGetDataCaptureAverage(int *pRetValue)
	{
		return bFetchIntAndCheck("WT_DATA_CAPTURE_AVERAGE",pRetValue,eumParserCheckMode_Min|eumParserCheckMode_Max,1,50);
	}

	bool bGetFreqCalDelayTime(int *pCount,int *pRetValue,int iFreqGroup)
	{
		bool bRslt = true;
		int leastCount = 0;  //所配置的最低个数，如果小于该个数，为获取配置错误
		int maxCount = 0;    //可以配置最多个数
		string strParmName="";

		switch(iFreqGroup)
		{
		case eumFreqGroup2_4G:
			strParmName = "WT_FREQ_CAL_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3; 
			leastCount = 6; //根据旧配置
			break;
		case eumFreqGroup5G:
			strParmName = "WT_FREQ_CAL_DELAY_MS_5G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		case eumFreqGroup6G:
			strParmName = "WT_FREQ_CAL_DELAY_MS_6G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		default:
			strParmName = "WT_FREQ_CAL_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3;
			leastCount = 6; //根据旧配置
			break;
		}

		
		bFetchIntArrayAndCheck(strParmName, pRetValue, maxCount, pCount, eumParserCheckMode_Min|eumParserCheckMode_Max,0,10000);

		if (*pCount < leastCount)
		{
			bRslt = bFetchIntArrayAndCheck(
				"WT_FREQ_CAL_DELAY_MS",pRetValue,maxCount,
				eumParserCheckMode_Min|eumParserCheckMode_Max,
				0,10000);
		}
		return bRslt;
	}

	bool bGetCalDelayTime(int *pCount,int *pRetValue, int iFreqGroup)
	{

		bool bRslt = true;
		int leastCount = 0;  //所配置的最低个数，如果小于该个数，为获取配置错误
		int maxCount = 0;    //可以配置最多个数
		string strParmName="";

		switch(iFreqGroup)
		{
		case eumFreqGroup2_4G:
			strParmName = "WT_POW_CAL_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3; 
			leastCount = 6; //根据旧配置
			break;
		case eumFreqGroup5G:
			strParmName = "WT_POW_CAL_DELAY_MS_5G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		case eumFreqGroup6G:
			strParmName = "WT_POW_CAL_DELAY_MS_6G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		default:
			strParmName = "WT_POW_CAL_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3;
			leastCount = 6; //根据旧配置
			break;
		}

		bFetchIntArrayAndCheck(strParmName, pRetValue, maxCount, pCount, eumParserCheckMode_Min|eumParserCheckMode_Max,0,10000);

		if (*pCount < leastCount)
		{
			bRslt = bFetchIntArrayAndCheck(
				"WT_POW_CAL_DELAY_MS",pRetValue,maxCount,
				eumParserCheckMode_Min|eumParserCheckMode_Max,
				0,10000);
		}

		return bRslt;
	}
	bool bGetVerifyDelayTime(int *pCount,int *pRetValue,int iFreqGroup)
	{
		bool bRslt = true;
		int leastCount = 0;  //所配置的最低个数，如果小于该个数，为获取配置错误
		int maxCount = 0;    //可以配置最多个数
		string strParmName="";

		switch(iFreqGroup)
		{
		case eumFreqGroup2_4G:
			strParmName = "WT_TX_VERIFY_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3; 
			leastCount = 6; //根据旧配置
			break;
		case eumFreqGroup5G:
			strParmName = "WT_TX_VERIFY_DELAY_MS_5G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		case eumFreqGroup6G:
			strParmName = "WT_TX_VERIFY_DELAY_MS_6G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		default:
			strParmName = "WT_TX_VERIFY_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3;
			leastCount = 6; //根据旧配置
			break;
		}

		bFetchIntArrayAndCheck(strParmName, pRetValue, maxCount, pCount, eumParserCheckMode_Min|eumParserCheckMode_Max,0,10000);

		if (*pCount < leastCount)
		{
			bRslt = bFetchIntArrayAndCheck(
				"WT_TX_VERIFY_DELAY_MS",pRetValue,maxCount,
				eumParserCheckMode_Min|eumParserCheckMode_Max,
				0,10000);
		}
		return bRslt;
	}

	bool bGetRXVerifyDelayTime(int *pCount,int *pRetValue,int iFreqGroup)
	{
		bool bRslt = true;
		int leastCount = 0;  //所配置的最低个数，如果小于该个数，为获取配置错误
		int maxCount = 0;    //可以配置最多个数
		string strParmName="";

		switch(iFreqGroup)
		{
		case eumFreqGroup2_4G:
			strParmName = "WT_RX_VERIFY_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3; 
			leastCount = 6; //根据旧配置
			break;
		case eumFreqGroup5G:
			strParmName = "WT_RX_VERIFY_DELAY_MS_5G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		case eumFreqGroup6G:
			strParmName = "WT_RX_VERIFY_DELAY_MS_6G";
			maxCount = eumDemod_11ac_80M_80M + 1;
			leastCount = maxCount;  //根据旧配置
			break;
		default:
			strParmName = "WT_RX_VERIFY_DELAY_MS_2_4G";
			maxCount = eumDemod_11ac_80M + 3;
			leastCount = 6; //根据旧配置
			break;
		}

		bFetchIntArrayAndCheck(strParmName, pRetValue, maxCount, pCount, eumParserCheckMode_Min|eumParserCheckMode_Max,0,10000);

		if (*pCount < leastCount)
		{
			bRslt = bFetchIntArrayAndCheck(
				"WT_RX_VERIFY_DELAY_MS",pRetValue,maxCount,
				eumParserCheckMode_Min|eumParserCheckMode_Max,
				0,10000);
		}
		return bRslt;
	}

	bool bGetPowCalibrationAccuracy(bool *pEn)
	{
		return bFetchBoolean("WT_CAL_POW_ACCURACY_EN",pEn);
	}
	bool bGetPowCalibration_ReducePowByEvm(bool *pEn)
	{
		return bFetchBoolean("WT_CAL_EVM_OPTIMIZE_POW_LOW_EN",pEn);
	}
	bool bGetPowCalibration_IncreasePowByEvm(bool *pEn)
	{
		return bFetchBoolean("WT_CAL_EVM_OPTIMIZE_POW_UP_EN",pEn);
	}

	bool bGetPowCalibrationAdvancedOptions(int *pRetValue,int iFreqGroup)
	{
		string strName = "";
		if(eumFreqGroup2_4G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_OPTIMIZE_2G";
		}
		else if(eumFreqGroup5G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_OPTIMIZE_5G";
		}
		else if(eumFreqGroup6G == iFreqGroup)
		{
			strName = "WT_POWER_CAL_OPTIMIZE_6G";
		}
		else
		{
			return false;
		}

		return bFetchIntAndCheck(strName,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,6);
	}

	bool bGetCarrierLeakageEn(int* Enable)
	{
		int nEnable=0;
		string strParmName = "WT_VERIFY_CARRIER_LEAKAGE_EN";
		bool bRslt = bFetchIntAndCheck(strParmName,&nEnable,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			0,
			5);
		if (bRslt == false)
		{
			*Enable = 0;
			bRslt = true;
		}
		else
		{
			*Enable = nEnable;
		}
		return bRslt;
	}

	bool bGetIsUesUserWaveFile(bool *pEn)
	{
		return bFetchBoolean("WT_USE_USER_WAVE_FILE",pEn);
	}
	
	bool bGetBandwidthMode(bool *retVal)
	{
		return bFetchBoolean("WT_SET_AUTO_BANDWIDTH_MODE", retVal);
	}
	
protected:
	enum{
		eumDataCaptureMode_FreeRun,
		eumDataCaptureMode_Triger,
	};
};


//atten 解析器
class WTCONFIG_API cParserExtAtten
	:public cParserParm
{
	public:
		cParserExtAtten(string configFile = "./WT_SETUP/WT_ATTEN.txt",int core_id=0)
			:cParserParm(configFile, eumAtten1Encrypt+core_id-1, core_id){};
		virtual ~cParserExtAtten(){};
		
	public:
		bool bGetExtAtten(int nFreqGrp,int nAnt,int nChannel,double *pRetValue)
		{
			double baseValue;
			if (!bFetchDoubleAndCheck(strGetAttenParmName(nFreqGrp,nAnt),&baseValue,
							eumParserCheckMode_Min|eumParserCheckMode_Max,-100,100))
				return false;

			double diffValue;
			if (!bGetExtAtten_DiffChannel(nFreqGrp, nAnt, nChannel, &diffValue))
				return false;
	
			*pRetValue = baseValue + diffValue;
			return true;
		}
		bool bSetExtAtten(int nFreqGrp,int nAnt,double dAtten)
		{
			return pGetParser()->iWriteDouble(strGetAttenParmName(nFreqGrp,nAnt),dAtten, "%5.1lf") == eumParserErr_OK;
		}

		int iGetExtAttenCount(int nFreqGrp)
		{
			//TODO: 当找不到WT_FIXED_ATTEN_2_4_CHAIN(N)时，bFetchDoubleAndCheck会使屏幕打印"无法找到..."
			//的错误，可以在Fetch机制中添加一种控制打印的接口
			/*int nAttenCount = 0;
			double dAttenValue=0;
			while(1)
			{
				if (bFetchDoubleAndCheck(strGetAttenParmName(nFreqGrp,nAttenCount),&dAttenValue) == false)
				{
					break;
				}
				nAttenCount ++;
			}
			return nAttenCount;*/
			return 2;	//HACK:暂定最多只有2天线
		}

		bool bGetExtAtten_DiffChannel(int nFreqGrp,int nAnt,int nChannel,double *pExtAtten_DiffChannel_dB);
		bool bSetExtAtten_DiffChannel(int nFreqGrp, int nAnt, int nChannel,double dExtAtten_DiffChannel_dB);

		bool bGetCableVerifyToggle(int nFreqGroup, bool *pRetValue);
		bool bSetCableVerifyToggle(int nFreqGroup, bool bValue);

		bool bGetCableVerifyAttenNum(int *pRetValue);

	private:
		string strGetAttenParmName(int nFreqGrp,int nAnt);
		string strGetCableVerifyToggleName(int nFreqGroup);
};


//enum{
//	eumMacIdx_Wifi=0,
//	eumMacIdx_Lan=1,
//	eumMacIdx_Wan=2,
//	eumMacIdx_Wifi_5G=3,	//额外添加的Mac地址
//	eumMacIdx_Bt=4,
//    eumMacIdx_Wan0_1=5,
//	eumMacIdx_Wan0_2=6,
//	eumMacIdx_Wan0_3=7,
//	eumMacIdx_Wan0_4=8,
//	eumMacIdx_Wan1_1=9,
//	eumMacIdx_Wan1_2=10,
//	eumMacIdx_Wan1_3=11,
//	eumMacIdx_Wan1_4=12,
//	eumMacIdx_Ex0=13,
//	eumMacIdx_Ex1=14,
//	eumMacIdx_MaxCount,
//};

//mac 解释器
class WTCONFIG_API cParserMac
	:public cParserParm
{
public:
	cParserMac(string configFile = "./WT_SETUP/WT_MAC.txt",int core_id=0)
		:cParserParm(configFile, eumMacEncrypt, core_id){};
	virtual ~cParserMac(){};
	
public:

	bool bGetScannInputType(int *InputType)
	{
		return bFetchIntAndCheck("WT_SCANN_INPUT_TYPE ",InputType,eumParserCheckMode_Enum,0,3);
	}

	bool bGetMacIncrement(int eumMacIdx,int *pIncrement)
	{
		string strParmName="";
		switch(eumMacIdx)
		{
		case eumMacIdx_Wan:
			{
				strParmName = "WT_MAC_WAN_INCREMENT";
				break;
			}
		case eumMacIdx_Lan:
			{
				strParmName = "WT_MAC_LAN_INCREMENT";
				break;
			}
		case eumMacIdx_Wifi:
			{
				strParmName = "WT_MAC_WIFI_INCREMENT";
				break;
			}
		case eumMacIdx_Wifi_5G:
			{
				strParmName = "WT_MAC_WIFI_INCREMENT_5G";
				break;
			}
		case eumMacIdx_Bt:
			{
				strParmName = "WT_MAC_BT_INCREMENT";
				break;
			}
		case eumMacIdx_Wan0_1:
			{
				strParmName = "WT_MAC_WAN0_1_INCREMENT";
				break;
			}
		case eumMacIdx_Wan0_2:
			{
				strParmName = "WT_MAC_WAN0_2_INCREMENT";
				break;
			}
		case eumMacIdx_Wan0_3:
			{
				strParmName = "WT_MAC_WAN0_3_INCREMENT";
				break;
			}
		case eumMacIdx_Wan0_4:
			{
				strParmName = "WT_MAC_WAN0_4_INCREMENT";
				break;
			}

		case eumMacIdx_Wan1_1:
			{
				strParmName = "WT_MAC_WAN1_1_INCREMENT";
				break;
			}
		case eumMacIdx_Wan1_2:
			{
				strParmName = "WT_MAC_WAN1_2_INCREMENT";
				break;
			}
		case eumMacIdx_Wan1_3:
			{
				strParmName = "WT_MAC_WAN1_3_INCREMENT";
				break;
			}
		case eumMacIdx_Wan1_4:
			{
				strParmName = "WT_MAC_WAN1_4_INCREMENT";
				break;
			}
		default:
			{
				int nExNum;
				if (eumMacIdx < eumMacIdx_MaxCount)
				{
					nExNum = eumMacIdx - eumMacIdx_Ex0;
					strParmName = cStringBase::strPrintf("WT_MAC_EX%d_INCREMENT",nExNum);
				}
				break;
			}
		}
		return bFetchIntAndCheck(strParmName,pIncrement);
	}
	bool bGetMacVendorId(string *pVendorID)
	{
		
		bool bRslt = bFetchStringAndCheck("WT_MAC_VENDOR_ID",pVendorID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pVendorID);
		}
		return bRslt;
	}
	bool bGetMacWriteEnable(int * pRegBuf,int nRegCount,int nMin,int nMax)
	{
		bool bRslt = bFetchIntArrayAndCheck(	
			"WT_MAC_WRITE_ENABLE",pRegBuf,nRegCount,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			(double)nMin,(double)nMax);

		return bRslt;
	}
	bool bGetMacVendorProtect(bool* pRetValue)
	{
		
		vDisableErrorPrint();
		bool bRslt = bFetchBoolean("WT_MAC_VENDOR_PROTECT", pRetValue);
		vEnableErrorPrint();
		return bRslt;
	}
	bool bGetMacProductId_Start(string *pStartID, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_MAC_RANGE_BEGIN_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_MAC_RANGE_BEGIN");
		}
		bool bRslt = bFetchStringAndCheck(buf,pStartID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pStartID);
		}
		return bRslt;
	}
	bool bGetMacProductId_End(string *pEndID, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_MAC_RANGE_END_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_MAC_RANGE_END");
		}
		bool bRslt =  bFetchStringAndCheck(buf,pEndID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pEndID);
		}
		return bRslt;
	}
	bool bGetMacCurrentId(string *pCurrentID, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_MAC_CURRENT_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_MAC_CURRENT");
		}
		bool bRslt =  bFetchStringAndCheck(buf,pCurrentID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pCurrentID);
		}
		return bRslt;
	}
	bool bSetMacCurrent(string strMacCurrent, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_MAC_CURRENT_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_MAC_CURRENT");
		}

		return pGetParser()->iWriteString(buf,strMacCurrent) == eumParserErr_OK;
	}
	bool bGetMacShow(int *pMacIdx)
	{
		return bFetchIntAndCheck("WT_MAC_SHOW",pMacIdx,eumParserCheckMode_Enum,0,4);
	}

	//BT
	bool bGetBdAddrIndenpendent(bool * pRetValue)
	{
		return bFetchBoolean("WT_BT_BD_INDENPENDENT", pRetValue);
	}
	bool bGetBDIncrement(int *pIncrement)
	{
		return bFetchIntAndCheck("WT_BT_BD_INCREMENT",pIncrement);
	}
	bool bGetBDAddrCurrent(string *pBDAddr,int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_BT_BD_CURRENT_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_BT_BD_CURRENT");
		}

		bool bRslt =  bFetchStringAndCheck(buf,pBDAddr,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		return bRslt;
	}
	bool bSetBDAddrCurrent(string strBDCurrent,int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_BT_BD_CURRENT_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_BT_BD_CURRENT");
		}

		return pGetParser()->iWriteString(buf,strBDCurrent) == eumParserErr_OK;
	}
	bool bGetBDAddrProductId_Start(string *pStartID, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_BT_BD_RANGE_BEGIN_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_BT_BD_RANGE_BEGIN");
		}
		bool bRslt = bFetchStringAndCheck(buf,pStartID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pStartID);
		}
		return bRslt;
	}
	bool bGetBDAddrProductId_End(string *pEndID, int id = -1)
	{
		char buf[32]={0};
		id = id < 0 ? iGetCoreId() : id;
		if (id)
		{
			sprintf_s(buf, "WT_BT_BD_RANGE_END_%d", id);
		}
		else
		{
			strcpy_s(buf, "WT_BT_BD_RANGE_END");
		}
		bool bRslt =  bFetchStringAndCheck(buf,pEndID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pEndID);
		}
		return bRslt;
	}
	bool bGetBtMacVendorId(string *pVendorID)
	{

		bool bRslt = bFetchStringAndCheck("WT_BT_BD_VENDOR",pVendorID,
			eumParserCheckMode_StringLength|eumParserCheckMode_StringHex,
			6,6);
		if (bRslt)
		{
			vMakeToUppercase(pVendorID);
		}
		return bRslt;
	}

	bool bGetMacOrder(int * pRegBuf,int nRegCount,int nMin,int nMax)
	{
		bool bRslt = bFetchIntArrayAndCheck(	
			"WT_MAC_WRITE_ORDER",pRegBuf,nRegCount,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			(double)nMin,(double)nMax);

		return bRslt;
	}
	bool bGetMacFromServerPosition(int *pSource)
	{
		return bFetchIntAndCheck("WT_MAC_GET_FROM_SERVER_POSITION",pSource,
								  eumParserCheckMode_Min|eumParserCheckMode_Max,
								  0,1);
	}
	bool bGetMacSource(int *pSource){return bFetchIntAndCheck("WT_MAC_SOURCE",pSource,
		eumParserCheckMode_Min | eumParserCheckMode_Max,
		0, 3);}
	bool bGetMacScannerType(int *pSource){return bFetchIntAndCheck("WT_MAC_SCANNER_TYPE",pSource,
		eumParserCheckMode_Min|eumParserCheckMode_Max,
		0,1);}
	bool bGetMacExternFileType(int *pSource){return bFetchIntAndCheck("WT_MAC_EXT_FILE_TYPE",pSource,
		eumParserCheckMode_Min|eumParserCheckMode_Max,
		0,1);}

	bool bIsScannerId()
	{
		bool enable = false;
		bFetchBoolean("WT_IS_SCANNER_ID", &enable);
		return enable;
	}
protected:
	void vMakeToUppercase(string *pContent)
	{
		transform(pContent->begin(), pContent->end(), pContent->begin(), ::toupper);
		return;
	}
	int m_coreId;
};

class WTCONFIG_API cParserDutConfig
{
public:
	cParserDutConfig();
	virtual ~cParserDutConfig(){};

	void updateDutConfig();

	string getDutType(){return m_strDutType;}

public:
	string m_strConfigFile;
	string m_strDutType;
};

class WTCONFIG_API cParserTestStatistics
	: public cParserParm
{
public:
	cParserTestStatistics(string configFile="./TestStatistics.txt",int core_id=0)
		:cParserParm(configFile, eumIsEncrypt, core_id)
	{
		m_strFileName = configFile;
	};
	virtual ~cParserTestStatistics(){};

public:
	//分别统计各个端口的pass和fail个数
	string strGetPassNumberParmName(){return "WT_PASS_NUMBER";};
	bool bGetPassNumber(int *pRetValue, int id = -1)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);
		char tmp[128] = {0};
		id = id < 0 ? iGetCoreId() : id;
		if (id > 0)
		{
			sprintf(tmp,"%s_%d",strGetPassNumberParmName().c_str(),id);
		}
		else
		{
			sprintf(tmp,"%s",strGetPassNumberParmName().c_str());
		}

		//return bFetchIntAndCheck(tmp,pRetValue,eumParserCheckMode_Min,0,0);		
		*pRetValue = GetPrivateProfileIntA("WT_STATISTICS", tmp, 0, m_strFileName.c_str());
		return true;
	}

	bool bGetTotalPassNumber(int *pRetValue)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);
		//return bFetchIntAndCheck(strGetPassNumberParmName(), pRetValue, eumParserCheckMode_Min,0,0);
		*pRetValue = GetPrivateProfileIntA("WT_STATISTICS", strGetPassNumberParmName().c_str(), 0, m_strFileName.c_str());
		return true;
	}

	bool bSetTotalPassNumber(int nPassNumber)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);
		//return pGetParser()->iWriteInt(strGetPassNumberParmName(), nPassNumber) == eumParserErr_OK;
		char numBuff[64] = {0};
		sprintf_s(numBuff, sizeof(numBuff), "%d", nPassNumber);
		return WritePrivateProfileStringA("WT_STATISTICS", strGetPassNumberParmName().c_str(), numBuff,  m_strFileName.c_str());
	}

	bool bSetPassNumber(int nPassNumber, int id = -1)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);

		bool bIsOk = true;
		char tmp[128] = {0};
		id = id < 0 ? iGetCoreId() : id;
		if (id > 0)
		{
			sprintf(tmp,"%s_%d",strGetPassNumberParmName().c_str(),id);
			////bIsOk &= (pGetParser()->iWriteInt(tmp,nPassNumber) == eumParserErr_OK);
			char numBuff[64] = {0};
			sprintf_s(numBuff, sizeof(numBuff), "%d", nPassNumber);
			bIsOk &= WritePrivateProfileStringA("WT_STATISTICS", tmp, numBuff,  m_strFileName.c_str());
#if 0
			//更新总的pass数量
			if (bIsOk)
			{
				int totalPass = 0;
				bIsOk &= bGetPassNumber(&totalPass);
				totalPass++;
				sprintf(tmp,"%s_TOTAL",strGetPassNumberParmName().c_str());
				bIsOk &= (pGetParser()->iWriteInt(tmp,totalPass) == eumParserErr_OK);
			}
#endif
			return bIsOk;
		}
		else
		{
			//直接设置总的pass数量
			sprintf(tmp,"%s",strGetPassNumberParmName().c_str());
			////return pGetParser()->iWriteInt(tmp,nPassNumber) == eumParserErr_OK;
			char numBuff[64] = {0};
			sprintf_s(numBuff, sizeof(numBuff), "%d", nPassNumber);
			return WritePrivateProfileStringA("WT_STATISTICS", tmp, numBuff,  m_strFileName.c_str());
		}
	}

	string strGetFailNumberParmName(){return "WT_FAIL_NUMBER";};
	bool bGetFailNumber(int *pRetValue, int id = -1)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);

		char tmp[128] = {0};
		id = id < 0 ? iGetCoreId() : id;
		if (id > 0)
		{
			sprintf(tmp,"%s_%d",strGetFailNumberParmName().c_str(),id);
		}
		else
		{
			sprintf(tmp,"%s",strGetFailNumberParmName().c_str());
		}
		////return bFetchIntAndCheck(tmp,pRetValue,eumParserCheckMode_Min,0,0);
		*pRetValue = GetPrivateProfileIntA("WT_STATISTICS", tmp, 0, m_strFileName.c_str());
		return true;
	}

	bool bSetFailNumber(int nFailNumber, int id = -1)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);

		bool bIsOk = true;
		char tmp[128] = {0};
		id = id < 0 ? iGetCoreId() : id;
		if (id > 0)
		{
			sprintf(tmp,"%s_%d",strGetFailNumberParmName().c_str(),id);
			////bIsOk &= (pGetParser()->iWriteInt(tmp,nFailNumber) == eumParserErr_OK);
			char numBuff[64] = {0};
			sprintf_s(numBuff, sizeof(numBuff), "%d", nFailNumber);
			bIsOk &=  WritePrivateProfileStringA("WT_STATISTICS", tmp, numBuff,  m_strFileName.c_str());
#if 0
			//更新总的fail数量
			if (bIsOk)
			{
				int totalFail = 0;
				bIsOk &= bGetFailNumber(&totalFail);
				totalFail++;
				sprintf(tmp,"%s_TOTAL",strGetFailNumberParmName().c_str());
				bIsOk &= (pGetParser()->iWriteInt(tmp,totalFail) == eumParserErr_OK);
			}
#endif
			return bIsOk;
		}
		else
		{
			//直接设置fail总数量
			sprintf(tmp,"%s",strGetFailNumberParmName().c_str());
			////return pGetParser()->iWriteInt(tmp,nFailNumber) == eumParserErr_OK;
			char numBuff[64] = {0};
			sprintf_s(numBuff, sizeof(numBuff), "%d", nFailNumber);
			return WritePrivateProfileStringA("WT_STATISTICS", tmp, numBuff,  m_strFileName.c_str());
		}
	}

	bool bGetTotalFailNumber(int *pRetValue)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);

		////return bFetchIntAndCheck(strGetFailNumberParmName(), pRetValue, eumParserCheckMode_Min,0,0);
		*pRetValue = GetPrivateProfileIntA("WT_STATISTICS", strGetFailNumberParmName().c_str(), 0, m_strFileName.c_str());
		return true;
	}

	bool bSetTotalFailNumber(int nFailNumber)
	{
		AutoThreadSection aSection(&m_vectrFileBufferMute);

		////return pGetParser()->iWriteInt(strGetFailNumberParmName(), nPassNumber) == eumParserErr_OK;
		char numBuff[64] = {0};
		sprintf_s(numBuff, sizeof(numBuff), "%d", nFailNumber);
		return WritePrivateProfileStringA("WT_STATISTICS", strGetFailNumberParmName().c_str(), numBuff,  m_strFileName.c_str());
	}

protected:
	string m_strFileName;

	ThreadSection m_vectrFileBufferMute;//多线程情况，对内存内容进行互斥操作
};

// GPS&BDS limit 解释器
class WTCONFIG_API cParserGPSBDSLimit
	: public cParserParm
{
public:
	cParserGPSBDSLimit(string configFile="./WT_SETUP/WT_GPS_BDS_LIMIT.txt",int core_id=0)
		:cParserParm(configFile, eumGpsBdsLmtEncrypt, core_id){};
	virtual ~cParserGPSBDSLimit(){};
public:
	bool bGetRssiPow(int nDemode,int nMcs,double *pRetValue)
	{
		string strParmName="";
		switch(nDemode)
		{
		case eumDemod_GPS:
			{
				nMcs  = 0;
				strParmName = "WT_PER_SENS_MIN_GPS";
				break;
			}
		case eumDemod_BDS:
			{
				nMcs  = 0;
				strParmName = "WT_PER_SENS_MIN_BDS";
				break;
			}
		}
		return bGetMcsParm(strParmName,nDemode,nMcs,pRetValue,
			eumParserCheckMode_Min|eumParserCheckMode_Max,
			-100,0);
	};

};

class WTCONFIG_API cParserManager
{
public:
	cParserManager(cParserDut *pcParserDut,
				   cParserTester *pcParserTester,
		           cParserWifiLimit *pcParserWifiLimit,
				   cParserBtLimit *pcParserBtLimit,
				   cParserZigbeeLimit *pcParserZigbeeLimit,
				   cParserGPSBDSLimit *pcParserGPSBDSLimit
				   )
	{
		m_pDutConfig = pcParserDut;
		m_pTesterConfig = pcParserTester;
		m_pWifiLimitConfig = pcParserWifiLimit;
		m_pBtLimitConfig = pcParserBtLimit;
		m_pZigbeeLimitConfig = pcParserZigbeeLimit;
		m_pGPSBDSLimitConfig = pcParserGPSBDSLimit;
	}
	virtual ~cParserManager(){};

	cParserDut *getDutCfg(){return m_pDutConfig;};
	cParserTester *getTesterCfg(){return m_pTesterConfig;};
	cParserWifiLimit *getWifiLimitCfg() {return m_pWifiLimitConfig;};
	cParserBtLimit *getBtLimitCfg() {return m_pBtLimitConfig;};
	cParserZigbeeLimit *getZigbeeLimitCfg() {return m_pZigbeeLimitConfig; };
	cParserGPSBDSLimit *getGPSBDSLimitCfg() {return m_pGPSBDSLimitConfig; };
private:
	cParserDut *m_pDutConfig;
	cParserTester *m_pTesterConfig;
	cParserWifiLimit *m_pWifiLimitConfig;
	cParserBtLimit *m_pBtLimitConfig;
	cParserZigbeeLimit *m_pZigbeeLimitConfig;
	cParserGPSBDSLimit *m_pGPSBDSLimitConfig;
};
WTCONFIG_API cParserDutConfig *pGetDutConfig();

#endif

namespace ExternNamespace
{
extern "C" __declspec(dllexport) BOOL __stdcall CreateObject(int n);

}


