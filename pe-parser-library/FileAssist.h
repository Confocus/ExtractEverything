#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"

using namespace peparse;

class CFileAssist
{
public:
	CFileAssist();
	~CFileAssist();

public:

	//************************************
	// Method:    ������ѹ����ļ����Ŀ¼
	// FullName:  CFileAssist::CreateOutputDir
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const std::string & sTarFile	�������ѹ���ļ�·������Ҫ��Ϊ�˻�ȡ�ļ�������ƴ�����·��
	// Parameter: const std::string & sDstFile	����һ�����Ŀ¼�����λ�գ����ڴ���ѹ���ļ���ǰ�ļ������������Ŀ¼
	// Parameter: std::string & sOutputFile		���һ����ѹĿ¼�����ļ���/��ѹ·����ʱ������߹�ͬ����
	//************************************
	bool CreateOutputDir(const char* szTarFile, const char* szDstFile, std::string & sOutputFile);
	void CheckOutputDir(const char* szDstFile, std::string & sOutputFile);
	bool DumpToLocal(const char* buff, uint32_t uOffset, uint32_t uSize, const char *const szLocalPath) const;
	bool GetTimeStamp(std::string& sTimeStamp);
	bool CreateDir(const char *const szOutputPath) const;
	bool IsEmptyDirectory(const char *const szOutputPath) const;

private:
	bool CreateOutputDirStr(const std::string & sTarFile, const std::string & sDstFile, const bool bDefaultOutput, std::string & sOutputFile);
};