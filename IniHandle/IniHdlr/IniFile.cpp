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
//函数说明：
//根据文件名读取文件内容
//参数说明:
//strFileName[IN] 文件绝对路径
//返回说明:
//文件存在并正常读取内容返回true，否则返回false
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
	//解密
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
//函数说明：
//加载数据
//参数说明:
//pbData[IN] ini文件格式的数据，外面已经把数据读取出来后可以用此函数把数据加载
//返回说明:
//正常返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool CIniFile::fnOpen(const char * pbData,bool bCode)
{
	m_bCode = bCode;
	char *  Buffer;
	
	Buffer = new char[100];
	m_pszBuffer = new char[std::strlen(pbData)+1];
	std::memset(m_pszBuffer,0,std::strlen(pbData)+1);
	std::strcpy(m_pszBuffer,pbData);
	//解密
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
//函数说明：
//把修改后的数据保存到文件，如果文件存在则删除原文件
//参数说明:
//strFileName[IN] 文件绝对路径
//返回说明:
//保存成功返回true，否则返回false
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
			//替换回车换行符
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
//函数说明：
//读取数据中所有段名，即[]内的数据
//参数说明:
//vecSections[OUT] 段名数组
//返回说明:
//正常返回true，否则返回false
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
//函数说明：
//读取数据中段名下所有项目数据
//参数说明:
//strSection[IN] 指定的段名称
//vecItems[OUT] 项目名称数组
//返回说明:
//正常返回true，否则返回false
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
//函数说明：
//读取字符串
//参数说明:
//strSection[IN] 指定的段名称
//strKey[IN] 段名下的关键字
//strDefault[IN] 默认值
//返回说明:
//正常返回获取得到的字符串，否则返回默认值
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
//函数说明：
//读取整形数据
//参数说明:
//strSection[IN] 指定的段名称
//strKey[IN] 段名下的关键字
//iDefault[IN] 默认值
//返回说明:
//正常返回获取得到的整型数据，否则返回默认值
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
//函数说明：
//写字符串，如果没有相应的strSection或strKey，则创建
//参数说明:
//strSection[IN] 指定的段名称
//strKey[IN] 段名下的关键字
//strValue[IN] 值
//返回说明:
//正常返回true，否则返回false
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
				//找到相应的关键字
				if(m_regex.fnSearch(m_vecAllSection[i].vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					m_vecAllSection[i].vecItems[j] = strKey + " = " + strValue;
					return true;
				}
			}//for (int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)	
			//如果没有找到,则增加一个
			strTmp = strKey + " = " + strValue;
			m_vecAllSection[i].vecItems.push_back(strTmp);
			return true;
		}//if(strSection == m_vecAllSection[i].strSectionName)	
	}//for (int i = 0; i < m_vecAllSection.size(); i++)
	//如果没有相应的段,则先增加相应的段名,再增加相应的关键字
	ST_SECTION  section;
	section.strSectionName = strSection;
	strTmp = strKey + " = " + strValue;
	section.vecItems.push_back(strTmp);
	m_vecAllSection.push_back(section);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//函数说明：
//写整型数据，如果没有相应的strSection或strKey，则创建
//参数说明:
//strSection[IN] 指定的段名称
//strKey[IN] 段名下的关键字
//iValue[IN] 值
//返回说明:
//正常返回true，否则返回false
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
				//找到相应的关键字
				if(m_regex.fnSearch(m_vecAllSection[i].vecItems[j].c_str(),strReg.c_str(),vecResult,strError))
				{
					m_vecAllSection[i].vecItems[j] = strKey + " = " + strValue;
					return true;
				}
			}//for (int j = 0; j < m_vecAllSection[i].vecItems.size(); j++)	
			//如果没有找到,则增加一个
			strTmp = strKey + " = " + strValue;
			m_vecAllSection[i].vecItems.push_back(strTmp);
			return true;
		}//if(strSection == m_vecAllSection[i].strSectionName)	
	}//for (int i = 0; i < m_vecAllSection.size(); i++)
	//如果没有相应的段,则先增加相应的段名,再增加相应的关键字
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
	//文件数据,最后一个字符后加换行符,方便截取所有行
	string          strData = m_base64Coder.fnGetData();
	strData += "\n";
	strReg = "^(.*?)\\n";
	m_regex.fnGrep(strData.c_str(),strReg.c_str(),vecAllLines,strError);
	ST_SECTION  section;
	for (unsigned int i = 0; i < vecAllLines.size(); i++)
	{
		//如果是[xxx]
		strReg = "^\\[(.+?)\\]";
		vector<string> vecResult;
		if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))
		{
			//添加前一个段
			if ("" != section.strSectionName)
			{
				m_vecAllSection.push_back(section);
			}
			section.strSectionName = vecResult[1];
			section.vecItems.clear();
		}//if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))
		//如果是一般项目
		else 
		{
			//替换回车换行符
			strTmp = vecAllLines[i];
			m_regex.fnRegReplace(strTmp.c_str(),"[\r|\n]","",strTmp,strError);
			//如果为空则下一个
			if ("" != strTmp)
			{
				section.vecItems.push_back(strTmp);
			}
		}//if (m_regex.fnSearch(vecAllLines[i].c_str(),strReg.c_str(),vecResult,strError))...else
	}//for (int i = 0; i < vecAllLines.size(); i++)
	//最后一个段
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

/////////////////Add by 刘永升 [10/8/2012]///////
//函数说明:fnGetLocalFile
//获取本地文件路径,如果strFileName为空,则返回当前文件夹路径
//参数说明:
//std::string strFileName
//返回说明:
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
 函 数 名  : 
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013-3-21
    作    者   : liyao
    修改内容   : 新生成函数

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