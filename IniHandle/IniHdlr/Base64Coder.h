// Base64Coder.h: interface for the CBase64Coder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64CODER_H__25C88D07_2BC9_40FF_B64C_4589B3FE5FA1__INCLUDED_)
#define AFX_BASE64CODER_H__25C88D07_2BC9_40FF_B64C_4589B3FE5FA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBase64Coder  
{
public:
	bool fnDecodeData();
	bool fnEncodeData();
	char* fnGetData();
	void fnSetData(const char* pData,int iLen);
	CBase64Coder();
	virtual ~CBase64Coder();
private:
	char * m_pData;
};


unsigned char* base64Decode(char* in, unsigned int& resultSize, bool trimTrailingZeros = true);   
// returns a newly allocated array - of size "resultSize" - that   
// the caller is responsible for delete[]ing.   

char* base64Encode(char const* orig, unsigned origLength); 
#endif // !defined(AFX_BASE64CODER_H__25C88D07_2BC9_40FF_B64C_4589B3FE5FA1__INCLUDED_)
