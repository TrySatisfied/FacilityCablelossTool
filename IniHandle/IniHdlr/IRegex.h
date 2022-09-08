
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the REGEX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// REGEX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef REGEX_EXPORTS
#define REGEX_API __declspec(dllexport)
#else
#define REGEX_API __declspec(dllimport)
#endif
// This class is exported from the Regex.dll
#include "Regexbase.h"
///////////////////////////////////////
//函数说明:
//根据对象名称构造对象并返回指针
//参数说明:
//pszObjectName[IN]    对象名称
//返回说明:
//对象指针
///////////////////////////////////////
extern "C"
REGEX_API  void*  fnConstructObject(const char* pszObjectName);

///////////////////////////////////////
//函数说明:
//销毁析构对象
//参数说明:
//pObject[IN]    对象指针
//返回说明:
//无
///////////////////////////////////////
extern "C"
REGEX_API  void  fnDeconstructObject(void * pObject);

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
extern "C"
REGEX_API int fnSearch(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError);
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
extern "C"
REGEX_API bool fnGrep(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError);
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
extern "C"
REGEX_API bool fnRegReplace(const char *pszSource,const char* pszExpre,const char* pszNewString,CListResult ** pResult,CListResult** pError);

extern "C"
REGEX_API void fnDeleteResult(CListResult * pResult);