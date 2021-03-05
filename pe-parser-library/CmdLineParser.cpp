#include "CmdLineParser.h"
#include <vector>
#include <algorithm>

using namespace std;

CCmdLineParser::CCmdLineParser():
	m_bRecognize(false),
	m_bExtract(false)
{
	m_vecValidOption.push_back("-r");
	m_vecValidOption.push_back("-o");
	m_vecValidOption.push_back("-x");
}

CCmdLineParser::~CCmdLineParser()
{
}

bool CCmdLineParser::Parse(const uint32_t argc, char** argv)
{
	CHECK_NULLPTR_ERR(argv);

	string sProcessName = argv[0];

	if (argc < 3)
	{
		printf("Not enough parameters..\n");
		return false;
	}

	//�����ж��ǳɶԵģ����Ͻ��̱����������ż������
	if (0 == argc % 2)
	{
		printf("Incorrect parameters..\n");
		return false;
	}

	for (uint32_t i = 1; i < argc - 1; i += 2)
	{
		m_mapOptValue.insert(pair<const string, const string>(*(argv + i), *(argv + i + 1)));
	}

	if (!IsValidOptions())
	{
		return false;
	}

	//todo�������н�\����Ϊת���ַ�����-x�����-o������������ֻ��Ҫ������ǰ���\ɾ�����ɡ�
	//ʶ��������治����ѹ�����·������
	if (IsExistOption("-r"))
	{
		if (IsExistOption("-x") || IsExistOption("-o"))
		{
			return false;
		}

		m_bRecognize = true;
		m_sTargetPath = m_mapOptValue["-r"];
		//Ϊ�˲��Ժ�ߵ�·�������Ƿ���ȷ����ʱ�������ȥ����
		//ConvertStrPathSlash(m_sTargetPath);
		m_sTotalCmdline = sProcessName + " -r \"" + m_sTargetPath + "\"";

		return true;
	}
	else if (IsExistOption("-x"))
	{
		m_bExtract = true;
		m_sTargetPath = m_mapOptValue["-x"];
		//ConvertStrPathSlash(m_sTargetPath);

		if (IsExistOption("-o"))
		{
			m_sOutputPath = m_mapOptValue["-o"];
			//ConvertStrPathSlash(m_sOutputPath);
			m_sTotalCmdline = sProcessName + " -x \"" + m_sTargetPath + "\" -o\"" + m_sOutputPath + "\"";

			return true;
		}

		m_sTotalCmdline = sProcessName + " -x \"" + m_sTargetPath + "\"";

		return true;
	}

	printf("Wrong parameters..\n");
	
	return false;
}

bool CCmdLineParser::IsExistOption(const char* pParam) const
{
	CHECK_NULLPTR_ERR(pParam);
	std::string sParam = pParam;
	return m_mapOptValue.find(sParam) != m_mapOptValue.end();
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

bool CCmdLineParser::IsValidOptions() const
{
	for (auto iter = m_mapOptValue.begin(); iter != m_mapOptValue.end(); iter++)
	{
		if (!IsValidOption(iter->first))
		{
			return false;
		}
	}

	return true;
}

bool CCmdLineParser::IsValidOption(const string& sParam) const
{
	auto iter = std::find(m_vecValidOption.begin(), m_vecValidOption.end(), sParam);
	return (iter != m_vecValidOption.end());
}