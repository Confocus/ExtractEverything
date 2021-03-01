#include "WiseInstallerUnpacker.h"
#include "parse.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

#define WISE_EP_SIG_1		"558bec81ec........5356576a..ff15........ff15........8bf08975..8a063c..0f85........8a46..46"
#define WISE_EP_SIG_2		"558bec81ec........5356576a..5e6a..8975..ff15........ff15........8bf8897d..8a073c..0f85........8a47..47"
#define WISE_EP_SIG_3		"81ec........535556576a..ff15........33db895c24..895c24..895c24..895c24..895c24..ff15........8a08894424"

CWiseInstallerUnpacker::CWiseInstallerUnpacker()
{
	m_szUnpackerName = "wise";
	ResetUnpackMethod();
}

CWiseInstallerUnpacker::~CWiseInstallerUnpacker()
{
}

bool CWiseInstallerUnpacker::IsMatchPackerSig(bool * pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	CHECK_NULLPTR_ERR(m_pPE);

	*pIsMatch = false;
	uint32_t uOverlaySize = 0;
	if (m_pPE->GetOverlaySize(&uOverlaySize) && uOverlaySize > 80)
	{
		uint32_t uOverlayOffset = 0;
		uint32_t uOffset = 0;
		uint32_t uDWord = 0;
		uint32_t uSize = 0;
		if (!m_pPE->GetOverlayOffset(&uOverlayOffset) || 0 == uOverlayOffset)
		{
			return false;
		}

		uOffset = uOverlayOffset + 77;
		if (m_pPE->ReadDWord(uOffset, &uDWord) && m_pPE->GetPEFileSize(&uSize) && uDWord == uSize)
		{
			*pIsMatch = true;
			return true;
		}
		else if (IsMatchSecName(".WISE"))
		{
			*pIsMatch = true;
			return true;
		}
		else if (IsMatchEPSig((const uint8_t*)WISE_EP_SIG_1, 0, sizeof(WISE_EP_SIG_1), pIsMatch) && *pIsMatch)
		{
			return true;
		}
		else if (IsMatchEPSig((const uint8_t*)WISE_EP_SIG_2, 0, sizeof(WISE_EP_SIG_2), pIsMatch) && *pIsMatch)
		{
			return true;
		}

	}
	else if (IsMatchEPSig((const uint8_t*)WISE_EP_SIG_3, 0, sizeof(WISE_EP_SIG_3), pIsMatch) && *pIsMatch)
	{
		/*if (PE.isSectionNamePresent(".WISE"))
		{
			sOptions = "CAB";
			bDetected = 1;
		}*/
	}

	/*if (PE.isSectionNamePresent(".WISE"))
	{
		if (PE.findSignature(PE.getSectionFileOffset(PE.getSectionNumber(".WISE")), 0x1000, "'Wise Installation Wizard...'") != -1)
		{
			bDetected = 1;
		}
	}
	if (PE.compareEP("64a1........558bec6a..68........68........50648925........83ec..5356578965..ff15"))
	{
		var nOffset = PE.getOverlayOffset();
		var nZipsig = PE.findSignature(nOffset, 0x1000, "504b0304");
		if (nZipsig != -1)
		{
			sOptions = "ZIP";
			bDetected = 1;
		}
		else if (PE.compareOverlay("'MSCF'0000"))
		{
			sOptions = "CAB";
			bDetected = 1;
		}
	}*/

	return true;
}

void CWiseInstallerUnpacker::PrintInfo()
{
	//printf("Summary: Wise Installer(-)[-]\n");
	printf("Wise Installer\n");
}

INSTALLER_TYPE CWiseInstallerUnpacker::GetEnumType()
{
	return INST_WISE;
}

bool CWiseInstallerUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:Wise Install..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	GetModuleFileNameA(NULL, szExePath, MAX_PATH);
	PathRemoveFileSpec(szExePath);
	std::string sExePath = szExePath;
	std::string sCommandLine = "quickbms.exe InstExpl.wcx ";
	std::string sTarFile = szTarFile;
	std::string sDstPath = szDstPath;
	sCommandLine += "\"" + sTarFile + "\" ";
	sCommandLine += "\"" + sDstPath + "\"";

	//todo：防止弹出控制台窗口
	if (!CreateProcessA(NULL, const_cast<LPSTR>(sCommandLine.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		HRESULT hErr = GetLastError();
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	printf("Unpacking %s finished..\n", szTarFile);
#endif

	return true;
}