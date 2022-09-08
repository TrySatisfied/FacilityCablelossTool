/*#############################################################

文件参数分析模块

##############################################################
Name: ConfigParser.cpp
Creat: gary
Date: 2012/05/18
Tool: VS2010
Dscpn: 文件参数分析模块。
***************************************************************/
#include "pch.h"
#include <sstream>
#include "ConfigParser.h"
#include "FreqGroupDescp.h"


static cParserDutConfig imDutConfig;

cParserDutConfig *pGetDutConfig()
{
	return &imDutConfig;
}
cParserDutConfig::cParserDutConfig()
{
	m_strConfigFile = ".\\Dut_Config.ini";
	m_strDutType = "";
	updateDutConfig();
}
void cParserDutConfig::updateDutConfig()
{
	char dut_type[256];
	for (int i = 0; i < 256; i++)
	{
		dut_type[i] = '\0';
	}

	GetPrivateProfileStringA("WT","DUT_TYPE","", dut_type,sizeof(dut_type),m_strConfigFile.c_str());

	m_strDutType = dut_type;

	return;
}

/*#####################################

cParserParm类

######################################*/
cParserParm::cParserParm(string strFileName, int core_id)
    : m_imConfigFile(strFileName, "//")
{
    m_coreID = core_id;
    m_bIsNeedPrintErr = true;
    m_strError = "";
}
cParserParm::cParserParm(string strFileName, string strCommentToken, int core_id)
    : m_imConfigFile(strFileName, strCommentToken)
{
    m_coreID = core_id;
    m_bIsNeedPrintErr = true;
    m_strError = "";
}

cParserParm::cParserParm(string strFileName, int cfgType, int core_id)
	: m_imConfigFile(strFileName, "//", cfgType)
{
	m_coreID = core_id;
	m_bIsNeedPrintErr = true;
	m_strError = "";
	m_cfgFileType = cfgType;

	SetFileAttributesA(strFileName.c_str(), FILE_ATTRIBUTE_NORMAL);
}

string cParserParm::strGetErrString_MissToken(string strFile, string strName)
{
    return cStringBase::strPrintf("   # Can not find %s in %s.\n", strName.c_str(), strFile.c_str());
}
string cParserParm::strGetErrString_MissContent(string strFile, string strName, int nLineInFile)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s miss content.\n\n", nLineInFile, strFile.c_str(), strName.c_str());
}
string cParserParm::strGetErrString_IntFormatErr(string strFile, string strName, int nLineInFile)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s format error. Please input integer.\n\n", nLineInFile, strFile.c_str(), strName.c_str());
}
string cParserParm::strGetErrString_OutOfValidRange_Int(string strFile, string strName, int nLineInFile, int iMin, int iMax)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s out of range (%d - %d).\n\n", nLineInFile, strFile.c_str(), strName.c_str(), iMin, iMax);
}
string cParserParm::strGetErrString_OutOfValidRange_Double(string strFile, string strName, int nLineInFile, double dMin, double dMax)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s out of range (%.2lf - %.2lf).\n\n", nLineInFile, strFile.c_str(), strName.c_str(), dMin, dMax);
}
string cParserParm::strGetErrString_OutOfValidRange_Enum(string strFile, string strName, int nLineInFile, double dMin, double dMax)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s unreachable.Please check the enumeration.\n\n", nLineInFile, strFile.c_str(), strName.c_str());
}
string cParserParm::strGetErrString_StringLength(string strFile, string strName, int nLineInFile, int nMinLength, int nMaxLength)
{
    if (nMinLength == nMaxLength)
    {
        return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s length error.The fit length is %d Bytes.\n\n", nLineInFile, strFile.c_str(), strName.c_str(), nMinLength);
    }

    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s length error.The length range is (%d - %d) Bytes.\n\n", nLineInFile, strFile.c_str(), strName.c_str(), nMinLength, nMaxLength);
}
string cParserParm::strGetErrString_HexFormat(string strFile, string strName, int nLineInFile)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s Hex format error.\n\n", nLineInFile, strFile.c_str(), strName.c_str());
}
string cParserParm::strGetErrString_MissElement(string strFile, string strName, int nLineInFile, int nMissElementCount)
{
    return cStringBase::strPrintf("   # Please check line %d in %s.\n   # %s lack of %d elements.\n\n", nLineInFile, strFile.c_str(), strName.c_str(), nMissElementCount);
}
//Fetch int
bool cParserParm::bFetchIntAndCheck(string strName, int *pRetValue, int eumParserCheckMode, double dMin, double dMax)
{
    int iRetVal = 0;
    int nLine;
    string strErrString = "";
    string strFile = pGetParser()->strGetFileName();
    int iErrCode = pGetParser()->iReadInt(strName, &iRetVal, &nLine);

    switch(iErrCode)
    {
    case eumParserErr_OK:
    {
        double dRetVal = (double)iRetVal;

        if (eumParserCheckMode & eumParserCheckMode_Enum)
        {
            if ((dRetVal < dMin) || (dRetVal > dMax))
            {
                strErrString = strGetErrString_OutOfValidRange_Enum(strFile, strName, nLine, (double)dMin, (double)dMax);
            }
        }
        else
        {
            if (eumParserCheckMode & eumParserCheckMode_Min)
            {
                if (dRetVal < dMin)
                {
                    strErrString = strGetErrString_OutOfValidRange_Int(strFile, strName, nLine, (int)dMin, (int)dMax);
                }
            }

            if (eumParserCheckMode & eumParserCheckMode_Max)
            {
                if (dRetVal > dMax)
                {
                    strErrString = strGetErrString_OutOfValidRange_Int(strFile, strName, nLine, (int)dMin, (int)dMax);
                }
            }
        }

        break;
    }

    case eumParserErr_MissToken:
    {
        strErrString = strGetErrString_MissToken(strFile, strName);
        break;
    }

    case eumParserErr_MissContent:
    {
        strErrString = strGetErrString_MissContent(strFile, strName, nLine);
        break;
    }

    case eumParserErr_IntFormat:
    {
        strErrString = strGetErrString_IntFormatErr(strFile, strName, nLine);
        break;
    }

    default:
    {
        return false;
    }
    }

    if (strErrString.length())
    {
        m_strError = strErrString;
        return false;
    }

    *pRetValue  = iRetVal;
    return true;
}
//Fetch Double
bool cParserParm::bFetchDoubleAndCheck(string strName, double *pRetValue, int eumParserCheckMode, double dMin, double dMax)
{
    double dRetVal = 0;
    int nLine;
    string strErrString = "";
    string strFile = pGetParser()->strGetFileName();
    int iErrCode = pGetParser()->iReadDouble(strName, &dRetVal, &nLine);

    switch(iErrCode)
    {
    case eumParserErr_OK:
    {
        if (eumParserCheckMode & eumParserCheckMode_Min)
        {
            if (dRetVal < dMin)
            {
                strErrString = strGetErrString_OutOfValidRange_Double(strFile, strName, nLine, dMin, dMax);
            }
        }

        if (eumParserCheckMode & eumParserCheckMode_Max)
        {
            if (dRetVal > dMax)
            {
                strErrString = strGetErrString_OutOfValidRange_Double(strFile, strName, nLine, dMin, dMax);
            }
        }

        break;
    }

    case eumParserErr_MissToken:
    {
        strErrString = strGetErrString_MissToken(strFile, strName);
        break;
    }

    case eumParserErr_MissContent:
    {
        strErrString = strGetErrString_MissContent(strFile, strName, nLine);
        break;
    }

    default:
    {
        return false;
    }
    }

    if (strErrString.length())
    {
        m_strError = strErrString;
        return false;
    }

    *pRetValue  = dRetVal;
    return true;
}
//Fetch String
bool cParserParm::bFetchStringAndCheck(string strName, string *pRetString, int eumParserCheckMode, int nMinLength, int nMaxLength)
{
    string strRetString = "";
    int nLine;
    string strErrString = "";
    string strFile = pGetParser()->strGetFileName();
    int iErrCode = pGetParser()->iReadString(strName, &strRetString, &nLine);

    switch(iErrCode)
    {
    case eumParserErr_OK:
    {
        if (eumParserCheckMode & eumParserCheckMode_StringLength)
        {
            if ((strRetString.length() < nMinLength) ||
                    (strRetString.length() > nMaxLength))
            {
                strErrString = strGetErrString_StringLength(strFile, strName, nLine, nMinLength, nMaxLength);
            }
        }

        if (eumParserCheckMode & eumParserCheckMode_StringHex)
        {
            if (cStringBase::bIsHexFormatValid(strRetString) == false)
            {
                strErrString = strGetErrString_HexFormat(strFile, strName, nLine);
            }
        }

        break;
    }

    case eumParserErr_MissToken:
    {
        strErrString = strGetErrString_MissToken(strFile, strName);
        break;
    }

    case eumParserErr_MissContent:
    {
        strErrString = strGetErrString_MissContent(strFile, strName, nLine);
        break;
    }

    default:
    {
        return false;
    }
    }

    if (strErrString.length())
    {
        m_strError = strErrString;
        return false;
    }

    *pRetString  = strRetString;
    return true;
}
//Fetch Int Array 
bool cParserParm::bFetchIntArrayAndCheck(string strName, int *pRetBuf, int nSz, int eumParserCheckMode, double dMin, double dMax)
{
    for (int i = 0; i < nSz; i++)
    {
        pRetBuf[i] = 0;
    }

    int nLine;
    int nElementNum;
    string strErrString = "";
    string strFile = pGetParser()->strGetFileName();
    int iErrCode = pGetParser()->iReadIntArray(strName, nSz, pRetBuf, &nElementNum, &nLine);

    switch(iErrCode)
    {
		case eumParserErr_OK:
		{
			bool bElementOutOfRange = false;

			for (int ii = 0; ii < nElementNum; ii++)
			{
				int iVal = pRetBuf[ii];

				if (eumParserCheckMode & eumParserCheckMode_Min)
				{
					if (iVal < (int)dMin)
					{
						bElementOutOfRange =  true;
						break;
					}
				}

				if (eumParserCheckMode & eumParserCheckMode_Max)
				{
					if (iVal > (int)dMax)
					{
						bElementOutOfRange = true;
						break;
					}
				}
			}
		
			if (bElementOutOfRange)
			{
				strErrString = strGetErrString_OutOfValidRange_Int(strFile, strName, nLine, (int)dMin, (int)dMax);
			}

			break;
		}

		case eumParserErr_MissToken:
		{
			strErrString = strGetErrString_MissToken(strFile, strName);
			break;
		}

		case eumParserErr_MissContent:
		{
			strErrString = strGetErrString_MissContent(strFile, strName, nLine);
			break;
		}

		case eumParserErr_MissElement:
		{
			strErrString = strGetErrString_MissElement(strFile, strName, nLine, nSz - nElementNum);
			break;
		}
    }

    if (strErrString.length())
    {

        m_strError = strErrString;
        return false;
    }

    return true;
}

//Fetch Int Array Add For B02T06R1 输出实际配置个数
bool cParserParm::bFetchIntArrayAndCheck(string strName, int *pRetBuf, int nSz, int *actualCount, int eumParserCheckMode, double dMin, double dMax)
{
	for (int i = 0; i < nSz; i++)
	{
		pRetBuf[i] = 0;
	}

	int nLine;
	int nElementNum;
	string strErrString = "";
	string strFile = pGetParser()->strGetFileName();
	int iErrCode = pGetParser()->iReadIntArray(strName, nSz, pRetBuf, &nElementNum, &nLine);

	*actualCount = nElementNum;

	switch(iErrCode)
	{
	case eumParserErr_OK:
		{
			bool bElementOutOfRange = false;

			for (int ii = 0; ii < nElementNum; ii++)
			{
				int iVal = pRetBuf[ii];

				if (eumParserCheckMode & eumParserCheckMode_Min)
				{
					if (iVal < (int)dMin)
					{
						bElementOutOfRange =  true;
						break;
					}
				}

				if (eumParserCheckMode & eumParserCheckMode_Max)
				{
					if (iVal > (int)dMax)
					{
						bElementOutOfRange = true;
						break;
					}
				}
			}

			if (bElementOutOfRange)
			{
				strErrString = strGetErrString_OutOfValidRange_Int(strFile, strName, nLine, (int)dMin, (int)dMax);
			}

			break;
		}

	case eumParserErr_MissToken:
		{
			strErrString = strGetErrString_MissToken(strFile, strName);
			break;
		}

	case eumParserErr_MissContent:
		{
			strErrString = strGetErrString_MissContent(strFile, strName, nLine);
			break;
		}

	case eumParserErr_MissElement:
		{
			strErrString = strGetErrString_MissElement(strFile, strName, nLine, nSz - nElementNum);
			break;
		}
	}

	if (strErrString.length())
	{

		m_strError = strErrString;
		return false;
	}

	return true;
}

//Fetch Double Array
bool cParserParm::bFetchDoubleArrayAndCheck(string strName, double *pRetBuf, int nSz, int eumParserCheckMode, double dMin, double dMax)
{
    for (int i = 0; i < nSz; i++)
    {
        pRetBuf[i] = 0;
    }

    int nLine;
    int nElementNum;
    string strErrString = "";
    string strFile = pGetParser()->strGetFileName();
    int iErrCode = pGetParser()->iReadDoubleArray(strName, nSz, pRetBuf, &nElementNum, &nLine);

    switch(iErrCode)
    {
    case eumParserErr_OK:
    {
        bool bElementOutOfRange = false;

        for (int ii = 0; ii < nElementNum; ii++)
        {
            double dVal = pRetBuf[ii];

            if (eumParserCheckMode & eumParserCheckMode_Min)
            {
                if (dVal < dMin)
                {
                    bElementOutOfRange =  true;
                    break;
                }
            }

            if (eumParserCheckMode & eumParserCheckMode_Max)
            {
                if (dVal > dMax)
                {
                    bElementOutOfRange = true;
                    break;
                }
            }
        }

        if (bElementOutOfRange)
        {
            strErrString = strGetErrString_OutOfValidRange_Int(strFile, strName, nLine, (int)dMin, (int)dMax);
        }

        break;
    }

    case eumParserErr_MissToken:
    {
        strErrString = strGetErrString_MissToken(strFile, strName);
        break;
    }

    case eumParserErr_MissContent:
    {
        strErrString = strGetErrString_MissContent(strFile, strName, nLine);
        break;
    }

    case eumParserErr_MissElement:
    {
        strErrString = strGetErrString_MissElement(strFile, strName, nLine, nSz - nElementNum);
        break;
    }
    }

    if (strErrString.length())
    {

        m_strError = strErrString;
        return false;
    }

    return true;
}
//Fetch bool
bool cParserParm::bFetchBoolean(string strParmName, bool *pRetValue)
{
    int nRetValue = 0;
    bool bRslt = bFetchIntAndCheck(strParmName, &nRetValue,
                                   eumParserCheckMode_Enum,
                                   0, 1);

    if ( bRslt)
    {
        if (nRetValue)
        {
            *pRetValue = true;
        }
        else
        {
            *pRetValue = false;
        }
    }

    return bRslt;
}

bool cParserParm::bGetMcsParm(string strParmName, int nDemode, int nMcs, double *pRetValue, int eumParserCheckMode, double dMin, double dMax)
{
    double dParm = 0;

    switch(nDemode)
    {
    case eumDemod_11ag:
    case eumDemod_11b:
    {
        double pBuf[eum11gMcsMax + eum11bMcsMax] = {0};
        bool bRslt = bFetchDoubleArrayAndCheck(strParmName, pBuf, eum11gMcsMax + eum11bMcsMax,
                                               eumParserCheckMode,
                                               dMin,
                                               dMax);

        if (bRslt == false)
        {
            return false;
        }

        if (nDemode == eumDemod_11ag)
        {
            dParm = pBuf[eum11gMcsMax - 1 - nMcs];
        }
        else
        {
            dParm = pBuf[eum11gMcsMax + eum11bMcsMax - 1 - nMcs];
        }

        break;
    }

    case eumDemod_11n_20M:
    case eumDemod_11n_40M:
    case eumDemod_11ac_20M:
    case eumDemod_11ac_40M:
    case eumDemod_11ac_80M:
	case eumDemod_11ac_160M:
	case eumDemod_11ac_80M_80M:
	case eumDemod_11ax_20M:
	case eumDemod_11ax_40M:
	case eumDemod_11ax_80M:
	case eumDemod_11ax_160M:
	case eumDemod_11ax_80_80M:
    {
        int nMcsTemp = nMcs;
        int nMcsCountInOneLine;

        switch(nDemode)
        {
        case eumDemod_11n_20M:
        case eumDemod_11n_40M:
            nMcsCountInOneLine = m_n11nMcsParmCountInOneLine;
            break;

        case eumDemod_11ac_20M:
            nMcsCountInOneLine = m_nAC20McsParmCountInOneLine;
            break;

        case eumDemod_11ac_40M:
            nMcsCountInOneLine = m_nAC40McsParmCountInOneLine;
            break;

        case eumDemod_11ac_80M:
		case eumDemod_11ac_160M:
		case eumDemod_11ac_80M_80M:
            nMcsCountInOneLine = m_nAC80McsParmCountInOneLine;
            break;

		case eumDemod_11ax_20M:
		case eumDemod_11ax_40M:
		case eumDemod_11ax_80M:
		case eumDemod_11ax_160M:
		case eumDemod_11ax_80_80M:
			nMcsCountInOneLine = m_nAXMcsParmCountInOneLine;
			break;
        }

        if (nMcsTemp >= nMcsCountInOneLine )
        {
            nMcsTemp -= nMcsCountInOneLine;
        }

        double pBuf[eum11acMcsMax] = {0};
        bool bRslt = bFetchDoubleArrayAndCheck(strParmName, pBuf, nMcsCountInOneLine,
                                               eumParserCheckMode,
                                               dMin,
                                               dMax);

        if (!bRslt)
        {
			if (nMcsTemp > (m_nAX80McsParmCountInOneLineOld - 1))
			{
				return false;
			}

			if ((nDemode >= eumDemod_11ac_20M) && (nDemode <= eumDemod_11ac_80M_80M) ||
				(nDemode >= eumDemod_11ax_20M) && (nDemode <= eumDemod_11ax_80_80M))
			{
				bRslt = bFetchDoubleArrayAndCheck(strParmName, pBuf, m_nAX80McsParmCountInOneLineOld,
					eumParserCheckMode,
					dMin,
					dMax);

				if (!bRslt)
				{
					return false;
				}

				dParm = pBuf[m_nAX80McsParmCountInOneLineOld - 1 - nMcsTemp];
				break;
			}
			else
			{
				return false;
			}    
        }

        dParm = pBuf[nMcsCountInOneLine - 1 - nMcsTemp];
        break;
    }
	case eumDemod_Zigbee:
	case eumDemod_GPS:
	case eumDemod_BDS:
		{
			double pBuf= 0;
			bool bRslt = bFetchDoubleArrayAndCheck(strParmName, &pBuf, 1,
				eumParserCheckMode,
				dMin,
				dMax);

			if (bRslt == false)
			{
				return false;
			}

			dParm = pBuf;
			break;
		}
	case eumDemod_BT:
		{
			double pBuf[7]= {0};
			int iNum = 1;

			if (nMcs == enumBT_LE1M)
			{
				iNum = 1;
			}
			else if (nMcs <= enumBT_DH5)
			{
				iNum = 2;
			}
			else if (nMcs <= enumBT_2DH5)
			{
				iNum = 3;
			}
			else if (nMcs <= enumBT_3DH5)
			{
				iNum = 4;
			}
			else if (nMcs == enumBT_LE2M)
			{
				iNum = 5;
			}
			else if (nMcs == enumBT_LE_125K)
			{
				iNum = 6;
			}
			else if (nMcs == enumBT_LE_500K)
			{
				iNum = 7;
			}
			bool bRslt = bFetchDoubleArrayAndCheck(strParmName, pBuf, iNum,
				eumParserCheckMode,
				dMin,
				dMax);

			if (bRslt == false)
			{
				return false;
			}

			dParm = pBuf[iNum - 1];
			break;
		}


    }

    *pRetValue = dParm;
    return true;
}
/*#####################################

cParserDut类

######################################*/

bool cParserDut::bGetPowRegDefault_2_4G_Realtek(int nAnt, int nDemode, int *pRegBuf, int nRegCount, int nMin, int nMax)
{
    string strParmName = "";

    if (nDemode == eumDemod_11b)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_11B_CH%d", nAnt);
    }
    //MARK:OFDM和HT20都以HT40为基础
    else
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_HT40_CH%d", nAnt);
    }

    return bFetchIntArrayAndCheck(
               strParmName, pRegBuf, nRegCount,
               eumParserCheckMode_Min | eumParserCheckMode_Max,
               (double)nMin, (double)nMax);
};
bool cParserDut::bGetPowRegDefault_5G_Realtek(int n5GSubChannelGroup, int nAnt, int nDemode, int *pRegBuf, int nRegCount, int nMin, int nMax)
{
    string strParmName = "";
    string str5GSubChannel[] = {"L", "M", "H"};
    strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_HT40_CH%d", str5GSubChannel[n5GSubChannelGroup].c_str(), nAnt);

    return bFetchIntArrayAndCheck(
               strParmName, pRegBuf, nRegCount,
               eumParserCheckMode_Min | eumParserCheckMode_Max,
               (double)nMin, (double)nMax);
};
bool cParserDut::bWritePowRegDefault_2_4G_Realtek(int nAnt, int nDemode, int nRegIndex, int nRegValue)
{
    string strParmName = "";

    if (nDemode == eumDemod_11b)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_11B_CH%d", nAnt);
    }
    //MARK:OFDM和HT20都以HT40为基础
    else if(nDemode == eumDemod_11n_40M)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_HT40_CH%d", nAnt);
    }
    else
    {
        return true;
    }

    return pGetParser()->iWriteIntArray(strParmName, nRegIndex, nRegValue);
}
bool cParserDut::bWritePowRegDefault_5G_Realtek(int n5GSubChannelGroup, int nAnt, int nDemode, int nRegIndex, int nRegValue)
{
    string strParmName = "";

    if(nDemode == eumDemod_11n_40M)
    {
        string str5GSubChannel[] = {"L", "M", "H"};
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_HT40_CH%d", str5GSubChannel[n5GSubChannelGroup].c_str(), nAnt);
    }
    else
    {
        return true;
    }

    return pGetParser()->iWriteIntArray(strParmName, nRegIndex, nRegValue) == eumParserErr_OK;
}
bool cParserDut::bGetPowRegOffSetDefault_2_4G_Realtek(int nAnt, int nDemode, int *pRegBuf, int nRegCount, int nMin, int nMax)
{
    string strParmName = "";

    if (eumDemod_11b == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_11B_CH%d", nAnt);
    }
    else if (eumDemod_11n_20M == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_2_4G_HT20_CH%d", nAnt);
    }
    else if (eumDemod_11ag == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_2_4G_OFDM_CH%d", nAnt);
    }

    return bFetchIntArrayAndCheck(
               strParmName, pRegBuf, nRegCount,
               eumParserCheckMode_Min | eumParserCheckMode_Max,
               (double)nMin, (double)nMax);
}
bool cParserDut::bGetPowRegOffSetDefault_5G_Realtek(int nAnt, int nDemode, int *pRegBuf, int nRegCount, int nMin, int nMax)
{
    string strParmName = "";

    if (eumDemod_11ag == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_5G_OFDM_CH%d", nAnt);
    }
    else if (eumDemod_11n_20M == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_5G_HT20_CH%d", nAnt);
    }
    else if (eumDemod_11ac_80M == nDemode)
    {
        strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_OFFSET_5G_AC80_CH%d", nAnt);
    }

    return bFetchIntArrayAndCheck(
               strParmName, pRegBuf, nRegCount,
               eumParserCheckMode_Min | eumParserCheckMode_Max,
               (double)nMin, (double)nMax);
}
bool cParserDut::bGetDutEfuseType(int *pRetValue)
{
    bool bRslt = true;
    vDisableErrorPrint();
    bRslt &= bFetchIntAndCheck("WT_DUT_EFUSE_TYPE",
                               pRetValue,
                               eumParserCheckMode_Min | eumParserCheckMode_Max,
                               1,
                               4);
    vEnableErrorPrint();
    return bRslt;
}

bool cParserDut::bGetCheckEnable(bool *pRetValue)
{
	int enable = 1;
	bool bRslt = true;
	bRslt &= bFetchIntAndCheck("WT_DUT_CHECK_ENABLE",
		&enable,
		eumParserCheckMode_Min | eumParserCheckMode_Max,
		false,
		true);

	if (bRslt)
		*pRetValue = (enable != 0);
	else
		*pRetValue = true;

	return bRslt;
}

bool cParserDut::bGetPowRegDefault_2_4G_Ralink(int nAnt, int *pRegBuf, int nMin, int nMax)
{
    int nFirstChannel = 1;
    int nRegCnt = 14;
    string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_CH%d", nAnt);
    bool bRslt = bFetchIntArrayAndCheck(    strParmName, &pRegBuf[nFirstChannel], nRegCnt, eumParserCheckMode_Min | eumParserCheckMode_Max, (double)nMin, (double)nMax);

    if (bRslt == false)
    {
        if (m_bIsNeedPrintErr)
        {
           // m_strError = cStringBase::strPrintf("\n   %s will all use 0.\n\n", strParmName.c_str());
           // pGetMtPrinter()->vPrintErr(strErr);
        }

        for (int i = nFirstChannel; i <nRegCnt+1; i++)
        {
            pRegBuf[i] = 0;
        }
    }

    return bRslt;
}
bool cParserDut::bGetPowRegDefault_5G_Ralink(int nAnt, int *pRegBuf, int nMin, int nMax)
{
    //分段获取5G的PowReg，5GL[36-64]有12个PowReg，5GM[100-140]有16个，5GH[149-165]有7个
    bool bRslt = true;
    string strDiff[] = {"L", "M", "H"};
    int narRegCount[] = {12, 15, 7};
    int narLocalRegBuf[eumChannelMax] = {0};
    int *pRegBuf_Tmp = narLocalRegBuf;

    for (int i = 0; i < 3; i++)
    {
        string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_CH%d", strDiff[i].c_str(), nAnt);

        if (bFetchIntArrayAndCheck(strParmName, pRegBuf_Tmp, narRegCount[i], eumParserCheckMode_Min | eumParserCheckMode_Max, (double)nMin, (double)nMax)
                == false)
        {
            if (m_bIsNeedPrintErr)
            {
              //  m_strError = cStringBase::strPrintf("\n   %s will all use 0.\n\n", strParmName.c_str());
              //  pGetMtPrinter()->vPrintErr(strErr);
            }

            for (int nRegIndex = 0; nRegIndex < narRegCount[i]; nRegIndex++)
            {
                pRegBuf_Tmp[nRegIndex] = 0;
            }

            bRslt = false;
        }

        pRegBuf_Tmp = &pRegBuf_Tmp[narRegCount[i]];
    }

    //将PowReg映射到Channel号对应的数组中
    cFreqGroupDescp imFreqGroupDescp;
    int nFreqGroup = eumFreqGroup5G;

    for (int i = 0; i < imFreqGroupDescp.nGetChannelCount(nFreqGroup); i++)
    {
        int nChannel = imFreqGroupDescp.nGetChannelInValidChannelArrary(nFreqGroup, i);
        pRegBuf[nChannel] = narLocalRegBuf[i];
    }

    return bRslt;
}

bool cParserDut::bGetPowRegDefault_5G_AC_Ralink(int nAnt, int *pRegBuf, int nMin, int nMax)
{
    //分段获取5G的PowReg，5GL[36-64]有14个PowReg，5GM[100-144]有21个，5GH[149-177]有14个
    bool bRslt = true;
    string strDiff[] = {"L", "M", "H"};
    int narRegCount[] = {14, 21, 14};
    int narLocalRegBuf[eumChannelMax] = {0};
    int *pRegBuf_Tmp = narLocalRegBuf;

    for (int i = 0; i < 3; i++)
    {
        string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_CH%d", strDiff[i].c_str(), nAnt);

        if (bFetchIntArrayAndCheck(strParmName, pRegBuf_Tmp, narRegCount[i], eumParserCheckMode_Min | eumParserCheckMode_Max, (double)nMin, (double)nMax)
                == false)
        {
            if (m_bIsNeedPrintErr)
            {
                m_strError = cStringBase::strPrintf("\n   %s will all use 0.\n\n", strParmName.c_str());
               
			   //pGetMtPrinter()->vPrintErr(strErr);
            }

            for (int nRegIndex = 0; nRegIndex < narRegCount[i]; nRegIndex++)
            {
                pRegBuf_Tmp[nRegIndex] = 0;
            }

            bRslt = false;
        }

        pRegBuf_Tmp = &pRegBuf_Tmp[narRegCount[i]];
    }

    //将PowReg映射到Channel号对应的数组中
    cFreqGroupDescp_11AC imFreqGroupDescp;
    int nFreqGroup = eumFreqGroup5G;

    for (int i = 0; i < imFreqGroupDescp.nGetChannelCount(nFreqGroup); i++)
    {
        int nChannel = imFreqGroupDescp.nGetChannelInValidChannelArrary(nFreqGroup, i);
        pRegBuf[nChannel] = narLocalRegBuf[i];
    }

    return bRslt;
}

bool cParserDut::bGetPowRegDefault_2_4G_MTK_TSSI(int nAnt,int *pRegBuf,int nGroupCont,int nMin,int nMax)
{
	string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_CH%d", nAnt);
	bool bRslt = bFetchIntArrayAndCheck(    strParmName, &pRegBuf[0], nGroupCont, eumParserCheckMode_Min | eumParserCheckMode_Max, (double)nMin, (double)nMax);

	if (bRslt == false)
	{
		for (int i = 0; i < nGroupCont; i++)
		{
			pRegBuf[i] = 0;
		}
	}

	return bRslt;
}

bool cParserDut::bGetPowRegDefault_5G_MTK_TSSI(int nAnt,int *pRegBuf,int nGroupCont,int nMin,int nMax)
{
	bool bRslt = true;
	string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G_CH%d", nAnt);

	if (bFetchIntArrayAndCheck(strParmName, pRegBuf, nGroupCont, eumParserCheckMode_Min | eumParserCheckMode_Max, (double)nMin, (double)nMax)
		== false)
	{

		for (int nRegIndex = 0; nRegIndex < nGroupCont; nRegIndex++)
		{
			pRegBuf[nRegIndex] = 0;
		}

		bRslt = false;
	}
	return bRslt;
}

bool cParserDut::bGetPowRegDefault_MTK_TSSI(int nFreqGroup,int nAnt,int nDemode,int nGroupCount,int *pRegBuf,int nMin,int nMax)
{
	bool bRslt;

	if (nFreqGroup == eumFreqGroup2_4G)
	{
		bRslt = bGetPowRegDefault_2_4G_MTK_TSSI(nAnt, pRegBuf, nGroupCount,nMin, nMax);
	}
	else
	{
		bRslt = bGetPowRegDefault_5G_MTK_TSSI(nAnt, pRegBuf, nGroupCount,nMin, nMax);
	}

	return bRslt;
}
bool cParserDut::bWritePowRegDefault_MTK_TSSI(int nFreqGroup,int nAnt,int nDemode,int nGroupIndex, int nRegValue)
{
	bool bRslt;
	if (nFreqGroup == eumFreqGroup2_4G)
	{
		bRslt = bWritePowRegDefault_2_4G_MTK_TSSI(nAnt,nGroupIndex,nRegValue);
	}
	else
	{
		bRslt = bWritePowRegDefault_5G_MTK_TSSI(nAnt,nGroupIndex,nRegValue);
	}
	return bRslt;
}

bool cParserDut::bGetPowRegDefault_Ralink(int nFreqGroup, int nAnt, int nDemode, int *pRegBuf, int nMin, int nMax)
{
    bool bRslt;

    if (nFreqGroup == eumFreqGroup2_4G)
    {
        bRslt = bGetPowRegDefault_2_4G_Ralink(nAnt, pRegBuf, nMin, nMax);
    }
    else
    {
        bRslt = bGetPowRegDefault_5G_Ralink(nAnt, pRegBuf, nMin, nMax);
    }

    return bRslt;
}

bool cParserDut::bGetPowRegDefault_AC_Ralink(int nFreqGroup, int nAnt, int nDemode, int *pRegBuf, int nMin, int nMax)
{
    bool bRslt;

    if (nFreqGroup == eumFreqGroup2_4G)
    {
        bRslt = bGetPowRegDefault_2_4G_Ralink(nAnt, pRegBuf, nMin, nMax);
    }
    else
    {
        bRslt = bGetPowRegDefault_5G_AC_Ralink(nAnt, pRegBuf, nMin, nMax);
    }

    return bRslt;
}


bool cParserDut::bWritePowRegDefault_Ralink(int nFreqGroup, int nAnt, int nDemode, int nChannel, int nRegValue)
{
    if (nFreqGroup == eumFreqGroup2_4G)
    {
        return bWritePowRegDefault_2_4G_Ralink(nAnt, nChannel, nRegValue);
    }
    else
    {
        return bWritePowRegDefault_5G_Ralink(nAnt, nChannel, nRegValue);
    }
}

bool cParserDut::bWritePowRegDefault_AC_Ralink(int nFreqGroup, int nAnt, int nDemode, int nChannel, int nRegValue)
{
    if (nFreqGroup == eumFreqGroup2_4G)
    {
        return bWritePowRegDefault_2_4G_Ralink(nAnt, nChannel, nRegValue);
    }
    else
    {
        return bWritePowRegDefault_5G_AC_Ralink(nAnt, nChannel, nRegValue);
    }
}

bool cParserDut::bWritePowRegDefault_2_4G_Ralink(int nAnt, int nChannel, int nRegValue)
{
    string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_CH%d", nAnt);
    return pGetParser()->iWriteIntArray(strParmName, nChannel - 1, nRegValue) == eumParserErr_OK;
}
bool cParserDut::bWritePowRegDefault_5G_Ralink(int nAnt, int nChannel, int nRegValue)
{
    cFreqGroupDescp imFreqGroupDescp;
    int n5GGroupNum = imFreqGroupDescp.nGet5GSubChannelGroup(nChannel);

    if ( n5GGroupNum < 0)
    {
        return false;
    }

    string strDiff[] = {"L", "M", "H"};
    string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_CH%d", strDiff[n5GGroupNum].c_str(), nAnt);
    int nRegIndex = imFreqGroupDescp.nGetIndexIn5GSubChannelGroup(nChannel);
    return pGetParser()->iWriteIntArray(strParmName, nRegIndex, nRegValue) == eumParserErr_OK;
}

bool cParserDut::bWritePowRegDefault_5G_AC_Ralink(int nAnt, int nChannel, int nRegValue)
{
    cFreqGroupDescp_11AC imFreqGroupDescp;
    int n5GGroupNum = imFreqGroupDescp.nGet5GSubChannelGroup(nChannel);

    if ( n5GGroupNum < 0)
    {
        return false;
    }

    string strDiff[] = {"L", "M", "H"};
    string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G%s_CH%d", strDiff[n5GGroupNum].c_str(), nAnt);
    int nRegIndex = imFreqGroupDescp.nGetIndexIn5GSubChannelGroup(nChannel);
    if(nRegIndex >= 0)
    {
        return pGetParser()->iWriteIntArray(strParmName, nRegIndex, nRegValue) == eumParserErr_OK;
    }
}

bool cParserDut::bWritePowRegDefault_2_4G_MTK_TSSI(int nAnt,int nGroupIndex,int nRegValue)
{
	string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_2_4G_CH%d", nAnt);
	return pGetParser()->iWriteIntArray(strParmName, nGroupIndex, nRegValue) == eumParserErr_OK;
}

bool cParserDut::bWritePowRegDefault_5G_MTK_TSSI(int nAnt,int nGroupIndex,int nRegValue)
{
	string strParmName = cStringBase::strPrintf("WT_POW_CAL_DEFAULT_REG_5G_CH%d",nAnt);
	return pGetParser()->iWriteIntArray(strParmName, nGroupIndex, nRegValue) == eumParserErr_OK;
}
/*#####################################

cParserWifiLimit类

######################################*/
bool cParserWifiLimit::bGetTargetPowDataRateDiff( int nDemode, int nMcs, double *pRetValue,int nFreqGroup)
{
    string strParmName = "";

    switch(nDemode)
    {
    case eumDemod_11ag:
    case eumDemod_11b:
    {
        strParmName = "WT_TARGET_PWR_LEGACY_RATE_DIFF";
        break;
    }

    case eumDemod_11n_20M:
    {
        if (nMcs < m_n11nMcsParmCountInOneLine)
        {
            strParmName = "WT_TARGET_PWR_HT20_1S_MCS_DIFF" ;
        }
        else
        {
            strParmName = "WT_TARGET_PWR_HT20_2S_MCS_DIFF";
        }

        break;
    }
    case eumDemod_11n_40M:
    {
        if (nMcs < m_n11nMcsParmCountInOneLine)
        {
            strParmName = "WT_TARGET_PWR_HT40_1S_MCS_DIFF";
        }
        else
        {
            strParmName = "WT_TARGET_PWR_HT40_2S_MCS_DIFF";
        }

        break;
    }

    case eumDemod_11ac_20M:
    {
        if (nMcs < m_nAC20McsParmCountInOneLine)
        {
            strParmName = "WT_TARGET_PWR_AC20_1S_MCS_DIFF";
        }
        else
        {
            strParmName = "WT_TARGET_PWR_AC20_2S_MCS_DIFF";
        }

        break;
    }
    case eumDemod_11ac_40M:
    {
        if (nMcs < m_nAC40McsParmCountInOneLine)
        {
            strParmName = "WT_TARGET_PWR_AC40_1S_MCS_DIFF";
        }
        else
        {
            strParmName = "WT_TARGET_PWR_AC40_2S_MCS_DIFF";
        }

        break;
    }
    case eumDemod_11ac_80M:
    {
        if (nMcs < m_nAC80McsParmCountInOneLine)
        {
            strParmName = "WT_TARGET_PWR_AC80_1S_MCS_DIFF";
        }
        else
        {
            strParmName = "WT_TARGET_PWR_AC80_2S_MCS_DIFF";
        }

        break;
    }
	case eumDemod_11ac_160M:
		{
			if (nMcs < m_nAC80McsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AC160_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AC160_2S_MCS_DIFF";
			}

			break;
		}
	case eumDemod_11ac_80M_80M:
		{
			if (nMcs < m_nAC80McsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AC8080_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AC8080_2S_MCS_DIFF";
			}

			break;
		}

		case eumDemod_11ax_20M:
		{
			if (nMcs < m_nAXMcsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AX20_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AX20_2S_MCS_DIFF";
			}

			break;
		}
		case eumDemod_11ax_40M:
		{
			if (nMcs < m_nAXMcsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AX40_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AX40_2S_MCS_DIFF";
			}

			break;
		}
		case eumDemod_11ax_80M:
		{
			if (nMcs < m_nAXMcsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AX80_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AX80_2S_MCS_DIFF";
			}

			break;
		}
		case eumDemod_11ax_160M:
		{
			if (nMcs < m_nAXMcsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AX160_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AX160_2S_MCS_DIFF";
			}

			break;
		}
		case eumDemod_11ax_80_80M:
		{
			if (nMcs < m_nAXMcsParmCountInOneLine)
			{
				strParmName = "WT_TARGET_PWR_AX8080_1S_MCS_DIFF";
			}
			else
			{
				strParmName = "WT_TARGET_PWR_AX8080_2S_MCS_DIFF";
			}

			break;
		}
    }


	string strParmNameTemp = "";
	if (nFreqGroup == eumFreqGroup2_4G)
	{
		strParmNameTemp = strParmName + "_2_4G";
	}
	else if (nFreqGroup == eumFreqGroup5G)
	{
		strParmNameTemp = strParmName + "_5G";
	}
	else if (nFreqGroup == eumFreqGroup6G)
	{
		strParmNameTemp = strParmName + "_6G";
	}
	else
	{
		strParmNameTemp = strParmName;
	}

	bool ret = false;
	ret = bGetMcsParm( strParmNameTemp, nDemode, nMcs, pRetValue,
		eumParserCheckMode_Min | eumParserCheckMode_Max,
		-30, 30);

	if (ret == false)
	{
		return bGetMcsParm( strParmName, nDemode, nMcs, pRetValue,
			eumParserCheckMode_Min | eumParserCheckMode_Max,
			-30, 30);
	}

	return ret;
}

bool cParserWifiLimit::bGetTargetPow(int nFreqGroup, int nDemode, double *pRetValue)  //最高MCS下的目标功率(VHT20/40/80都是MCS7)
{
    string strDemodeKeyWorld[eumDemodMax_11ac_80M];
    strDemodeKeyWorld[eumDemod_11b] = "WT_TARGET_PWR_11B";
	

    if (nFreqGroup == eumFreqGroup2_4G)
    {
        strDemodeKeyWorld[eumDemod_11n_40M] = "WT_TARGET_PWR_HT40_2_4G";
        strDemodeKeyWorld[eumDemod_11ag] = "WT_TARGET_PWR_OFDM_2_4G";
        strDemodeKeyWorld[eumDemod_11n_20M] = "WT_TARGET_PWR_HT20_2_4G";

		strDemodeKeyWorld[eumDemod_11ac_20M] = "WT_TARGET_PWR_AC20_2_4G";
		strDemodeKeyWorld[eumDemod_11ac_40M] = "WT_TARGET_PWR_AC40_2_4G";
		strDemodeKeyWorld[eumDemod_11ac_80M] = "WT_TARGET_PWR_AC80_2_4G";

    }
    else 
    {
        strDemodeKeyWorld[eumDemod_11n_40M] = "WT_TARGET_PWR_HT40_5G";
        strDemodeKeyWorld[eumDemod_11ag] = "WT_TARGET_PWR_OFDM_5G";
        strDemodeKeyWorld[eumDemod_11n_20M] = "WT_TARGET_PWR_HT20_5G";

		strDemodeKeyWorld[eumDemod_11ac_20M] = "WT_TARGET_PWR_AC20_5G";
		strDemodeKeyWorld[eumDemod_11ac_40M] = "WT_TARGET_PWR_AC40_5G";
		strDemodeKeyWorld[eumDemod_11ac_80M] = "WT_TARGET_PWR_AC80_5G";
    }

    string strKeyWord = strDemodeKeyWorld[nDemode];
    double dTargetPow = 0;

    if (bFetchDoubleAndCheck(strKeyWord, &dTargetPow, eumParserCheckMode_Min | eumParserCheckMode_Max, -100, 100)
            == false)
    {
        return false;
    }

    *pRetValue = dTargetPow;
    return true;
}

bool cParserWifiLimit::bGetPowVerifyErrRange( bool b1Up0Low, int nDemode, int nMcs, double *pRetValue )
{
	string strUpLow = "";

	if (b1Up0Low)
	{
		strUpLow = "UP";
	}
	else
	{
		strUpLow = "LOW";
	}

	string strParmName = "";

	switch(nDemode)
	{
	case eumDemod_11ag:
	case eumDemod_11b:
		{
			strParmName = "WT_TX_POWER_LEGACY_TOLERANCE_" + strUpLow;
			break;
		}

	case eumDemod_11n_20M:
		{
			if (nMcs < m_n11nMcsParmCountInOneLine)
			{
				strParmName = "WT_TX_POWER_HT20_1S_TOLERANCE_"  + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_HT20_2S_TOLERANCE_" + strUpLow;
			}

			break;
		}

	case eumDemod_11n_40M:
		{
			if (nMcs < m_n11nMcsParmCountInOneLine)
			{
				strParmName = "WT_TX_POWER_HT40_1S_TOLERANCE_" + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_HT40_2S_TOLERANCE_" + strUpLow;
			}

			break;
		}

	case eumDemod_11ac_20M:
		{
			if (nMcs < m_nAC20McsParmCountInOneLine)
			{
				strParmName = "WT_TX_POWER_AC20_1S_TOLERANCE_" + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_AC20_2S_TOLERANCE_" + strUpLow;
			}

			break;
		}

	case eumDemod_11ac_40M:
		{
			if (nMcs < m_nAC40McsParmCountInOneLine)
			{
				strParmName = "WT_TX_POWER_AC40_1S_TOLERANCE_" + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_AC40_2S_TOLERANCE_" + strUpLow;
			}

			break;
		}

	case eumDemod_11ac_80M:
		{
			if (nMcs < m_nAC80McsParmCountInOneLine)
			{
				strParmName = "WT_TX_POWER_AC80_1S_TOLERANCE_" + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_AC80_2S_TOLERANCE_" + strUpLow;
			}

			break;
		}
	}

	return bGetMcsParm( strParmName, nDemode, nMcs, pRetValue,
		eumParserCheckMode_Min | eumParserCheckMode_Max,
		0, 100);
}
bool cParserWifiLimit::bGetPowVerifyErrRange(bool b1Up0Low,int iBand, int nDemode,int nMcs,double *pRetValue)
{
	string strUpLow = "";

	if (b1Up0Low)
	{
		strUpLow = "UP";
	}
	else
	{
		strUpLow = "LOW";
	}

	string strParmName = "";

	switch(nDemode)
	{
	case eumDemod_11ag:
	case eumDemod_11b:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				strParmName = "WT_TX_POWER_LEGACY_TOLERANCE_" + strUpLow;
			}
			else if(eumFreqGroup5G == iBand)
			{
				strParmName = "WT_TX_POWER_5G_11A_TOLERANCE_" + strUpLow;
			}
			else
			{
				strParmName = "WT_TX_POWER_6G_11A_TOLERANCE_" + strUpLow;
			}
			break;
		}

	case eumDemod_11n_20M:
		{
			if (eumFreqGroup2_4G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_HT20_1S_TOLERANCE_"  + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_HT20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if(eumFreqGroup5G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_HT20_1S_TOLERANCE_"  + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_HT20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_HT20_1S_TOLERANCE_"  + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_HT20_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_HT40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_HT40_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_HT40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_HT40_2S_TOLERANCE_" + strUpLow;
				}
			}	
			else
			{
				if (nMcs < m_n11nMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_HT40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_HT40_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AC20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AC20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AC20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AC20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAC20McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AC20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AC20_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AC40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AC40_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AC40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AC40_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAC40McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AC40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AC40_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AC80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AC80_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AC80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AC80_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AC80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AC80_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AC160_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AC160_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AC160_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AC160_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AC8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AC8080_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AC8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AC8080_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAC80McsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AC8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AC8080_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AX20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AX20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AX20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AX20_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AX20_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AX20_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AX40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AX40_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AX40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AX40_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AX40_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AX40_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AX80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AX80_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AX80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AX80_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AX80_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AX80_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AX160_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AX160_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AX160_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AX160_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AX160_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AX160_2S_TOLERANCE_" + strUpLow;
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
					strParmName = "WT_TX_POWER_2_4G_AX8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_2_4G_AX8080_2S_TOLERANCE_" + strUpLow;
				}
			}
			else if (eumFreqGroup5G == iBand)
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_5G_AX8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_5G_AX8080_2S_TOLERANCE_" + strUpLow;
				}
			}
			else
			{
				if (nMcs < m_nAXMcsParmCountInOneLine)
				{
					strParmName = "WT_TX_POWER_6G_AX8080_1S_TOLERANCE_" + strUpLow;
				}
				else
				{
					strParmName = "WT_TX_POWER_6G_AX8080_2S_TOLERANCE_" + strUpLow;
				}
			}
			break;
		}
	}

	return bGetMcsParm( strParmName, nDemode, nMcs, pRetValue,
		eumParserCheckMode_Min | eumParserCheckMode_Max,
		0, 100);

}

/*#####################################

cParserExtAtten类

######################################*/
string cParserExtAtten::strGetAttenParmName( int nFreqGrp, int nAnt )
{
    char u8arBuf[128] = {0};

    switch(nFreqGrp)
    {
    case eumFreqGroup2_4G:
    {
        sprintf_s(u8arBuf, "WT_FIXED_ATTEN_2_4_CHAIN%d", nAnt);
        break;
    }

    case  eumFreqGroup5G:
    {
        sprintf_s(u8arBuf, "WT_FIXED_ATTEN_5_CHAIN%d", nAnt);
        break;
    }

	case  eumFreqGroup6G:
    {
        sprintf_s(u8arBuf, "WT_FIXED_ATTEN_6_CHAIN%d", nAnt);
        break;
    }

    case  eumFreqGroupBT:
        {
            sprintf_s(u8arBuf, "WT_FIXED_ATTEN_BT");

			if (nAnt >= 1)
			{
				sprintf_s(u8arBuf, "WT_FIXED_ATTEN_BT_CHAIN%d", nAnt);
			}
            break;
        }

	case  eumFreqGroupZigbee:
		{
			sprintf_s(u8arBuf, "WT_FIXED_ATTEN_ZIGBEE");

			if (nAnt >= 1)
			{
				sprintf_s(u8arBuf, "WT_FIXED_ATTEN_ZIGBEE_CHAIN%d", nAnt);
			}
			break;
		}

	case eumFreqGroupGPS:
		{
			sprintf_s(u8arBuf, "WT_FIXED_ATTEN_GPS");
			break;
		}

	case eumFreqGroupBDS:
		{
			sprintf_s(u8arBuf, "WT_FIXED_ATTEN_BDS");
			break;
		}
    }

    string strParmName = u8arBuf;

    return strParmName;
}

//当返回为False时，停止运行，提示用户检查配置
bool cParserExtAtten::bGetExtAtten_DiffChannel(int nFreqGrp, int nAnt, int nChannel, double *pExtAtten_DiffChannel_dB)
{
    bool bIsFindExtAttenDiffChannel = false;    //最终是否找到PowerCode修正部分
    double dExtAttenDiffChannel_dB = 0;

    string strAttenTableStart = "";
    string strAttenTableEnd = "";
    string strChannel = "";

    if (nFreqGrp == eumFreqGroup2_4G)
    {
        strAttenTableStart = "WT_DELTA_ATTEN_2_4_BEGIN";
        strAttenTableEnd = "WT_DELTA_ATTEN_2_4_END";
    }
    else if (nFreqGrp == eumFreqGroup5G)
    {
        strAttenTableStart = "WT_DELTA_ATTEN_5_BEGIN";
        strAttenTableEnd = "WT_DELTA_ATTEN_5_END";
    }
	else if (nFreqGrp  == eumFreqGroup6G)
	{
		strAttenTableStart = "WT_DELTA_ATTEN_6_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_6_END";
	}
    else if (nFreqGrp == eumFreqGroupBT)
    {
        strAttenTableStart = "WT_DELTA_ATTEN_BT_BEGIN";
        strAttenTableEnd = "WT_DELTA_ATTEN_BT_END";
    }
	else if (nFreqGrp == eumFreqGroupZigbee)
	{
		strAttenTableStart = "WT_DELTA_ATTEN_ZIGBEE_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_ZIGBEE_END";
	}
	else if (nFreqGrp == eumFreqGroupGPS || nFreqGrp == eumFreqGroupBDS)
	{
		*pExtAtten_DiffChannel_dB = 0; //只需要测一个频点，所以只要配置线衰基准值
		return true;
	}

	switch(nFreqGrp)
	{
	case eumFreqGroup2_4G:
	case eumFreqGroup5G:
	case eumFreqGroup6G:
		strChannel = cStringBase::strPrintf("CH%d", nChannel);
		break;
	case eumFreqGroupBT:
		strChannel = cStringBase::strPrintf("BT_CH%d", nChannel);
		break;
	case eumFreqGroupZigbee:
		strChannel = cStringBase::strPrintf("ZIGBEE_CH%d", nChannel);
		break;
	}
    //MARK:
    //如果能找到频点Item，但无法提取出AttenModify，都将接着在Table中找下去，直到遇到第一个可提取出AttenModify的频点Item
    //但如果遍历Table都无法提取出AttenModify时，提醒用户检查配置(返回false)
    //如果能提取出AttenModify，但该值不符合要求，提醒用户并让其修改(返回false)
    //不论何种原因，一旦找不到AttenModify,就提醒用户检查配置(返回false)

    fstream pFile;
    pFile.open(pGetParser()->strGetFileName().c_str(), ios::in);

    if (pFile.is_open())
    {
        string strLine;
        bool bIsIntoTable = false;  //遍历文件时，当前行是否处于Table中

		istreambuf_iterator<char> beg(pFile);
		istreambuf_iterator<char> end;

		string fileContent(beg, end);
	
		//Add For B31R1 判断当前是否处于加密状态,如果是才需要解密
		if (pGetContentProcessing()->isUseEndecrypt(m_cfgFileType))
		{
			//解密文件
			fileContent = pGetContentProcessing()->decryption(fileContent);
		}
		
		std::istringstream sFile(fileContent);
		//vector<string> strLineList;	
		//cStringBase::split(fileContent,"\n", strLineList);

		//for(vector<string>::const_iterator iterLine=strLineList.begin();iterLine!=strLineList.end();iterLine++)//逐行分析
		while(getline(sFile, strLine))  
        {
            //忽略 注释行,空白行
            if (cStringBase::bIsDumb(strLine, "//"))
            {
                continue;
            }

            cStringBase::vChangeIntoStandardFormat(strLine, "//");
            string strWorld = cStringBase::strGetWordFromSentence(strLine, 1);

            if (bIsIntoTable == false)
            {
                if (strAttenTableStart.compare(strWorld) == 0)  //==0表示相同
                {
                    bIsIntoTable = true;
                }
            }
            else    //进入表格中
            {
                if (strChannel.compare(strWorld) == 0)      //找到Channel Item
                {
                    string strRegCode = cStringBase::strGetWordFromSentence(strLine, 2 + nAnt);     //MARK:内部处理时，Ant是从0开始计算的

                    if (strRegCode.size())
                    {
                        if (sscanf_s(strRegCode.c_str(), "%lf", &dExtAttenDiffChannel_dB) == 1) //如果！=1:有字符，但不是double的字符串
                        {
                            bIsFindExtAttenDiffChannel = true;
                            break;  //提取成功后退出，不关心END关键字
                        }
                    }
                }
                else if (strAttenTableEnd.compare(strWorld) == 0)
                {
                    //遇到END关键字即结束遍历，忽略后面可能会出现的Table，只取第一个Table中的值
                    bIsIntoTable = false;
                    break;
                }
            }
        }

        pFile.close();
    }

    if (bIsFindExtAttenDiffChannel == false)
    {
        return false;
    }

    *pExtAtten_DiffChannel_dB = dExtAttenDiffChannel_dB;

    return true;
}

bool cParserExtAtten::bSetExtAtten_DiffChannel( int nFreqGrp, int nAnt, int nChannel, double dExtAtten_DiffChannel_dB )
{
	string strAttenTableStart = "";
	string strAttenTableEnd = "";


	string strChannel = "";
	switch(nFreqGrp)
	{
	case eumFreqGroup2_4G:
		strChannel = cStringBase::strPrintf("CH%d", nChannel);
		strAttenTableStart = "WT_DELTA_ATTEN_2_4_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_2_4_END";
		break;
	case eumFreqGroup5G:
		strChannel = cStringBase::strPrintf("CH%d", nChannel);
		strAttenTableStart = "WT_DELTA_ATTEN_5_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_5_END";
		break;
	case eumFreqGroup6G:
		strChannel = cStringBase::strPrintf("CH%d", nChannel);
		strAttenTableStart = "WT_DELTA_ATTEN_6_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_6_END";
		break;
	case eumFreqGroupBT:
		strChannel = cStringBase::strPrintf("BT_CH%d", nChannel);
		strAttenTableStart = "WT_DELTA_ATTEN_BT_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_BT_END";
		break;
	case eumFreqGroupZigbee:
		strChannel = cStringBase::strPrintf("ZIGBEE_CH%d", nChannel);
		strAttenTableStart = "WT_DELTA_ATTEN_ZIGBEE_BEGIN";
		strAttenTableEnd = "WT_DELTA_ATTEN_ZIGBEE_END";
		break;
	}
	return pGetParser()->iWriteDoubleArray(strAttenTableStart, strAttenTableEnd, strChannel, nAnt, dExtAtten_DiffChannel_dB, "%.1lf") == eumParserErr_OK;
}

bool cParserExtAtten::bGetCableVerifyToggle( int nFreqGroup, bool *pRetValue )
{
    string strParmName = strGetCableVerifyToggleName(nFreqGroup);
    return bFetchBoolean(strParmName, pRetValue);
}

bool cParserExtAtten::bSetCableVerifyToggle( int nFreqGroup, bool bValue )
{
    string strParmName = strGetCableVerifyToggleName(nFreqGroup);
    return pGetParser()->iWriteInt(strParmName, bValue);
}

string cParserExtAtten::strGetCableVerifyToggleName( int nFreqGroup )
{
    string strParmName = "WT_CALIBRATION_ATTEN_";

    if (nFreqGroup == eumFreqGroup2_4G)
    {
        strParmName += "2_4G";
    }
    else
    {
        strParmName += "5G";
    }

    return strParmName;
}

bool cParserExtAtten::bGetCableVerifyAttenNum( int *pRetValue )
{
    return bFetchIntAndCheck("WT_CALIBRATION_ATTEN_CHAIN", pRetValue, eumParserCheckMode_Enum, 0, 1 );
}


namespace ExternNamespace
{
	cParserExtAtten* g_obj = nullptr;
	extern "C" __declspec(dllexport) BOOL __stdcall CreateObject(int n) {
		if (n > 8 || n < 1)
			return false;

		char buf[256] = { 0 };
		g_obj = new cParserExtAtten(cStringBase::strPrintf((char *)"./WT_SETUP/WT_ATTEN_DUT_%d.txt", n));
		if (g_obj != nullptr)
			return true;
		else
			return false;
	}

	extern "C" __declspec(dllexport) bool __stdcall  bGetExtAtten(int nFreqGrp, int nAnt, int nChannel, double* pRetValue)
	{
		return g_obj->bGetExtAtten(nFreqGrp, nAnt, nChannel, pRetValue);
	}

	extern "C" __declspec(dllexport) bool __stdcall  bSetExtAtten(int nFreqGrp, int nAnt, double dAtten)
	{
		return g_obj->bSetExtAtten(nFreqGrp, nAnt, dAtten);
	}

	extern "C" __declspec(dllexport) BOOL __stdcall  bSetExtAtten_DiffChannel(int nFreqGrp, int nAnt, int nChannel, double dExtAtten_DiffChannel_dB)
	{
		return g_obj->bSetExtAtten_DiffChannel(nFreqGrp, nAnt, nChannel, dExtAtten_DiffChannel_dB);
	}
}