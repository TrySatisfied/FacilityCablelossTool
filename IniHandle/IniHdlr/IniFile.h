// IniFile1.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE1_H__EEB22C4C_5F38_4BB2_B850_7FC528FBB4FC__INCLUDED_)
#define AFX_INIFILE1_H__EEB22C4C_5F38_4BB2_B850_7FC528FBB4FC__INCLUDED_

#include <string>
#include <map>
#include <VECTOR>
#include <algorithm>
#include "Base64Coder.h"
#include "Regex.h"
using namespace std;

typedef struct _ST_SECTION
{
	string     strSectionName;
	vector<string >   vecItems;
	_ST_SECTION()
	{
		strSectionName.empty();
		vecItems.clear();
	}
	_ST_SECTION(const _ST_SECTION& obj)
	{
		
		strSectionName.empty();
		vecItems.clear();

		strSectionName = obj.strSectionName;
		vecItems = obj.vecItems;
	}
	_ST_SECTION& operator =(const _ST_SECTION& obj)
	{
		if (this != &obj)
		{
			
			strSectionName = obj.strSectionName;
			vecItems = obj.vecItems;
		}
		return *this;
	}
}ST_SECTION;
typedef  vector<ST_SECTION >      VEC_SECTION;	
//////////////////////////////////////////////////////////////////////////
//用于读取ini配置文件,调用顺序,fnOpen->fnParseFile->fnGetAllSections->fnGetSectionItems
//////////////////////////////////////////////////////////////////////////
class CIniFile  
{
public:
	void fnInsertSection(ST_SECTION & section);
	void fnClear();
	void fnParseFile();
	char * fnGetData();
	void fnGetScriptInVector(VEC_SECTION  & vecAllSection);
	CIniFile();
	CIniFile& operator = (const CIniFile& obj);
	virtual ~CIniFile();
	/////////////////Add by 刘永升 [10/8/2012]///////
	//函数说明:fnGetLocalFile
	//获取本地文件路径,如果strFileName为空,则返回当前文件夹路径
	//参数说明:
	//std::string strFileName
	//返回说明:
	//std::string 
	/////////////////////////////////////////////////////////////
	static string fnGetLocalFile(string strFileName);
	//////////////////////////////////////////////////////////////////////////
	//函数说明：
	//根据文件名读取文件内容
	//参数说明:
	//strFileName[IN] 文件绝对路径
	//返回说明:
	//文件存在并正常读取内容返回true，否则返回false
	//////////////////////////////////////////////////////////////////////////
	bool fnOpen(std::string strFileName,bool bCode = true);
	
	//////////////////////////////////////////////////////////////////////////
	//函数说明：
	//加载数据
	//参数说明:
	//pbData[IN] ini文件格式的数据，外面已经把数据读取出来后可以用此函数把数据加载
	//返回说明:
	//正常返回true，否则返回false
	//////////////////////////////////////////////////////////////////////////
	bool fnOpen(const char * pbData,bool bCode = true);
	
	//////////////////////////////////////////////////////////////////////////
	//函数说明：
	//把修改后的数据保存到文件，如果文件存在则删除原文件
	//参数说明:
	//strFileName[IN] 文件绝对路径
	//bCode[IN]    是否加密保存
	//返回说明:
	//保存成功返回true，否则返回false
	//////////////////////////////////////////////////////////////////////////
	bool fnSave(string strFileName,bool bCode);
	
	//////////////////////////////////////////////////////////////////////////
	//函数说明：
	//读取数据中所有段名，即[]内的数据
	//参数说明:
	//vecSections[OUT] 段名数组
	//返回说明:
	//正常返回true，否则返回false
	//////////////////////////////////////////////////////////////////////////
	bool fnGetAllSections(vector<string > & vecSections);
	//////////////////////////////////////////////////////////////////////////
	//函数说明：
	//读取数据中段名下所有项目数据
	//参数说明:
	//strSection[IN] 指定的段名称
	//vecItems[OUT] 项目名称数组
	//返回说明:
	//正常返回true，否则返回false
	//////////////////////////////////////////////////////////////////////////
	bool fnGetSectionItems(string strSection,vector<string > & vecItems);
    
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
	string fnReadString(string strSection,string strKey,string strDefault);
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
	int fnReadInt(string strSection,string strKey,int iDefault);
	
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
	bool fnWriteString(string strSection,string strKey,string strValue);
	
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
	bool fnWriteInt(string strSection,string strKey,int iValue);

	//获取异常信息
	string fnGetErrMsg();
	void fnSetErrMsg(string strSection,string strKey);
protected:
	//变量
private:
	VEC_SECTION     m_vecAllSection;
	char *          m_pszBuffer;

	CBase64Coder    m_base64Coder;

	CRegex			m_regex;

	bool			m_bCode;

	vector<string> m_vecErrMsg;
	string m_strFileName;
};

#endif // !defined(AFX_INIFILE1_H__EEB22C4C_5F38_4BB2_B850_7FC528FBB4FC__INCLUDED_)
