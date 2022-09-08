
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
//����˵��:
//���ݶ������ƹ�����󲢷���ָ��
//����˵��:
//pszObjectName[IN]    ��������
//����˵��:
//����ָ��
///////////////////////////////////////
extern "C"
REGEX_API  void*  fnConstructObject(const char* pszObjectName);

///////////////////////////////////////
//����˵��:
//������������
//����˵��:
//pObject[IN]    ����ָ��
//����˵��:
//��
///////////////////////////////////////
extern "C"
REGEX_API  void  fnDeconstructObject(void * pObject);

//////////////////////////////////////////////////////////////////////////
//����˵��:
//�ַ�������,��pszSource=abc123cde pszExpre=(\d*).* ����Ϊ[0]123cde [1]123
//����˵��:
//pszSource[IN]:Դ�ַ���
//pszExpre[IN]:������ʽ
//vecResult[OUT]:���ҵõ��Ľ��
//strError[OUT]:ʧ��ʱ���صĴ�����Ϣ
//����˵��:
//���ҳɹ�����true,����ʧ�ܷ���false
//////////////////////////////////////////////////////////////////////////
extern "C"
REGEX_API int fnSearch(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError);
//////////////////////////////////////////////////////////////////////////
//����˵��:
//�ַ���ƥ��,��pszSource=abc123cde321 pszExpre=\d* ����Ϊ[0]123  [1]321
//����˵��:
//pszSource[IN]:Դ�ַ���
//pszExpre[IN]:������ʽ
//vecResult[OUT]:ƥ��õ��Ľ��
//strError[OUT]:ʧ��ʱ���صĴ�����Ϣ
//����˵��:
//ƥ��ɹ�����true,����ʧ�ܷ���false
//////////////////////////////////////////////////////////////////////////
extern "C"
REGEX_API bool fnGrep(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError);
//////////////////////////////////////////////////////////////////////////
//����˵��:
//����ʽ�滻,�ҳ�������ʽһ�µ��ַ������滻
//����˵��:
//pszSource[IN]:Դ�ַ���
//pszExpre[IN]:������ʽ
//pszNewString[IN]:���滻�����ַ���
//strResult[OUT]:�滻��Ľ��
//strError[OUT]:ʧ��ʱ���صĴ�����Ϣ
//����˵��:
//ƥ��ɹ�����true,����ʧ�ܷ���false
//////////////////////////////////////////////////////////////////////////
extern "C"
REGEX_API bool fnRegReplace(const char *pszSource,const char* pszExpre,const char* pszNewString,CListResult ** pResult,CListResult** pError);

extern "C"
REGEX_API void fnDeleteResult(CListResult * pResult);