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
	// Method:    创建解压后的文件输出目录
	// FullName:  CFileAssist::CreateOutputDir
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const std::string & sTarFile	输入待解压的文件路径，主要是为了获取文件名用来拼接输出路径
	// Parameter: const std::string & sDstFile	给定一个输出目录，如果位空，则在待解压的文件当前文件夹下生成输出目录
	// Parameter: std::string & sOutputFile		输出一个解压目录，由文件名/解压路径及时间戳三者共同构成
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