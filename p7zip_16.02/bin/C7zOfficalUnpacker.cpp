#include "C7zOfficalUnpacker.h"

const char* const g_sz7zSig_0 = "3b2140496e7374616c6c40215554462d3821";
const char* const g_sz7zSig_1 = "377abcaf271c";
const char* const g_sz7zSig_2 = "efbbbf3b2140496e7374616c6c40215554462d3821";


C7zOfficalUnpacker::C7zOfficalUnpacker():
	m_p7zOffical(new C7zOffical)
{
	m_szUnpackerName = "7zOffical";
	ResetUnpackMethod();
}

C7zOfficalUnpacker::~C7zOfficalUnpacker()
{
	if (m_p7zOffical)
	{
		delete m_p7zOffical;
		m_p7zOffical = nullptr;
	}
}

bool C7zOfficalUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	const char* pTargetFilePath = m_pPE->GetTargetFilePath();

	//临时测试使用
	*pIsMatch = m_p7zOffical->IsPacked(pTargetFilePath);
	return true;

	/*if (!m_p7zOffical->IsPacked(pTargetFilePath))
	{
		return true;
	}*/

	/*if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_sz7zSig_0), 0, strlen(g_sz7zSig_0), pIsMatch) && *pIsMatch)
	{
		return true;
	}
	else if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_sz7zSig_1), 0, strlen(g_sz7zSig_1), pIsMatch) && *pIsMatch)
	{
		return true;
	}
	else if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_sz7zSig_2), 0, strlen(g_sz7zSig_2), pIsMatch) && *pIsMatch)
	{
		return true;
	}*/

	return true;
}

void C7zOfficalUnpacker::PrintInfo()
{
	printf("7z_check\n");
}

INSTALLER_TYPE C7zOfficalUnpacker::GetEnumType()
{
	return INST_7Z_CHECK;
}

std::vector<std::string> C7zOfficalUnpacker::split(std::string& str, std::string& pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	uint32_t size = str.size();
	for (uint32_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (std::string::npos == pos)
		{
			break;
		}

		std::string s = str.substr(i, pos - i);
		result.push_back(s);
		i = pos + pattern.size() - 1;
	}
	return result;
}

bool C7zOfficalUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);

	return UnpackInternal(szTarFile, szDstPath) && !IsEmptyDirectory(szDstPath);
}

bool C7zOfficalUnpacker::UnpackInternal(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);

	return m_p7zOffical->Unpack(szTarFile, szDstPath);
}