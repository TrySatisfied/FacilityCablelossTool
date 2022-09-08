/*#############################################################

					   		FileBuffer模块
					
##############################################################
 Name: FileBuffer.cpp
 Creat: gary
 Date: 2013/02/4
 Tool: VS2010
 Dscpn: 文件缓冲
##############################################################*/
#include "pch.h"
#include <sstream>
#include <io.h>
#include "FileBuffer.h"


#pragma comment(lib, "CommonLib.lib")

/*#####################################

			cFileBuffer类

######################################*/
bool cFileBuffer::bGetLastWriteTime(FILETIME *pFileTime)
{
	//打开文件
	HANDLE    hFile = CreateFileA(m_strFileName.c_str(),
		GENERIC_READ ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	//如果无法获取文件信息，可能文件不存在，或者正被其他程序使用
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FILETIME imCreationTime;
	FILETIME imLastAccessTime;
	FILETIME imLastWriteTime;
	GetFileTime(	hFile,
		&imCreationTime,
		&imLastAccessTime,
		&imLastWriteTime);
	CloseHandle(hFile);
	*pFileTime = imLastWriteTime;
	return true;
};
bool cFileBuffer::bCheckFileHasChange()
{
	FILETIME imLastWriteTime;
	if (bGetLastWriteTime(&imLastWriteTime) == false)
	{
		return true;
	}

	//如果最后写入时间相同，无需更新文件缓冲
	if ((imLastWriteTime.dwHighDateTime == m_imLastWriteTime.dwHighDateTime)
		&& (imLastWriteTime.dwLowDateTime == m_imLastWriteTime.dwLowDateTime))
	{
		return false;
	}
	//更新'最后修改时间'
	m_imLastWriteTime = imLastWriteTime;
	return true;
}
//将文件内容复制到buffer中
bool cFileBuffer::bFileToBuffer()
{
	EnterCriticalSection(&m_csFileOperateProtection);

	fstream pFile;
	pFile.open(m_strFileName.c_str(), ios::in);
	if (pFile.is_open() == 0)
	{
		LeaveCriticalSection(&m_csFileOperateProtection);
		return false;
	}

	m_vectFileBuffer.clear();

	string fileContent = "";
	
	//Add For B31R1 判断当前是否处于加密状态,如果是才需要解密
	if (pGetContentProcessing()->isUseEndecrypt(m_cfgFileType))
	{
		istreambuf_iterator<char> beg(pFile);
		istreambuf_iterator<char> end;

		string fileContent(beg, end);

		//解密
		fileContent = pGetContentProcessing()->decryption(fileContent);

		std::istringstream sFile(fileContent);
		while(1)
		{
			string strLine = "";
			if (getline(sFile,strLine) == false)
			{
				break;
			}

			m_vectFileBuffer.push_back(strLine);
		}

	}
	else
	{
		while(1)
		{
			string strLine = "";
			if (getline(pFile,strLine) == false)
			{
				break;
			}
			m_vectFileBuffer.push_back(strLine);
		}
	}
	pFile.close();

	LeaveCriticalSection(&m_csFileOperateProtection);
	return true; 
}
//将buffer中的内容复制到文件中
bool cFileBuffer::bBufferToFile()
{
	EnterCriticalSection(&m_csFileOperateProtection);

	fstream pFile;
	pFile.open(m_strFileName.c_str(), ios::out);
	if(pFile.is_open() == 0)
	{
		LeaveCriticalSection(&m_csFileOperateProtection);
		return false;
	}
	pFile.clear();
	pFile.seekp(0,ios::beg);

	string bufferForFile = "";
	for (unsigned int ii=0;ii<m_vectFileBuffer.size();ii++)
	{
		//规避由于仪器API T19版本导致配置文件乱码。
		//目前的做法恢复到原始版本
		bufferForFile += m_vectFileBuffer.at(ii)+'\n';
	}

	//Add For B31R1 判断当前是否处于加密状态,如果是才需要解密
	if (pGetContentProcessing()->isUseEndecrypt(m_cfgFileType))
	{
		//加密
		bufferForFile = pGetContentProcessing()->encryption(bufferForFile);
	}
	
	pFile<<bufferForFile;
	pFile.close();

	LeaveCriticalSection(&m_csFileOperateProtection);
	return true;
}
//更新最后写入时间
void cFileBuffer::vUpdateLastWriteTime()
{
	bGetLastWriteTime(&m_imLastWriteTime);
	return;
}

bool cFileBuffer::bIsFileChanged()
{
	FILETIME imLastWriteTime;
	if (bGetLastWriteTime(&imLastWriteTime) == false)
	{
		return true;
	}

	if ((imLastWriteTime.dwHighDateTime == m_imLastWriteTime.dwHighDateTime)
		&& (imLastWriteTime.dwLowDateTime == m_imLastWriteTime.dwLowDateTime))
	{
		return false;
	}

	return true;
}
