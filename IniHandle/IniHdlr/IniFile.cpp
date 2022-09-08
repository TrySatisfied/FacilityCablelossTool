// IniFile1.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
#include "pch.h"
#include "IniFile.h"
#include <windows.h>
#include <sys\stat.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


extern "C"  __declspec(dllexport)CIniFile * CreatePlugin()
{
	CIniFile* objptr = new CIniFile();
	return objptr;
}

CIniFile::CIniFile()
{
	m_pszBuffer = 0;
}

CIniFile::~CIniFile()
{
	if (0 != m_pszBuffer)
	{
		delete[] m_pszBuffer;
		m_pszBuffer = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//�����ļ�����ȡ�ļ�����
//����˵��:
//strFileName[IN] �ļ�����·��
//����˵��:
//�ļ����ڲ�������ȡ���ݷ���true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnOpen(std::string strFileName,bool bCode)
{
	char path[128] = {0};
	GetCurrentDirectoryA(128, path);
	m_bCode = bCode;
	FILE * pFile = fopen(strFileName.c_str(),"rb");
	m_strFileName = strFileName;
	if (!pFile)
	{
		return false;
	}
	struct stat statbuf;
	fstat(fileno(pFile), &statbuf);
	m_pszBuffer = new char[statbuf.st_size+1];
	std::memset(m_pszBuffer,0,statbuf.st_size+1);
	int iSize = fread(m_pszBuffer,1,statbuf.st_size,pFile);
	m_pszBuffer[iSize] = '\0';
	fclose(pFile);
	*(m_pszBuffer+statbuf.st_size) = 0;
	//����
	m_base64Coder.fnSetData(m_pszBuffer,statbuf.st_size);
	if (m_bCode)
	{
		m_base64Coder.fnDecodeData();
	}
	fnParseFile();
	delete[] m_pszBuffer;
	m_pszBuffer = 0;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//��������
//����˵��:
//pbData[IN] ini�ļ���ʽ�����ݣ������Ѿ������ݶ�ȡ����������ô˺��������ݼ���
//����˵��:
//��������true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnOpen(const char * pbData,bool bCode)
{
	m_bCode = bCode;
	char *  Buffer;
	
	Buffer = new char[100];
	m_pszBuffer = new char[std::strlen(pbData)+1];
	std::memset(m_pszBuffer,0,std::strlen(pbData)+1);
	std::strcpy(m_pszBuffer,pbData);
	//����
	m_base64Coder.fnSetData(m_pszBuffer,(int)std::strlen(m_pszBuffer)+1);
	if (m_bCode)
	{
		m_base64Coder.fnDecodeData();
	}

	fnParseFile();
	delete[] m_pszBuffer;
	m_pszBuffer = 0;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//���޸ĺ�����ݱ��浽�ļ�������ļ�������ɾ��ԭ�ļ�
//����˵��:
//strFileName[IN] �ļ�����·��
//����˵��:
//����ɹ�����true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnSave(string strFileName,bool bCode)
{
	FILE *pFile = fopen(strFileName.c_str(),"wb");
	if (0 == pFile)
	{
		return false;
	}
	string strData;
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		ST_SECTION section = m_vecAllSection[i];
		strData += "[" + section.strSectionName + "]\r\n";
		for (unsigned int j = 0; j < section.vecItems.size(); j++)
		{
			vector<string >   vecResult;
			string			  strError;
			string			  strSrc = section.vecItems[j];
			//�滻�س����з�
			m_regex.fnRegReplace(strSrc.c_str(),"[\\r|\\n]","",strSrc,strError);
			if ("" == strSrc)
			{
				continue;
			}

			strData += strSrc + "\r\n";
		}
		strData += "\r\n";
	}
	m_base64Coder.fnSetData(strData.c_str(),(int)strData.length()+1);
	if (bCode)
	{
		m_base64Coder.fnEncodeData();
	}
 	fwrite(m_base64Coder.fnGetData(),std::strlen(m_base64Coder.fnGetData()),1,pFile);
 	fclose(pFile);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//��ȡ���������ж�������[]�ڵ�����
//����˵��:
//vecSections[OUT] ��������
//����˵��:
//��������true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnGetAllSections(vector<string > & vecSections)
{
	vecSections.clear();
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		vecSections.push_back(m_vecAllSection[i].strSectionName);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵����
//��ȡ�����ж�����������Ŀ����
//����˵��:
//strSection[IN] ָ���Ķ�����
//vecItems[OUT] ��Ŀ��������
//����˵��:
//��������true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnGetSectionItems(string strSection,vector<string > & vecItems)
{
	vecItems.clear();
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		if(strSection == m_vecAllSection[i].strSectionName)
		{
			vecItems = m_vecAllSection[i].vecItems;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//��ȡ�ַ���
//����˵��:
//strSection[IN] ָ���Ķ�����
//strKey[IN] �����µĹؼ���
//strDefault[IN] Ĭ��ֵ
//����˵��:
//�������ػ�ȡ�õ����ַ��������򷵻�Ĭ��ֵ
//////////////////////////////////////////////////////////////////////////
string CIniFile::fnReadString(string strSection,string strKey,string strDefault)
{
	string  strReturn = strDefault;
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		if(strSection == m_vecAllSection[i].strSectionName)
		{
			ST_SECTION  section = m_vecAllSection[i];
			for (unsigned int j = 0; j < section.vecItems.size(); j++)
			{
				string strReg = "^" + strKey + "\\s*=\\s*(.*?)\\s*$";
				vector<string>  vecResult;
				string strError;
				if(m_regex.fnSearch(section.vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					strReturn = vecResult[1];
					return strReturn;

				}
			}
		}
	}
	fnSetErrMsg(strSection,strKey);

	return strReturn;
}
//////////////////////////////////////////////////////////////////////////
//����˵����
//��ȡ��������
//����˵��:
//strSection[IN] ָ���Ķ�����
//strKey[IN] �����µĹؼ���
//iDefault[IN] Ĭ��ֵ
//����˵��:
//�������ػ�ȡ�õ����������ݣ����򷵻�Ĭ��ֵ
//////////////////////////////////////////////////////////////////////////
int CIniFile::fnReadInt(string strSection,string strKey,int iDefault)
{
	int  iReturn = iDefault;
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		if(strSection == m_vecAllSection[i].strSectionName)
		{
			ST_SECTION  section = m_vecAllSection[i];
			for (unsigned int j = 0; j < section.vecItems.size(); j++)
			{
				string strReg = "^" + strKey + "\\s*=\\s*(\\d*)";
				vector<string>  vecResult;
				string strError;
				if(m_regex.fnSearch(section.vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					iReturn = atoi(vecResult[1].c_str());
					return iReturn;
					
				}
			}
		}
	}
	fnSetErrMsg(strSection,strKey);
	return iReturn;
}

//////////////////////////////////////////////////////////////////////////
//����˵����
//д�ַ��������û����Ӧ��strSection��strKey���򴴽�
//����˵��:
//strSection[IN] ָ���Ķ�����
//strKey[IN] �����µĹؼ���
//strValue[IN] ֵ
//����˵��:
//��������true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnWriteString(string strSection,string strKey,string strValue)
{
	string strTmp;
	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		if(strSection == m_vecAllSection[i].strSectionName)
		{			
			for (unsigned int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)
			{
				string strReg = "^" + strKey + "\\s*=\\s*(.*?)\\s*$";
				vector<string>  vecResult;
				string strError;
				//�ҵ���Ӧ�Ĺؼ���
				if(m_regex.fnSearch(m_vecAllSection[i].vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					m_vecAllSection[i].vecItems[j] = strKey + " = " + strValue;
					return true;
				}
			}//for (int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)	
			//���û���ҵ�,������һ��
			strTmp = strKey + " = " + strValue;
			m_vecAllSection[i].vecItems.push_back(strTmp);
			return true;
		}//if(strSection == m_vecAllSection[i].strSectionName)	
	}//for (int i = 0; i < m_vecAllSection.size(); i++)
	//���û����Ӧ�Ķ�,����������Ӧ�Ķ���,��������Ӧ�Ĺؼ���
	ST_SECTION  section;
	section.strSectionName = strSection;
	strTmp = strKey + " = " + strValue;
	section.vecItems.push_back(strTmp);
	m_vecAllSection.push_back(section);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵����
//д�������ݣ����û����Ӧ��strSection��strKey���򴴽�
//����˵��:
//strSection[IN] ָ���Ķ�����
//strKey[IN] �����µĹؼ���
//iValue[IN] ֵ
//����˵��:
//��������true�����򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnWriteInt(string strSection,string strKey,int iValue)
{
	string strTmp;
	char buffer[30] = {0};
	string strValue = itoa(iValue,buffer,10);

	for (unsigned int i = 0; i < m_vecAllSection.size(); i++)
	{
		if(strSection == m_vecAllSection[i].strSectionName)
		{			
			for (unsigned int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)
			{
				string strReg = "^" + strKey + "\\s*=\\s*(.*?)\\s*$";
				vector<string>  vecResult;
				string strError;
				//�ҵ���Ӧ�Ĺؼ���
				if(m_regex.fnSearch(m_vecAllSection[i].vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					m_vecAllSection[i].vecItems[j] = strKey + " = " + strValue;
					return true;
				}
			}//for (int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)	
			//���û���ҵ�,������һ��
			strTmp = strKey + " = " + strValue;
			m_vecAllSection[i].vecItems.push_back(strTmp);
			return true;
		}//if(strSection == m_vecAllSection[i].strSectionName)	
	}//for (int i = 0; i < m_vecAllSection.size(); i++)
	//���û����Ӧ�Ķ�,����������Ӧ�Ķ���,��������Ӧ�Ĺؼ���
	ST_SECTION  section;
	section.strSectionName = strSection;
	strTmp = strKey + " = " + strValue;
	section.vecItems.push_back(strTmp);
	m_vecAllSection.push_back(section);
	return true;
}






char * CIniFile::fnGetData()
{
	return m_base64Coder.fnGetData();
}

void CIniFile::fnParseFile()
{
	m_vecAllSection.clear();
	vector<string > vecAllLines;
	vector<string > vecAllSection;
	string          strError;
	string          strReg;
	string			strTmp;
	//�ļ�����,���һ���ַ���ӻ��з�,�����ȡ������
	string          strData = m_base64Coder.fnGetData();
	strData += "\n";
	strReg = "^(.*?)\\n";
	m_regex.fnGrep(strData.c_str(),strReg.c_str(),vecAllLines,strError);
	ST_SECTION  section;
	for (unsigned int i = 0; i < vecAllLines.size(); i++)
	{
		//�����[xxx]
		strReg = "^\\[(.+?)\\]";
		vector<string> vecResult;
		if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))
		{
			//���ǰһ����
			if ("" != section.strSectionName)
			{
				m_vecAllSection.push_back(section);
			}
			section.strSectionName = vecResult[1];
			section.vecItems.clear();
		}//if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))
		//�����һ����Ŀ
		else 
		{
			//�滻�س����з�
			strTmp = vecAllLines[i];
			m_regex.fnRegReplace(strTmp.c_str(),"[\r|\n]","",strTmp,strError);
			//���Ϊ������һ��
			if ("" != strTmp)
			{
				section.vecItems.push_back(strTmp);
			}
		}//if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))...else
	}//for (int i = 0; i < vecAllLines.size(); i++)
	//���һ����
	if ("" != section.strSectionName)
	{
		m_vecAllSection.push_back(section);
	}
}

void CIniFile::fnClear()
{
	m_vecAllSection.clear();
}

void CIniFile::fnInsertSection(ST_SECTION &section)
{
	m_vecAllSection.push_back(section);
}

CIniFile& CIniFile::operator=( const CIniFile& obj )
{
	if (0 != this->m_pszBuffer)
	{
		delete this->m_pszBuffer;
	}
	this->m_pszBuffer = new char[strlen(obj.m_pszBuffer)+1];
	memset(this->m_pszBuffer,0,strlen(obj.m_pszBuffer)+1);
	strcpy(this->m_pszBuffer,obj.m_pszBuffer);
	this->m_vecAllSection = obj.m_vecAllSection;
	return * this;
}

/////////////////Add by ������ [10/8/2012]///////
//����˵��:fnGetLocalFile
//��ȡ�����ļ�·��,���strFileNameΪ��,�򷵻ص�ǰ�ļ���·��
//����˵��:
//std::string strFileName
//����˵��:
//std::string 
/////////////////////////////////////////////////////////////
string CIniFile::fnGetLocalFile( string strFileName )
{
//#define  MAX_PATH    1024
	string csResult;
	char buffer[MAX_PATH] = {0};
	unsigned long ulBufferLen = GetModuleFileNameA(NULL,buffer,MAX_PATH);
	if (0 < ulBufferLen)
	{
 		csResult = buffer;
		csResult = csResult.substr(0,csResult.rfind('\\')+1);
		if (strFileName.length())
		{
			csResult += strFileName;
		}

	}
	return csResult;
}

/*****************************************************************************
 �� �� ��  : 
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013-3-21
    ��    ��   : liyao
    �޸�����   : �����ɺ���

*****************************************************************************/
void CIniFile::fnGetScriptInVector(VEC_SECTION & vecAllSection)
{
	vecAllSection = m_vecAllSection;
}

std::string CIniFile::fnGetErrMsg()
{
	string ret = "";
	vector<string>::iterator iter = m_vecErrMsg.begin();
	for (; iter!=m_vecErrMsg.end(); iter++)
	{
		ret += iter->c_str();
	}

	return ret;
}
void CIniFile::fnSetErrMsg(string strSection,string strKey)
{
	char err[1028] = {0};
	sprintf_s(err,"read %s --> [%s] --> [%s] fail!\r\n", m_strFileName.c_str(), strSection.c_str(), strKey.c_str());

	m_vecErrMsg.push_back(err);
}