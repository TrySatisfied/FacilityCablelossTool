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
//�������
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
	//����
	char*         pData;
	//��һ��Ԫ��
	CListResult * pNext;
	//��ȡ��ǰ����
	char * fnGetData(){return pData;}
	//����һ������
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
    virtual int fnSearch(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError) = 0;
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
    virtual bool fnGrep(const char *pszSource,const char* pszExpre,CListResult ** pResult,CListResult** pError) = 0;
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
    virtual bool fnRegReplace(const char *pszSource,const char* pszExpre,const char* pszNewString,CListResult ** pResult,CListResult** pError) = 0;
	
	virtual void fnDeleteResult(CListResult* pListResultDel) = 0;
};

#endif // !defined(AFX_REGEXBASE_H__01B0C3BC_E049_41B3_83E5_3B498C94D66A__INCLUDED_)
