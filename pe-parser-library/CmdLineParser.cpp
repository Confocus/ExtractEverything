#include "CmdLineParser.h"
#include <vector>
#include <algorithm>

CCmdLineParser::CCmdLineParser():
	m_bRecognize(false),
	m_bExtract(false)
{
}

CCmdLineParser::~CCmdLineParser()
{
}

bool CCmdLineParser::Parse(const uint32_t argc, char** argv)
{
	CHECK_NULLPTR_ERR(argv);
	if (argc < 3)
	{
		printf("Not enough parameters..\n");
		return false;
	}

	for (uint32_t i = 0; i < argc; i++)
	{
		std::string sArgv = *(argv + i);
		m_vArgv.push_back(sArgv);
	}

	//todo：命令行将\解析为转义字符导致-x后面的-o解析不出来。只需要把引号前面的\删除即可
	if (IsExistParam("-r") && !IsExistParam("-x") && !IsExistParam("-o"))
	{
		m_bRecognize = true;
		uint32_t uIndex = 0;
		GetParamIndex("-r", &uIndex);
		m_sTargetPath = m_vArgv[uIndex + 1];
		//为了测试后边的路径矫正是否正确，暂时把这里的去掉了
		//ConvertStrPathSlash(m_sTargetPath);
		if (uIndex + 1 > argc)
		{
			return false;
		}
		m_sTotalCmdline = m_vArgv[0] + " -r \"" + m_sTargetPath + "\"";

		return true;
	}
	else if (IsExistParam("-x"))
	{
		m_bExtract = true;
		uint32_t uIndex = 0;
		GetParamIndex("-x", &uIndex);
		if (uIndex + 1 > argc)
		{
			return false;
		}

		m_sTargetPath = m_vArgv[uIndex + 1];
		//ConvertStrPathSlash(m_sTargetPath);

		if (IsExistParam("-o"))
		{
			uint32_t uIndex = 0;
			GetParamIndex("-o", &uIndex);
			if (uIndex + 1 > argc)
			{
				return false;
			}

			m_sOutputPath = m_vArgv[uIndex + 1];
			//ConvertStrPathSlash(m_sOutputPath);
			m_sTotalCmdline = m_vArgv[0] + " -x \"" + m_sTargetPath + "\" -o\"" + m_sOutputPath + "\"";

			return true;
		}

		m_sTotalCmdline = m_vArgv[0] + " -x \"" + m_vArgv[uIndex - 1] + "\"";

		return true;
	}

	printf("Wrong parameters..\n");
	
	return false;
}

bool CCmdLineParser::IsExistParam(const char* pParam) const
{
	CHECK_NULLPTR_ERR(pParam);

	std::string sParam = pParam;
	std::vector <std::string>::const_iterator iter = find(m_vArgv.begin(), m_vArgv.end(), sParam);

	return iter != m_vArgv.end();
}

bool CCmdLineParser::GetParamIndex(const char* pParam, uint32_t* pIndex) const
{
	CHECK_NULLPTR_ERR(pParam);
	CHECK_NULLPTR_ERR(pIndex);

	std::string sParam = pParam;
	std::vector <std::string>::const_iterator iter = find(m_vArgv.begin(), m_vArgv.end(), sParam);
	if (iter != m_vArgv.end())
	{
		*pIndex = distance(m_vArgv.begin(), iter);
		return true;
	}

	return false;
}

bool CCmdLineParser::IsOnlyRecognize() const
{
	return m_bRecognize;
}

const std::string& CCmdLineParser::GetTargetPath() const
{
	return m_sTargetPath;
}

const std::string& CCmdLineParser::GetOutputPath() const
{
	return m_sOutputPath;
}

void CCmdLineParser::ConvertStrPathSlash(std::string& sPath)
{
	std::replace(sPath.begin(), sPath.end(), '\\', '/');
	
	if ('\"' == sPath.back())
	{
		sPath.erase(sPath.end() - 1);
	}

	if ('\\' == sPath.back())
	{
		sPath.erase(sPath.end() - 1);
	}
}