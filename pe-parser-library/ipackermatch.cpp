#include "ipackermatch.h"
using namespace peparse;

#define CALL_INST_SIZE		5

bool IPackerMatcher::IsMatchEPSig(const uint8_t * szEPSig, const uint32_t uEPSigOffset, const uint32_t uEPSigSize, bool * pIsMatch)
{
	*pIsMatch = false;
	uint32_t uOffset = 0;
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(m_pPE->m_pParsedPE);
	m_pPE->GetEPFileOffset(&uOffset);
	//todo：重载->检测指针的有效性
	return IsMatch(szEPSig, m_pPE->m_pParsedPE->fileBuffer->buf + uOffset, 0, uEPSigSize, pIsMatch);
}

bool IPackerMatcher::IsMatchOverlaySig(const uint8_t * szOverlaySig, const uint32_t uOffset, const uint32_t uOverlaySize, bool * pIsMatch)
{
	*pIsMatch = false;
	uint32_t uOverlayOffset = 0;
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(m_pPE->m_pParsedPE);
	if (!m_pPE->GetOverlayOffset(&uOverlayOffset) || 0 == uOverlayOffset)
	{
		return false;
	}

	return IsMatch(szOverlaySig, m_pPE->m_pParsedPE->fileBuffer->buf + uOverlayOffset, uOffset, uOverlaySize, pIsMatch);
}

bool IPackerMatcher::IsOverlayPresented(uint32_t *pOverlayOffset)
{
	uint32_t uOverlayOffset = 0;
	CHECK_NULLPTR_ERR(pOverlayOffset);
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(m_pPE->m_pParsedPE);

	if (m_pPE->GetOverlayOffset(&uOverlayOffset) && uOverlayOffset > 0)
	{
		*pOverlayOffset = uOverlayOffset;
		return true;
	}

	return false;
}

bool IPackerMatcher::IsMatchOverlaySig_deprecated(const uint8_t * szOverlaySig, const uint32_t uOverlayOffset, const uint32_t uOverlaySize, bool * pIsMatch)
{
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(pIsMatch);
	CHECK_NULLPTR_ERR(szOverlaySig);
	CHECK_NULLSIZE_ERR(uOverlaySize);

	bool bRet = false;
	bool bBreak = false;
	*pIsMatch = false;
	unsigned char *szOverlayBuff = nullptr;
	unsigned char* szOverlayDecSig = nullptr;
	if (nullptr == (szOverlayDecSig = new (std::nothrow)unsigned char[uOverlaySize + 1]))
	{
		goto SigMatchErr;
	}

	INIT_ARRAY(szOverlayDecSig, uOverlaySize + 1);
	for (uint32_t i = 0; i < uOverlaySize; i++)
	{
		uint8_t uHigh = HexToDec(szOverlaySig[2 * i]);
		uint8_t uLow = HexToDec(szOverlaySig[2 * i + 1]);
		szOverlayDecSig[i] = uHigh * 16 + uLow;
	}

	szOverlayBuff = new (std::nothrow)unsigned char[uOverlaySize + 1];
	if (nullptr == szOverlayBuff)
	{
		goto SigMatchErr;
	}
	INIT_ARRAY(szOverlayBuff, uOverlaySize + 1);

	if (!GetOverlaySig(szOverlayBuff, uOverlayOffset, uOverlaySize))
	{
		goto SigMatchErr;
	}

	for (uint32_t i = 0; i < uOverlaySize; i++)
	{
		if (szOverlayBuff[i] != szOverlayDecSig[i])
		{
			*pIsMatch = false;
			bBreak = true;
			break;
		}
	}

	if (!bBreak)
	{
		*pIsMatch = true;
	}
	bRet = true;

SigMatchErr:
	DELETE_ARRAY(szOverlayDecSig);
	DELETE_ARRAY(szOverlayBuff);

	return bRet;
}

bool IPackerMatcher::IsFindStringSig(const char * szFindSig, const uint32_t uFindOffset, const uint32_t uFindSize, bool * pIsMatch)
{
	CHECK_NULLPTR_ERR(szFindSig);
	CHECK_NULLPTR_ERR(pIsMatch);
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLSIZE_ERR(uFindSize);

	bool bMatch = true;
	if (strlen(szFindSig) > uFindSize)
	{
		return false;
	}

	for (uint32_t i = uFindOffset; i + strlen(szFindSig) < uFindOffset + uFindSize; i++)
	{
		bMatch = true;
		for (uint32_t j = 0; j < strlen(szFindSig); j++)
		{
			if (szFindSig[j] != m_pPE->m_pParsedPE->fileBuffer->buf[i + j])
			{
				bMatch = false;
				break;
			}
		}

		if (bMatch)
		{
			*pIsMatch = true;
			break;
		}
	}

	return true;
}

bool IPackerMatcher::IsFindHexSig(const char * szFindHexSig, const uint32_t uFindOffset, const uint32_t uFindSize, bool * pIsMatch)
{
	CHECK_NULLPTR_ERR(szFindHexSig);
	CHECK_NULLPTR_ERR(pIsMatch);
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLSIZE_ERR(uFindSize);

	*pIsMatch = false;
	bool bMatch = true;
	const uint8_t* pBuff = m_pPE->m_pParsedPE->fileBuffer->buf + uFindOffset;
	uint32_t uLength = strlen(szFindHexSig) / 2;//504B0304
	if (uLength > uFindSize)
	{
		return false;
	}

	for (uint32_t i = 0; i + uLength < uFindSize; i++)
	{
		bMatch = true;
		for (uint32_t j = 0; j < uLength; j++)
		{
			uint8_t uHigh = DecToHex(pBuff[i + j] / 16);
			uint8_t uLow = DecToHex(pBuff[i + j] % 16);
			if (szFindHexSig[2 * j] != uHigh || szFindHexSig[2 * j + 1] != uLow)
			{
				bMatch = false;
				break;
			}
		}

		if (bMatch)
		{
			*pIsMatch = true;
			break;
		}
	}

	return true;
}

bool IPackerMatcher::IsMatchSecName(const char* szSecName)
{
	return m_pPE->HasSecName(szSecName);
}

void IPackerMatcher::SetUnpackMethod(const uint32_t uMethod)
{
	m_uUnpackMethod = uMethod;
}

void IPackerMatcher::ResetUnpackMethod()
{
	m_uUnpackMethod = 0;
}

uint32_t IPackerMatcher::GetUnpackMethod()
{
	return m_uUnpackMethod;
}

uint8_t IPackerMatcher::HexToDec(uint8_t uHex)
{
	uint8_t uRet = 0;
	if (uHex <= '9' && uHex >= '0')
	{
		uRet = uHex - '0';
		return uRet;
	}

	if (uHex <= 'f' && uHex >= 'a')
	{
		uRet = uHex - 'a' + 10;
		return uRet;
	}

	if (uHex <= 'F' && uHex >= 'A')
	{
		uRet = uHex - 'A' + 10;
		return uRet;
	}

	return uRet;
}

uint8_t IPackerMatcher::DecToHex(uint8_t uDec)
{
	if (0 <= uDec && uDec <= 9)
	{
		return uDec + '0';
	}
	
	return uDec - 10 + 'a';
}

//todo：重载地址操作符，确保不越界
bool IPackerMatcher::IsMatch(const uint8_t * szSig, const uint8_t * szBuff, const uint32_t uOffset, const uint32_t uLength, bool * pIsMatch)
{
	CHECK_NULLPTR_ERR(szSig);
	CHECK_NULLPTR_ERR(szBuff);
	CHECK_NULLSIZE_ERR(uLength);
	CHECK_NULLPTR_ERR(pIsMatch);

	const uint8_t * pBuff = szBuff + uOffset / 2;
	*pIsMatch = false;
	int32_t k = 0;
	bool bBreak = false;
	//uint32_t uSize = uLength / 2;
	std::string sSig = (const char*)szSig;

	for (uint32_t i = 0; i < uLength;)
	{
		if ('.' == szSig[i])
		{
			i += 2;
			k++;
		}
		else if ('$' == szSig[i])
		{
			if (i + 8 < uLength)
			{
				//uint32_t index = i / 2;
				int32_t index = k;
				std::string sTmp(sSig, i, 8);
				if ("$$$$$$$$" == sTmp)//todo：确认下"$$$$$$$$"前面是否都是e8
				{
					int32_t uJmp = 0;
					for (int8_t j = 3; j >= 0; j--)//32位只考虑4个字节
					{
						//B0 9F 00 00
						uJmp += pBuff[index + j];
						if (0 != j)
						{
							uJmp = uJmp * 256;
						}
					}
					//执行一个跳转，继续匹配
					k += uJmp + CALL_INST_SIZE - 1;
					i += 8;
				}
			}

			//todo：逆向分析发现还有$$$$的可能
		}
		else
		{
			uint8_t uHigh = DecToHex(pBuff[k] / 16);
			uint8_t uLow = DecToHex(pBuff[k] % 16);
			if (szSig[i] != uHigh || szSig[i + 1] != uLow)
			{
				bBreak = true;
				break;
			}
			i += 2;
			k += 1;
		}
	}

	if (!bBreak)
	{
		*pIsMatch = true;
	}

	return true;
}

IPackerMatcher::IPackerMatcher()
{

}

IPackerMatcher::~IPackerMatcher()
{

}

bool IPackerMatcher::SetParsedPE(peparse::CPEParser * pParsedPE)
{
	CHECK_NULLPTR_ERR(pParsedPE);
	m_pPE = pParsedPE;

	return true;
}

bool IPackerMatcher::GetOverlaySig(unsigned char* pOverlaySig, uint32_t uSigOffset, uint32_t uSigSize)
{
	CHECK_NULLPTR_ERR(pOverlaySig);
	CHECK_NULLSIZE_ERR(uSigSize);
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(m_pPE->m_pParsedPE);
	uint32_t uOverlayOffset = 0;
	if (!m_pPE->GetOverlayOffset(&uOverlayOffset) || 0 == uOverlayOffset)
	{
		return false;
	}

	for (uint32_t i = 0; i < uSigSize; i++)
	{
		pOverlaySig[i] = m_pPE->m_pParsedPE->fileBuffer->buf[uOverlayOffset + i - 1];
	}

	return true;
}