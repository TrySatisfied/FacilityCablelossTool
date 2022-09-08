// Regexbase.h: interface for the CRegexbase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGEXBASE_H__01B0C3BC_E049_41B3_83E5_3B498C94D66A__INCLUDED_)
#define AFX_REGEXBASE_H__01B0C3BC_E049_41B3_83E5_3B498C94D66A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4996)
#include <vector>
#include <string>
using namespace std;
//结果链表
struct CListResult
{
	CListResult()
	{
		pData = 0;
		pNext = 0;
	}
	CListResult(char * pszData)
	{
		pData = new char[::strlen(pszData) + 1];
		strcpy(pData,pszData);
		pNext = 0;
	}
	~CListResult()
	{
		if (0 != pData)
		{
			delete[] pData;
			pData = 0;
		}
		if (0 != pNext)
		{
			delete pNext;
			pNext = 0;
		}
	}
	//数据
	char*         pData;
	//下一个元素
	CListResult * pNext;
	//获取当前数据
	char * fnGetData(){return pData;}
	//插入一个数据
	bool fnInsertItem(char* pszData)
	{
		CListResult ** pCur = &pNext;
		while(0 != *pCur)
		{
			pCur = &((*pCur)->pNext);
		}
		*pCur = new CListResult(pszData);
		return true;
	}
	char * fnSetData(char * pszData)
	{
		pData = new char[::strlen(pszData) + 1];
		::strcpy(pData,pszData);
		return pData;
	}
};
class IRegexbase  
{
public:
	virtual ~IRegexbase(){};
	//////////////////////////////////////////////////////////////////////////
    //函数说明:
    //字符串查找,如pszSource=abc123cde pszExpre=(\d*).* 则结果为[0]123cde [1]123
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //vecResult[OUT]:查找得到的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //查找成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual int fnSearch(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError) = 0;
    //////////////////////////////////////////////////////////////////////////
    //函数说明:
    //字符串匹配,如pszSource=abc123cde321 pszExpre=\d* 则结果为[0]123  [1]321
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //vecResult[OUT]:匹配得到的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //匹配成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual bool fnGrep(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError) = 0;
    //////////////////////////////////////////////////////////////////////////
    //函数说明:
    //正则式替换,找出与正则式一致的字符串并替换
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //pszNewString[IN]:被替换的新字符串
    //strResult[OUT]:替换后的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //匹配成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual bool fnRegReplace(const char *pszSource,const char* pszExpre,const char* pszNewString,CListResult ** pResult,CListResult** pError) = 0;
	
	virtual void fnDeleteResult(CListResult* pListResultDel) = 0;
};

#endif // !defined(AFX_REGEXBASE_H__01B0C3BC_E049_41B3_83E5_3B498C94D66A__INCLUDED_)
