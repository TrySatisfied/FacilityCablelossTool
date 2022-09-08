/*#############################################################

					   		ConfigFile模块
					
##############################################################
 Name: ConfigFile.cpp
 Creat: gary
 Date: 2013/02/20
 Tool: VS2010
 Dscpn: 配置文件
##############################################################*/
#include "pch.h"
#include "ConfigFile.h"



/*#####################################

			cConfigFile类

######################################*/
bool cConfigFile::bUpdate()
{
	if (bCheckFileHasChange())
	{
		if (bFileToBuffer() == false)
		{
			return false;
		}
		m_mapContentMap.clear();
	}
	return true;
}
bool cConfigFile::bGetContentFromFileBuffer(string strParmName,cParmContent *pContent)
{
	bool bRslt = false;
	//逐行分析
	for (unsigned int iLineNumber=0; iLineNumber< m_vectFileBuffer.size(); iLineNumber++)
	{
		string strLine = m_vectFileBuffer.at(iLineNumber);
		if (cStringBase::bIsDumb(strLine,m_strCommentToken))
		{
			continue;
		}

		cStringBase::vChangeIntoStandardFormat(strLine,m_strCommentToken);
		string strFirstWord = cStringBase::strGetWordFromSentence(strLine,1);
		if (strFirstWord != strParmName)
		{
			continue;
		}

		string strContent = strLine.erase(0,strFirstWord.length());	//删除第一个单词
		strContent = cStringBase::strTrim(strContent);
		cParmContent imContent;
		imContent.m_strContent = strContent;
		imContent.m_nLineNum = iLineNumber+1;
		*pContent = imContent;
		bRslt = true;
		break;
	}
	return bRslt;
}
bool cConfigFile::bGetContentFromMap(string strParmName,cParmContent *pContent)
{
	bool bResult = false;
	map<string, cParmContent>::iterator iter;
	iter = m_mapContentMap.find(strParmName);
	if(iter != m_mapContentMap.end())
	{
		*pContent = iter->second;
		bResult = true;
	}
	return bResult;
}
void cConfigFile::vWriteContentToMap(string strParmName,cParmContent imContent)
{
	map<string, cParmContent>::iterator iter;
	iter = m_mapContentMap.find(strParmName);
	if(iter != m_mapContentMap.end())
	{
		m_mapContentMap.at(strParmName) = imContent;
	}
	else
	{
		m_mapContentMap.insert(pair<string, cParmContent>(strParmName,imContent));
	}
	return;
}
bool cConfigFile::bGetContent(string strParmName,cParmContent *pContent)
{
	bool bResult = false;
	bUpdate();
	strParmName = cStringBase::strTrim(strParmName);
	if (bGetContentFromMap(strParmName,pContent))
	{
		bResult  = true;
		return bResult;
	}
	if (bGetContentFromFileBuffer(strParmName,pContent))
	{
		m_mapContentMap.insert(pair<string, cParmContent>(strParmName,*pContent));
		return true;
	}
	return bResult;
}
int cConfigFile::iReadInt(string strParmName,int *pRetVal,int *pLineNum)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	cParmContent imContent;
	if (bGetContent(strParmName,&imContent) == false)
	{
		return eumParserErr_MissToken;
	}
	*pLineNum = imContent.m_nLineNum;

	int iValue;
	if ((sscanf_s((char *)imContent.m_strContent.c_str(),"%d",&iValue)!=1))
	{
		return eumParserErr_MissContent;
	}
	string strValConfirm = cStringBase::strPrintf("%d",iValue);
	if (strValConfirm.length() != imContent.m_strContent.length())
	{
		return eumParserErr_IntFormat;
	}

	*pRetVal = iValue;
	return eumParserErr_OK;
}
int cConfigFile::iReadDouble(string strParmName,double *pRetVal,int *pLineNum)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	cParmContent imContent;
	if (bGetContent(strParmName,&imContent) == false)
	{
		return eumParserErr_MissToken;
	}
	*pLineNum = imContent.m_nLineNum;

	double dValue;
	if ((sscanf_s((char *)imContent.m_strContent.c_str(),"%lf",&dValue)!=1))
	{
		return eumParserErr_MissContent;
	}

	*pRetVal = dValue;
	return eumParserErr_OK;
}
int cConfigFile::iReadString(string strParmName,string *pRetVal,int *pLineNum)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	cParmContent imContent;
	if (bGetContent(strParmName,&imContent) == false)
	{
		return eumParserErr_MissToken;
	}
	*pLineNum = imContent.m_nLineNum;
	if (imContent.m_strContent.length() == 0)
	{
		return eumParserErr_MissContent;
	}
	*pRetVal = imContent.m_strContent;
	return eumParserErr_OK;
}
int cConfigFile::iReadIntArray(string strParmName,int nExpectElementNum,int *pRetArray,int *pElementNum,int *pLineNum)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	cParmContent imContent;
	if (bGetContent(strParmName,&imContent) == false)
	{
		return eumParserErr_MissToken;
	}
	*pLineNum = imContent.m_nLineNum;
	if (imContent.m_strContent.length() == 0)
	{
		return eumParserErr_MissContent;
	}
	if (nExpectElementNum == 0)
	{
		return eumParserErr_OK;
	}

	char *pu8Pointer = (char *)imContent.m_strContent.c_str();
	char *pu8Content = (char *)imContent.m_strContent.c_str();
	int nElementIndex=0;
	int nStrLen = imContent.m_strContent.length();
	int nCharIdx=0;
	while(nCharIdx<nStrLen)
	{
		int iValue=0;
		if (sscanf_s(pu8Pointer,"%d",&iValue) != 1)
		{
			break;
		}
		pRetArray[nElementIndex] = iValue;
		nElementIndex++;
		if (nElementIndex >= nExpectElementNum)
		{
			break;
		}
		while(!iswspace(pu8Content[nCharIdx++]))	//越过已被分析过的字符
		{
			if (nCharIdx>=nStrLen)
			{
				break;
			}
		}
		while (iswspace(pu8Content[nCharIdx]))	//越过两个参数间多余的空格
		{
			nCharIdx++;
		}
		pu8Pointer = &pu8Content[nCharIdx];
	}
	*pElementNum = nElementIndex;
	if (nElementIndex != nExpectElementNum)
	{
		return eumParserErr_MissElement;
	}
	return eumParserErr_OK;
}
int cConfigFile::iReadDoubleArray(string strParmName,int nExpectElementNum,double *pRetArray,int *pElementNum,int *pLineNum)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	cParmContent imContent;
	if (bGetContent(strParmName,&imContent) == false)
	{
		return eumParserErr_MissToken;
	}
	*pLineNum = imContent.m_nLineNum;
	if (imContent.m_strContent.length() == 0)
	{
		return eumParserErr_MissContent;
	}
	if (nExpectElementNum == 0)
	{
		return eumParserErr_OK;
	}

	char *pu8Pointer = (char *)imContent.m_strContent.c_str();
	char *pu8Content = (char *)imContent.m_strContent.c_str();
	int nElementIndex=0;
	int nStrLen = imContent.m_strContent.length();
	int nCharIdx=0;
	while(nCharIdx<nStrLen)
	{
		double dValue=0;
		if (sscanf_s(pu8Pointer,"%lf",&dValue) != 1)
		{
			break;
		}
		pRetArray[nElementIndex] = dValue;
		nElementIndex++;
		if (nElementIndex >= nExpectElementNum)
		{
			break;
		}
		while(!iswspace(pu8Content[nCharIdx++]))	//越过已被分析过的字符
		{
			if (nCharIdx>=nStrLen)
			{
				break;
			}
		}
		while (iswspace(pu8Content[nCharIdx]))	//越过两个参数间多余的空格
		{
			nCharIdx++;
		}
		pu8Pointer = &pu8Content[nCharIdx];
	}
	*pElementNum = nElementIndex;
	if (nElementIndex != nExpectElementNum)
	{
		return eumParserErr_MissElement;
	}
	return eumParserErr_OK;
}
int cConfigFile::iReadBool(string strParmName,bool *pRetVal,int *pLineNum)
{
	//不需要加锁。Int处已经有锁
	int iRetval=0;
	int iRslt = iReadInt(strParmName,&iRetval,pLineNum);
	if (iRslt != eumParserErr_OK)
	{
		if (iRslt == eumParserErr_IntFormat)
		{
			iRslt = eumParserErr_BoolFormat;
		}
		return iRslt;
	}
	if ((iRetval == 0) || (iRetval == 1))
	{
		*pRetVal = (iRetval == 1);
	}
	else
	{
		return eumParserErr_BoolFormat;
	}
	return eumParserErr_OK;
}
int cConfigFile::iReadEnum(string strParmName,int nFirstEnum,int nLastEnum,int *pRetVal,int *pLineNum)
{
	//不需要加锁。Int处已经有锁
	int iRetval=0;
	int iRslt = iReadInt(strParmName,&iRetval,pLineNum);
	if (iRslt != eumParserErr_OK)
	{
		if (iRslt == eumParserErr_IntFormat)
		{
			iRslt = eumParserErr_EnumFormat;
		}
		return iRslt;
	}
	if ((iRetval >= nFirstEnum) || (iRetval <= nLastEnum))
	{
		*pRetVal = iRetval;
	}
	else
	{
		return eumParserErr_OutOfEnum;
	}
	return eumParserErr_OK;
}
int cConfigFile::iWriteInt(string strParmName,int iVal,int iRadix)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	char u8arBuf[128]={0};
	_itoa_s(iVal,u8arBuf,iRadix);
	string strVal = u8arBuf;
	return iWriteNumber(strParmName,strVal);
}
int cConfigFile::iWriteDouble(string strParmName,double dVal, string strFormat)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	string strVal = cStringBase::strPrintf((char *)strFormat.c_str(),dVal);
	return iWriteNumber(strParmName,strVal);
}
int cConfigFile::iWriteString(string strParmName,string strContent)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);
	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}

	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	strContent = cStringBase::strTrim(strContent);
	unsigned int  nLineNumber;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);

		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken) == false)
		{
			string strLineStandar = strLineRaw;
			cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
			string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
			if (strParmNameInFile == strParmName)
			{
				string strContentOld_First = cStringBase::strGetWordFromSentence(strLineStandar,2);
				if (strContentOld_First.length())		//原来有值,代替原值
				{
					string strContentRaw = strLineRaw.substr(strParmName.length(),strLineRaw.length()-strParmName.length());
					int nContentOldStartPosi = strContentRaw.find(strContentOld_First) + strParmName.length();
					int nReplaceLength=0;
					int nCommentTokenPosi = strLineRaw.find(m_strCommentToken);
					if (nCommentTokenPosi == string::npos)	//后面没有注释，后面的内容都替换成新的
					{
						nReplaceLength = strLineRaw.length() - nContentOldStartPosi;
					}
					else	//后面有注释，保留注释并保留注释前的空格
					{
						for (int i=nCommentTokenPosi-1;
							i>=nContentOldStartPosi+strContentOld_First.length();
							i--)
						{
							if ((strLineRaw.at(i) == ' ')
								|| (strLineRaw.at(i) == '\t'))
							{
								nCommentTokenPosi--;
							}
							else
							{
								break;
							}
						}
						nReplaceLength = nCommentTokenPosi - nContentOldStartPosi;
					}
					strLineRaw.replace(nContentOldStartPosi,nReplaceLength,strContent);
				}
				else	//原来没有值
				{
					int nPosi = strLineRaw.find("=");	//假设原来为 <XXX  =   //> or <XXX=>
					if (nPosi != string::npos)
					{
						strLineRaw.insert(nPosi+1," "+strContent);
					}
					else	//假设原来为 <XXX //> or <XXX>
					{
						strLineRaw.insert(strParmNameInFile.length()," = " + strContent);
					}
				}
				m_vectFileBuffer.at(nLineNumber) = strLineRaw;
				iRslt = eumParserErr_OK;
				break;	//只写了文件中第一个遇到的配置项
			}
		}
	}
	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContent;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}
//向文件中的指定参数写入新的内容,nIndex从0开始计数
int cConfigFile::iWriteIntArray(string strParmName,int nIndex,int iValue)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);
	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}

	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	string strContentNew;
	unsigned int  nLineNumber;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);
		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken))
		{
			continue;
		}

		string strLineStandar = strLineRaw;
		cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
		string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
		if (strParmNameInFile == strParmName)
		{
			iRslt = eumParserErr_MissElement;
			int nWordPosi = 0;
			if (cStringBase::bGetWordPosiInSentence(strLineRaw,nIndex+2, &nWordPosi))
			{
				 string strVal = cStringBase::strPrintf("%d",iValue);
				if (cStringBase::bModifyWord(strLineRaw, nIndex+2, strVal))
				{
					strContentNew = strLineStandar.substr(strParmNameInFile.length());
					strContentNew = cStringBase::strTrim(strContentNew);
					m_vectFileBuffer.at(nLineNumber) = strLineRaw;
					iRslt = eumParserErr_OK;
				}
			}
			break;
		}
	}

	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContentNew;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}
int cConfigFile::iWriteDoubleArray(string strParmName,int nIndex,double dValue, string strFormat)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}

	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	string strContentNew;
	unsigned int  nLineNumber;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);
		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken))
		{
			continue;
		}

		string strLineStandar = strLineRaw;
		cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
		string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
		if (strParmNameInFile == strParmName)
		{
			iRslt = eumParserErr_MissElement;
			int nWordPosi = 0;
			if (cStringBase::bGetWordPosiInSentence(strLineRaw,nIndex+2, &nWordPosi))
			{
				string strVal = cStringBase::strPrintf((char *)strFormat.c_str(), dValue);
				if (cStringBase::bModifyWord(strLineRaw, nIndex+2, strVal))
				{
					strContentNew = strLineStandar.substr(strParmNameInFile.length());
					strContentNew = cStringBase::strTrim(strContentNew);
					m_vectFileBuffer.at(nLineNumber) = strLineRaw;
					iRslt = eumParserErr_OK;
				}
			}
			break;
		}
	}

	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContentNew;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}
int cConfigFile::iWriteStringArray(string strParmName,int nIndex,string strContent)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);
	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}

	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	string strContentNew;
	unsigned int  nLineNumber;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);
		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken))
		{
			continue;
		}

		string strLineStandar = strLineRaw;
		cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
		string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
		if (strParmNameInFile == strParmName)
		{
			iRslt = eumParserErr_MissElement;
			int nWordPosi = 0;
			if (cStringBase::bGetWordPosiInSentence(strLineRaw,nIndex+2, &nWordPosi))
			{
				if (cStringBase::bModifyWord(strLineRaw, nIndex+2, strContent))
				{
					strContentNew = strLineStandar.substr(strParmNameInFile.length());
					strContentNew = cStringBase::strTrim(strContentNew);
					m_vectFileBuffer.at(nLineNumber) = strLineRaw;
					iRslt = eumParserErr_OK;
				}
			}
			break;
		}
	}

	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContentNew;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}

int cConfigFile::iWriteDoubleArray(string rangeStart, string rangeEnd, string strParmName,int nIndex,double dValue, string strFormat)
{
	AutoThreadSection aSection(&m_vectrFileBufferMute);

	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}

	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	string strContentNew;
	unsigned int  nLineNumber;
	bool isIntoRange = false;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);
		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken))
		{
			continue;
		}

		if (!isIntoRange)
		{
			if (strLineRaw.find(rangeStart) != string::npos)
			{
				isIntoRange = true;
				continue;
			}
		}
		else
		{
			if (strLineRaw.find(rangeEnd) != string::npos)
			{
				isIntoRange = false;
				break;
			}

			string strLineStandar = strLineRaw;
			cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
			string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
			if (strParmNameInFile == strParmName)
			{
				iRslt = eumParserErr_MissElement;
				int nWordPosi = 0;
				if (cStringBase::bGetWordPosiInSentence(strLineRaw,nIndex+2, &nWordPosi))
				{
					string strVal = cStringBase::strPrintf((char *)strFormat.c_str(), dValue);
					if (cStringBase::bModifyWord(strLineRaw, nIndex+2, strVal))
					{
						strContentNew = strLineStandar.substr(strParmNameInFile.length());
						strContentNew = cStringBase::strTrim(strContentNew);
						m_vectFileBuffer.at(nLineNumber) = strLineRaw;
						iRslt = eumParserErr_OK;
					}
				}
				break;
			}
		}
	}

	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContentNew;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}

void cConfigFile::vGetMapFromBuffer(map<string,string> &fileMap)
 {
	 bUpdate();

	 for (unsigned int iLineNumber=0; iLineNumber< m_vectFileBuffer.size(); iLineNumber++)
	 {
		 string strLine = m_vectFileBuffer.at(iLineNumber);
		 if (cStringBase::bIsDumb(strLine,m_strCommentToken))
		 {
			 continue;
		 }

		 if (strLine.empty())
		 {
			 continue;
		 }
		 cStringBase::vChangeIntoStandardFormat(strLine,m_strCommentToken);
		 string strFirstWord = cStringBase::strGetWordFromSentence(strLine,1);
		
		 string strContent = strLine.erase(0,strFirstWord.length());	//删除第一个单词
		 strContent = cStringBase::strTrim(strContent);

		 fileMap[strFirstWord] = strContent;	 
	 }
	 return;
 }

//向文件中的指定参数写入新的内容
int cConfigFile::iWriteNumber(string strParmName,string strContentNew)
{
	if (bUpdate() == false)
	{
		return eumParserErr_FileOperate;
	}
	
	int iRslt = eumParserErr_MissToken;
	strParmName = cStringBase::strTrim(strParmName);
	strContentNew = cStringBase::strTrim(strContentNew);
	unsigned int  nLineNumber;
	//逐行分析
	for (nLineNumber=0;nLineNumber<m_vectFileBuffer.size();nLineNumber++)
	{
		string strLineRaw = m_vectFileBuffer.at(nLineNumber);

		if (cStringBase::bIsDumb(strLineRaw,m_strCommentToken) == false)
		{
			string strLineStandar = strLineRaw;
			cStringBase::vChangeIntoStandardFormat(strLineStandar,m_strCommentToken);
			string strParmNameInFile = cStringBase::strGetWordFromSentence(strLineStandar,1);
			if (strParmNameInFile == strParmName)
			{
				string strContentOld = cStringBase::strGetWordFromSentence(strLineStandar,2);
				if (strContentOld.length())		//原来有值,代替原值
				{
					strLineRaw.replace(strLineRaw.rfind(strContentOld),strContentOld.length(),strContentNew);
				}
				else	//原来没有值
				{
					int nPosi = strLineRaw.find("=");	//假设原来为 <XXX  =   //> or <XXX=>
					if (nPosi != string::npos)
					{
						strLineRaw.insert(nPosi+1," "+strContentNew);
					}
					else	//假设原来为 <XXX //> or <XXX>
					{
						strLineRaw.insert(strParmNameInFile.length()," = " + strContentNew);
					}
				}
				m_vectFileBuffer.at(nLineNumber) = strLineRaw;
				iRslt = eumParserErr_OK;
				break;	//只写了文件中第一个遇到的配置项
			}
		}
	}
	if (iRslt == eumParserErr_OK)
	{
		cParmContent imContent;
		imContent.m_strContent = strContentNew;
		imContent.m_nLineNum = nLineNumber+1;
		vWriteContentToMap(strParmName,imContent);

		if (bBufferToFile() == false)
		{
			iRslt = eumParserErr_FileOperate;
		}
		vUpdateLastWriteTime();
		//m_needWriteFile = true;
	}
	return iRslt;
}
