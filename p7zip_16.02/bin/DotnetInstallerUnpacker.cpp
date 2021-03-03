#include "DotnetInstallerUnpacker.h"

const char* const g_szDotnetInstSig_0 = "e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb........3bc774..85c374";
const char* const g_szDotnetInstSig_1 = "377abcaf";
const char* const g_szDotnetInstSig_2 = "7a7a642e0200";

#define MAX_DOTNETINST_SIG_OFFSET	0x110

#ifndef _WIN32
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

CDotnetInstallerUnpacker::CDotnetInstallerUnpacker():
	m_uExtractNum(0),
	m_p7zUnpacker(new C7zOfficalUnpacker())
{
	m_szUnpackerName = "dotNetInstaller";
	ResetUnpackMethod();
}

CDotnetInstallerUnpacker::~CDotnetInstallerUnpacker()
{
}

bool CDotnetInstallerUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szDotnetInstSig_0), 0, strlen(g_szDotnetInstSig_0), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		uint32_t uOverlayOffset = 0;
		uint32_t uOverlaySize = 0;
		m_pPE->GetOverlayOffset(&uOverlayOffset);
		m_pPE->GetOverlaySize(&uOverlaySize);

		if (IsFindHexSig(g_szDotnetInstSig_1, uOverlayOffset, min(MAX_DOTNETINST_SIG_OFFSET, uOverlaySize), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}

	return true;
}

void CDotnetInstallerUnpacker::PrintInfo()
{
	printf("dotNet Installer\n");
}

INSTALLER_TYPE CDotnetInstallerUnpacker::GetEnumType()
{
	return INST_DOTNET;
}

bool CDotnetInstallerUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);
	CHECK_NULLPTR_ERR(szDstPath);

	bool bMatch = false;
	uint32_t uOverlayOffset = 0;
	uint32_t uOverlaySize = 0;
	uint32_t uOffset = 0;
	m_pPE->GetOverlayOffset(&uOverlayOffset);
	m_pPE->GetOverlaySize(&uOverlaySize);
	const std::uint8_t* pCur = nullptr;

	//xxx里没有zzd这个特征，所以没把它放到IsMatchPackerSig中
	if (!(IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_szDotnetInstSig_2), 0, strlen(g_szDotnetInstSig_2), &bMatch) && bMatch))
	{
		return false;
	}

	uOffset = uOverlayOffset;
	uOffset += strlen(g_szDotnetInstSig_2) / 2 + 8;
	pCur = m_pPE->GetBuffer() + uOverlayOffset + strlen(g_szDotnetInstSig_2) / 2;

	while (HasUnprocessed7zPackage(uOffset) && ((uOffset - uOverlayOffset) < uOverlaySize))
	{
		uint32_t u7zSize = 0;
		u7zSize = HexToDec(pCur, 4);
		pCur += 8;//跳过7z大小和7z的CheckSum
		bMatch = false;
		//检查7z标记是否正确
		if (!(IsMatch(reinterpret_cast<const std::uint8_t*>(g_szDotnetInstSig_1), pCur, 0, strlen(g_szDotnetInstSig_1), &bMatch) && bMatch))
		{
			break;
		}

		std::string sTmpPath;
		CreateTempOutputPathString(szDstPath, sTmpPath);
		if (DumpToLocal(reinterpret_cast<const char*>(pCur), 0, u7zSize, sTmpPath.c_str()))
		{
			m_vecDump7zFile.push_back(sTmpPath);
		}

		pCur += u7zSize;
		uOffset += u7zSize;
	}

	for (std::string sTmpPath : m_vecDump7zFile)
	{
		m_p7zUnpacker->Unpack(sTmpPath.c_str(), szDstPath);
		remove(sTmpPath.c_str());
	}

	return true;
}

uint32_t CDotnetInstallerUnpacker::HexToDec(const uint8_t* pBuff, const uint32_t uSize)
{
	CHECK_NULLPTR_ERR(pBuff);
	CHECK_NULLSIZE_ERR(uSize);
	uint32_t uDec = 0;
	for (int i = uSize - 1; 0 <= i; i--)
	{
		uDec = uDec * 256 + pBuff[i];
	}

	return uDec;
}

bool CDotnetInstallerUnpacker::HasUnprocessed7zPackage(const uint32_t  uOffset)
{
	bool bMatch = false;
	return (IsFindHexSig(g_szDotnetInstSig_1, uOffset, 0x20, &bMatch) && bMatch);
}

bool CDotnetInstallerUnpacker::CreateTempOutputPathString(const std::string& sOutputDir, std::string& sExtractPath)
{
	sExtractPath = sOutputDir + "/";
	char szNum[0x10] = { 0 };
	std::string sTimeStamp;
	GetTimeStamp(sTimeStamp);
	sprintf(szNum, "%d", m_uExtractNum++);
	sExtractPath += szNum;
	sExtractPath += "_" + sTimeStamp;

	return true;
}