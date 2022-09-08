/*#############################################################

					   		FileBuffer.h
					
##############################################################
 Name: FileBuffer.h
 Creat: gary
 Desc: 文件缓冲
***************************************************************/
#ifndef  __FILEBUFFER_H__
#define __FILEBUFFER_H__

#include "WTConfig.h"
#include "ContentProcessing.h"

class WTCONFIG_API cFileBuffer
{
public:
	cFileBuffer()
	{
		m_strFileName = "";
		memset(&m_imLastWriteTime,0,sizeof(m_imLastWriteTime));
		m_vectFileBuffer.clear();
		InitializeCriticalSection(&m_csFileOperateProtection);
	};
	virtual ~cFileBuffer()
	{
		m_vectFileBuffer.clear();
		DeleteCriticalSection(&m_csFileOperateProtection);
	};

	void vSetFileName(string strName){m_strFileName = strName;};
	string strGetFileName(){return m_strFileName;};

	void vUpdateLastWriteTime();
	bool bIsFileChanged();

protected:
	bool bCheckFileHasChange();
	bool bFileToBuffer();
	bool bBufferToFile();

	bool bGetLastWriteTime(FILETIME *pFileTime);

	void vSetCfgFileType(int cfgType){m_cfgFileType = cfgType;};
	int iGetCfgFileType(){return m_cfgFileType;};

	cContentProcessing *pGetContentProcessing(){return &m_contentProcessing;};

	string m_strFileName;
	FILETIME m_imLastWriteTime;
	vector <string>m_vectFileBuffer;
	CRITICAL_SECTION m_csFileOperateProtection;
	ThreadSection m_vectrFileBufferMute;//多线程情况，对内存内容进行互斥操作

	int m_cfgFileType;

	cContentProcessing m_contentProcessing;
};

#endif

