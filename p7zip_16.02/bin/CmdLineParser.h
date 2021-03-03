#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
using namespace peparse;

class CCmdLineParser
{
public:
	CCmdLineParser();
	~CCmdLineParser();

	//-r	识别
	//-u	解压
	//-o	输出路径
	bool Parse(const uint32_t argc, char** argv);

	bool IsOnlyRecognize() const;

	const std::string& GetTargetPath() const;
	const std::string& GetOutputPath() const;

private:
	bool IsExistParam(const char* pParam) const;
	bool GetParamIndex(const char* pParam, uint32_t* pIndex) const;
	void ConvertStrPathSlash(std::string& sPath);

private:
	bool m_bRecognize;
	bool m_bExtract;
	std::string m_sTargetPath;
	std::string m_sOutputPath;
	std::string m_sTotalCmdline;
	std::vector<std::string> m_vArgv;
};

