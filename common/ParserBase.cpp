/*#############################################################

					   		�ļ��еĲ���������Ϊ
					
##############################################################
 Name: ParserBase.cpp
 Creat: gary
 Date: 2012/03/2
 Tool: VS2010
 Dscpn: ��Ϊ��--��ȡ�ļ��еĲ���,�����ļ���д�������
***************************************************************/
#include "pch.h"
#include "ParserBase.h"

/***********************************************************************
			����                                        
************************************************************************/
bool cToken::bIsMatch(char *pu8Cmd)
{
	cStringBase::trim(pu8Cmd);
	return (!strcmp(m_strName.c_str(),pu8Cmd)) && (strlen(pu8Cmd) == m_strName.length());
}


/***********************************************************************
			Fetch State                                        
************************************************************************/
cFetchState::cFetchState()
{
	vFetchStateInit("");
}
void cFetchState::vFetchStateInit(string strName)
{
	m_iLineNumber = 0;
	m_strParmName = strName;
	m_iErrCode = eumFetchErr_MissToken;

	m_dValMax = 0;
	m_dValMin = 0;
	m_iFetchType = eumFetchType_Unknow;

	m_iCheckMode = eumParserCheckMode_None;

	m_iRealElementNum = 0;
	m_iExpectedElemntNum = 0;

	m_strFetchErrDesp = "";
	m_iRealStringLen = 0;
}
void cFetchState::vSetFetchType(int iFetchType)
{
	//��valTypeΪUnknowʱ,��GetParmʱ�Զ���ֵ
	//������û�������FetchStateʱ��ʼ����ValType,���ｫ������ValType������
	if (m_iFetchType == eumFetchType_Unknow)
	{
		m_iFetchType = iFetchType;
	}
	return;
};
bool cFetchState::bIsContentValid(double dVal)
{
	if (m_iCheckMode & eumParserCheckMode_Min)
	{
		if (dVal < m_dValMin)
		{
			m_iErrCode = eumFetchErr_Min;
			return false;
		}
	}
	if (m_iCheckMode & eumParserCheckMode_Max)
	{
		if (dVal > m_dValMax)
		{
			m_iErrCode = eumFetchErr_Max;
			return false;
		}
	}
	if (m_iCheckMode & eumParserCheckMode_Enum)
	{
		if ((dVal < m_dValMin) || (dVal > m_dValMax))
		{
			m_iErrCode = eumFetchErr_Enum;
			return false;
		}
	}
	if (m_iCheckMode & eumParserCheckMode_StringLength)
	{
		//dVal�����ַ������ȣ�dValMin��dValMax��Ϊ�ַ������ȵ�����
		if ((dVal < m_dValMin) || (dVal > m_dValMax))
		{
			m_iErrCode = eumFetchErr_StringLength;
			return false;
		}
	}

	return true;
}
bool cFetchState::bIsContentValid(int iVal)
{
	return bIsContentValid((double)iVal);
}
bool cFetchState::bIsContentValid(string strReal)
{
	if (m_iCheckMode & eumParserCheckMode_StringHex)
	{
		if (bIsHexFormatValid(strReal) == false)
		{
			m_iErrCode = eumFetchErr_HexFormat;
			return false;
		}
	}
	if (m_iCheckMode & eumParserCheckMode_StringLength)
	{
		m_iRealStringLen = strReal.length();
		if (bIsContentValid((double)m_iRealStringLen) == false)
		{
			return false;
		}
	}
	return true;
}
bool cFetchState::bIsHexFormatValid(string str8Parm)
{
	char *pu8Char = (char *)str8Parm.c_str();
	for (unsigned int Parm_itor=0;Parm_itor<str8Parm.length();Parm_itor++)
	{
		if ((pu8Char[Parm_itor] >= '0') && (pu8Char[Parm_itor] <= '9'))
		{
		}
		else if ((pu8Char[Parm_itor] >= 'A') && (pu8Char[Parm_itor] <= 'F'))
		{
		}
		else if ((pu8Char[Parm_itor] >= 'a') && (pu8Char[Parm_itor] <= 'f'))
		{
		}
		else
		{
			return false;
		}
	}
	return true;
}




/***********************************************************************
			int�����                                         
************************************************************************/
bool cFileParmInt::bGetParm(char *pu8Parm)
{
	if (!pu8Parm)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissToken);
		return false;
	}
	if ((sscanf_s(pu8Parm,"%d",m_pVal)!=1))
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissContent);
		return false;
	}
	double dVal = *m_pVal;
	double dValConfirm;
	sscanf_s(pu8Parm,"%lf",&dValConfirm);
	if (dValConfirm != dVal)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_JustInt);
		return false;
	}
	
	if (m_pFetchState)
	{
		m_pFetchState->vSetFetchType(cFetchState::eumFetchType_Int);
		if (m_pFetchState->bIsNeedFetchCheck())
		{
			if (m_pFetchState->bIsContentValid(*m_pVal) == false)
			{
				return false;
			}
		}
	}

	vSetFetchErrCode(cFetchState::eumFetchErr_OK);
	return true;
};
/***********************************************************************
			double�����                                         
************************************************************************/
bool cFileParmDouble::bGetParm(char *pu8Parm)
{
	if (!pu8Parm)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissToken);
		return false;
	}

	if (sscanf_s(pu8Parm,"%lf",m_pVal) != 1)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissContent);
		return false;
	}

	if (m_pFetchState)
	{
		m_pFetchState->vSetFetchType(cFetchState::eumFetchType_Double);
		if (m_pFetchState->bIsNeedFetchCheck())
		{
			if (m_pFetchState->bIsContentValid(*m_pVal) == false)
			{
				return false;
			}
		}
	}

	vSetFetchErrCode(cFetchState::eumFetchErr_OK);
	return true;
};
/***********************************************************************
			int/double���������                                         
************************************************************************/
int cFileParmArrayBase::nGetParm(char *pu8Parm)
{
	if (m_nArraySize == 0)
	{
		return 0;
	}
	cStringBase::trim(pu8Parm);
	char *pu8Buf = pu8Parm;
	int nParmIdx=0;
	int nStrLen = strlen(pu8Buf);
	int nCharIdx=0;
	while(nCharIdx<nStrLen)
	{
		if (bIsFetch(pu8Parm,nParmIdx++) == false)
		{
			break;
		}
		if (nParmIdx >= m_nArraySize)
		{
			break;
		}
		while(!iswspace(pu8Buf[nCharIdx++]))	//Խ���ѱ����������ַ�
		{
			if (nCharIdx>=nStrLen)
			{
				break;
			}
		}
		while (iswspace(pu8Buf[nCharIdx]))	//Խ���������������Ŀո�
		{
			nCharIdx++;
		}
		pu8Parm = &pu8Buf[nCharIdx];
	}
	return nParmIdx;
}
bool cFileParmArrayBase::bGetParm(char *pu8Parm)
{
	if (!pu8Parm)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissToken);
		return false;
	}

	int nGetParmNum = nGetParm(pu8Parm);
	if (nGetParmNum != m_nArraySize)
	{
		if (nGetParmNum == 0)
		{
			vSetFetchErrCode(cFetchState::eumFetchErr_MissContent);
		}
		else
		{
			vSetFetchErrCode(cFetchState::eumFetchErr_MissElement);
			vSetFetchElementNum(m_nArraySize,nGetParmNum);
		}
		return false;
	}

	return true;
}
bool cFileParmIntArray::bGetParm(char *pu8Parm)
{
	if (cFileParmArrayBase::bGetParm(pu8Parm) == false)
	{
		return false;
	}

	if (m_pFetchState)
	{
		m_pFetchState->vSetFetchType(cFetchState::eumFetchType_Int);
		if (m_pFetchState->bIsNeedFetchCheck())
		{
			for (int i=0;i<m_nArraySize;i++)
			{
				if (m_pFetchState->bIsContentValid(m_pVal[i]) == false)
				{
					return false;
				}
			}
		}
	}

	vSetFetchErrCode(cFetchState::eumFetchErr_OK);
	return true;
}
bool cFileParmDoubleArray::bGetParm(char *pu8Parm)
{
	if (cFileParmArrayBase::bGetParm(pu8Parm) == false)
	{
		return false;
	}

	if (m_pFetchState)
	{
		m_pFetchState->vSetFetchType(cFetchState::eumFetchType_Double);
		if (m_pFetchState->bIsNeedFetchCheck())
		{
			for (int i=0;i<m_nArraySize;i++)
			{
				if (m_pFetchState->bIsContentValid(m_pVal[i]) == false)
				{
					return false;
				}
			}
		}
	}

	vSetFetchErrCode(cFetchState::eumFetchErr_OK);
	return true;
}
/***********************************************************************
			string�����                                         
************************************************************************/
bool cFileParmString::bGetParm(char *pu8Parm)
{
	if (!pu8Parm)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissToken);
		return false;
	}

	char u8Buf[512] = {0};
	strcpy_s(u8Buf,pu8Parm);
	cStringBase::trim(u8Buf);
	*m_pPtr = u8Buf;

	if (strlen(u8Buf) == 0)
	{
		vSetFetchErrCode(cFetchState::eumFetchErr_MissContent);
		return false;
	}

	if (m_pFetchState)
	{
		m_pFetchState->vSetFetchType(cFetchState::eumFetchType_String);
		if (m_pFetchState->bIsNeedFetchCheck())
		{
			if (m_pFetchState->bIsContentValid(u8Buf) == false)
			{
				return false;
			}
		}
	}

	vSetFetchErrCode(cFetchState::eumFetchErr_OK);
	return true;
};



/***********************************************************************
			cFetcherBase��                                   
************************************************************************/
//�鿴 ������ �Ƿ�ƥ��
cToken *cFetcherBase::pGetMatching(string strParmName)
{
	list <cToken *>::iterator iter;
	for (iter = m_listTokenList.begin();iter != m_listTokenList.end();iter++)
	{
		if ((*iter)->bIsMatch((char *)strParmName.c_str()))
		{
			return *iter;
		}
	}
	return NULL;
}

void cFetcherBase::vInsertTokenList(cToken *pUnit)
{
	m_listTokenList.push_front(pUnit);
	return;
}
void cFetcherBase::vRemoveParm(cToken *pUnit)
{
	m_listTokenList.remove(pUnit);
	delete pUnit;
	pUnit = NULL;
	return;
}
void cFetcherBase::vRemoveParmList()
{
	while(m_listTokenList.size())
	{
		cToken *pUnit = m_listTokenList.front();
		m_listTokenList.pop_front();
		delete pUnit;
		pUnit = NULL;
	}
};



/***********************************************************************
			��Ӳ���                                       
************************************************************************/
//��� INT ����
void cFileConfigFetcher::vAddParm(char *pu8ParmName, int *piRet,cFetchState *pState)
{
	cFileParm *pcParm = new cFileParmInt(pu8ParmName, piRet);
	pcParm->vSetFetchState(pState);
	vInsertTokenList(pcParm);
	return;
}
//��� Double ����
void cFileConfigFetcher::vAddParm(char *pu8ParmName, double *pdRet,cFetchState *pState)
{
	cFileParm *pcParm = new cFileParmDouble(pu8ParmName, pdRet);
	pcParm->vSetFetchState(pState);
	vInsertTokenList(pcParm);
	return;
}
//��� String ����
void cFileConfigFetcher::vAddParm(char *pu8ParmName, string *pstrRet,cFetchState *pState)
{
	cFileParm *pcParm = new cFileParmString(pu8ParmName, pstrRet);
	pcParm->vSetFetchState(pState);
	vInsertTokenList(pcParm);
	return;
}
//��� INT�� ����
void cFileConfigFetcher::vAddParm(char *pu8ParmName, int *pRet, int nRetCnt,cFetchState *pState)
{
	cFileParm *pcParm = new cFileParmIntArray(pu8ParmName, pRet, nRetCnt);
	pcParm->vSetFetchState(pState);
	vInsertTokenList(pcParm);
	return;
}
//��� Double�� ����
void cFileConfigFetcher::vAddParm(char *pu8ParmName, double *pRet, int nRetCnt,cFetchState *pState)
{
	cFileParm *pcParm = new cFileParmDoubleArray(pu8ParmName, pRet, nRetCnt);
	pcParm->vSetFetchState(pState);
	vInsertTokenList(pcParm);
	return;
}
/***********************************************************************
			��ȡ����                                    
************************************************************************/
//�������ļ��л�ȡ int
bool cFileConfigFetcher::bFetchInt(string strParmName,int *pRet,cFetchState *pState)
{
	vAddParm((char *)strParmName.c_str(),pRet,pState);
	return bGetConfig();
}
//�������ļ��л�ȡ double
bool cFileConfigFetcher::bFetchDouble(string strParmName,double *pRet,cFetchState *pState)
{
	vAddParm((char *)strParmName.c_str(),pRet,pState);
	return bGetConfig();
}
//�������ļ��л�ȡ string
bool cFileConfigFetcher::bFetchString(string strParmName,string *pRet,cFetchState *pState)
{
	vAddParm((char *)strParmName.c_str(),pRet,pState);
	return bGetConfig();
}
//�������ļ��ж�ȡ int ����
bool cFileConfigFetcher::bFetchIntArray(string strParmName,int *pRet,int nSz,cFetchState *pState)
{
	vAddParm((char *)strParmName.c_str(),pRet,nSz,pState);
	return bGetConfig();
}
//�������ļ��ж�ȡ double ����
bool cFileConfigFetcher::bFetchDoubleArray(string strParmName,double *pRet,int nSz,cFetchState *pState)
{
	vAddParm((char *)strParmName.c_str(),pRet,nSz,pState);
	return bGetConfig();
}
//����ȫ�����
bool cFileConfigFetcher::bGetConfig()
{
	fstream pFile;
	bool bRslt = false;
	do 
	{
		pFile.open(m_strFile.c_str(), ios::in);
		if (pFile.is_open() == 0)
		{
			break;
		}
		
		vector <string>vectFile;
		while(1)
		{
			string strLine;
			if (getline(pFile,strLine) == false)
			{
				break;
			}
			vectFile.push_back(strLine);
		}
		pFile.close();

		//���з���
		for (unsigned int iLineNumber=0; iLineNumber< vectFile.size(); iLineNumber++)
		{
			string strLine = vectFile.at(iLineNumber);
			if (cStringBase::bIsDumb(strLine,"//"))
			{
				continue;
			}

			//��'\t'�滻�ɿո�
			cStringBase::vReplaceBlankToTabInSentence(strLine);

			string strParmName = cStringBase::strGetWordBeforeEqual(strLine);
			if (strParmName.length() == 0)	//û�еȺ�
			{
				strParmName = cStringBase::strGetWordFromSentence(strLine,1);
			}
			cFileParm *pcParm = (cFileParm *)pGetMatching(strParmName);
			if (pcParm == NULL)
			{
				continue;
			}

			cStringBase::vChangeIntoStandardFormat(strLine,"//");
 			string strContent = strLine.erase(0,strParmName.length());	//ɾ����һ������

			bRslt = true;
			if (pcParm->bGetParm((char *)strContent.c_str()) == false)
			{
				if (pcParm->m_pFetchState)
				{
					pcParm->m_pFetchState->m_iLineNumber = iLineNumber+1;
				}
				bRslt = false;
				break;	//���ٲ鿴ParmList���������Parm
			}

			vRemoveParm(pcParm);	//��Parm�Ѿ��ɹ��������,�Ƴ�������Ӱ������Parm����ʱ��Ч��
			if (m_listTokenList.empty())
			{
				break;
			}
		}
	} while (0);
	
	//�Ƴ�ȫ��Parm
	vRemoveParmList();
	return bRslt;
}
//���ļ��е�ָ������д���µ�����
bool cFileConfigFetcher::bWriteSingleParm(string strParmName,string strContentNew)
{
	bool bRslt = false;
	char u8arFileRd[1024*16]={0};
	do 
	{
		fstream pFile;
		pFile.open(m_strFile.c_str(), ios::in |ios::out);
		if(pFile.is_open() == 0)
		{
			break;
		}
		pFile.read(u8arFileRd,sizeof(u8arFileRd));
		pFile.clear();
		pFile.seekp(0,ios::beg);
		strParmName = cStringBase::strTrim(strParmName);
		string strFile = u8arFileRd;
		//���з���
		while(1)
		{
			int nPosi = strFile.find("\n");
			if (nPosi == string::npos)
			{
				break;
			}

			string strLineRaw = strFile.substr(0,nPosi+1);	//��"\n"һ������
			strFile.erase(0,nPosi+1);									//��"\n"һ��ɾ��

			if (!cStringBase::bIsDumb(strLineRaw,"//"))
			{
				string strLineStandar = strLineRaw;
				cStringBase::vChangeIntoStandardFormat(strLineStandar,"//");
				string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
				if (strParmNameInFile == strParmName)
				{
					string strContentOld = cStringBase::strGetWordFromSentence(strLineStandar,2);
					if (strContentOld.length())		//ԭ����ֵ,����ԭֵ
					{
						strLineRaw.replace(strLineRaw.rfind(strContentOld),strContentOld.length(),strContentNew);
					}
					else	//ԭ��û��ֵ
					{
						nPosi = strLineRaw.find("=");	//����ԭ��Ϊ <XXX  =   //> or <XXX=>
						if (nPosi != string::npos)
						{
							strLineRaw.insert(nPosi+1," "+strContentNew);
						}
						else	//����ԭ��Ϊ <XXX //> or <XXX>
						{
							strLineRaw.insert(strParmNameInFile.length()," = " + strContentNew);
						}
					}

					bRslt = true;
				}
			}
			pFile << strLineRaw;
		}
		pFile.close();
	} while (0);
	return bRslt;
}
bool cFileConfigFetcher::bWriteInt(string strParmName,int iVal,int iRadix)
{
	char u8arBuf[128]={0};
	_itoa_s(iVal,u8arBuf,iRadix);
	string strVal = u8arBuf;
	return bWriteSingleParm(strParmName,strVal);
}
bool cFileConfigFetcher::bWriteDouble(string strParmName,double dVal)
{
	string strVal = cStringBase::strPrintf("%.2lf",dVal);
	return bWriteSingleParm(strParmName,strVal);
}
bool cFileConfigFetcher::bWriteString(string strParmName,string strContent)
{
	return bWriteSingleParm(strParmName,strContent);
}
//���ļ��е�ָ������д���µ�����,nIndex��0��ʼ����
bool cFileConfigFetcher::bWreiteIntArrayParm(string strParmName,int nIndex,int nVal)
{
	bool bRslt = false;
	char u8arFileRd[1024*16]={0};
	do 
	{
		fstream pFile;
		pFile.open(m_strFile.c_str(), ios::in |ios::out);
		if(pFile.is_open() == 0)
		{
			break;
		}
		pFile.read(u8arFileRd,sizeof(u8arFileRd));
		pFile.clear();
		pFile.seekp(0,ios::beg);
		strParmName = cStringBase::strTrim(strParmName);
		string strFile = u8arFileRd;
		//���з���
		while(1)
		{
			int nPosi = strFile.find("\n");
			if (nPosi == string::npos)
			{
				break;
			}

			string strLineRaw = strFile.substr(0,nPosi+1);	//��"\n"һ������
			strFile.erase(0,nPosi+1);						//��"\n"һ��ɾ��

			if (!cStringBase::bIsDumb(strLineRaw,"//"))
			{
				string strLineStandar = strLineRaw;
				cStringBase::vChangeIntoStandardFormat(strLineStandar,"//");
				string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
				if (strParmNameInFile == strParmName)
				{
					int nWordPosi = 0;
					if (cStringBase::bGetWordPosiInSentence(strLineRaw,nIndex+2, &nWordPosi))
					{
						char u8arBuf[1024]={0};
						sprintf_s(u8arBuf,"%d",nVal);
						string strVal = u8arBuf;
						bRslt = cStringBase::bModifyWord(strLineRaw, nIndex+2, strVal);
					}
				}
			}
			pFile << strLineRaw;
		}
		pFile.close();
	} while (0);
	return bRslt;
}


