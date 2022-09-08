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
//���ڶ�ȡini�����ļ�,����˳��,fnOpen->fnParseFile->fnGetAllSections->fnGetSectionItems
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
	/////////////////Add by ������ [10/8/2012]///////
	//����˵��:fnGetLocalFile
	//��ȡ�����ļ�·��,���strFileNameΪ��,�򷵻ص�ǰ�ļ���·��
	//����˵��:
	//std::string strFileName
	//����˵��:
	//std::string 
	/////////////////////////////////////////////////////////////
	static string fnGetLocalFile(string strFileName);
	//////////////////////////////////////////////////////////////////////////
	//����˵����
	//�����ļ�����ȡ�ļ�����
	//����˵��:
	//strFileName[IN] �ļ�����·��
	//����˵��:
	//�ļ����ڲ�������ȡ���ݷ���true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool fnOpen(std::string strFileName,bool bCode = true);
	
	//////////////////////////////////////////////////////////////////////////
	//����˵����
	//��������
	//����˵��:
	//pbData[IN] ini�ļ���ʽ�����ݣ������Ѿ������ݶ�ȡ����������ô˺��������ݼ���
	//����˵��:
	//��������true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool fnOpen(const char * pbData,bool bCode = true);
	
	//////////////////////////////////////////////////////////////////////////
	//����˵����
	//���޸ĺ�����ݱ��浽�ļ�������ļ�������ɾ��ԭ�ļ�
	//����˵��:
	//strFileName[IN] �ļ�����·��
	//bCode[IN]    �Ƿ���ܱ���
	//����˵��:
	//����ɹ�����true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool fnSave(string strFileName,bool bCode);
	
	//////////////////////////////////////////////////////////////////////////
	//����˵����
	//��ȡ���������ж�������[]�ڵ�����
	//����˵��:
	//vecSections[OUT] ��������
	//����˵��:
	//��������true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool fnGetAllSections(vector<string > & vecSections);
	//////////////////////////////////////////////////////////////////////////
	//����˵����
	//��ȡ�����ж�����������Ŀ����
	//����˵��:
	//strSection[IN] ָ���Ķ�����
	//vecItems[OUT] ��Ŀ��������
	//����˵��:
	//��������true�����򷵻�false
	//////////////////////////////////////////////////////////////////////////
	bool fnGetSectionItems(string strSection,vector<string > & vecItems);
    
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
	string fnReadString(string strSection,string strKey,string strDefault);
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
	int fnReadInt(string strSection,string strKey,int iDefault);
	
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
	bool fnWriteString(string strSection,string strKey,string strValue);
	
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
	bool fnWriteInt(string strSection,string strKey,int iValue);

	//��ȡ�쳣��Ϣ
	string fnGetErrMsg();
	void fnSetErrMsg(string strSection,string strKey);
protected:
	//����
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
