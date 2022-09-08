/*#############################################################

					   		FileBufferģ��
					
##############################################################
 Name: FileBuffer.cpp
 Creat: gary
 Date: 2013/02/4
 Tool: VS2010
 Dscpn: �ļ�����
##############################################################*/
#include "pch.h"
#include <sstream>
#include <io.h>
#include "FileBuffer.h"


#pragma comment(lib, "CommonLib.lib")

/*#####################################

			cFileBuffer��

######################################*/
bool cFileBuffer::bGetLastWriteTime(FILETIME *pFileTime)
{
	//���ļ�
	HANDLE    hFile = CreateFileA(m_strFileName.c_str(),
		GENERIC_READ ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	//����޷���ȡ�ļ���Ϣ�������ļ������ڣ�����������������ʹ��
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

	//������д��ʱ����ͬ����������ļ�����
	if ((imLastWriteTime.dwHighDateTime == m_imLastWriteTime.dwHighDateTime)
		&& (imLastWriteTime.dwLowDateTime == m_imLastWriteTime.dwLowDateTime))
	{
		return false;
	}
	//����'����޸�ʱ��'
	m_imLastWriteTime = imLastWriteTime;
	return true;
}
//���ļ����ݸ��Ƶ�buffer��
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
	
	//Add For B31R1 �жϵ�ǰ�Ƿ��ڼ���״̬,����ǲ���Ҫ����
	if (pGetContentProcessing()->isUseEndecrypt(m_cfgFileType))
	{
		istreambuf_iterator<char> beg(pFile);
		istreambuf_iterator<char> end;

		string fileContent(beg, end);

		//����
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
//��buffer�е����ݸ��Ƶ��ļ���
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
		//�����������API T19�汾���������ļ����롣
		//Ŀǰ�������ָ���ԭʼ�汾
		bufferForFile += m_vectFileBuffer.at(ii)+'\n';
	}

	//Add For B31R1 �жϵ�ǰ�Ƿ��ڼ���״̬,����ǲ���Ҫ����
	if (pGetContentProcessing()->isUseEndecrypt(m_cfgFileType))
	{
		//����
		bufferForFile = pGetContentProcessing()->encryption(bufferForFile);
	}
	
	pFile<<bufferForFile;
	pFile.close();

	LeaveCriticalSection(&m_csFileOperateProtection);
	return true;
}
//�������д��ʱ��
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
