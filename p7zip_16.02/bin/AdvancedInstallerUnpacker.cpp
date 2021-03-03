#include "AdvancedInstallerUnpacker.h"
#include "parse.h"
#include <assert.h>

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif
using namespace peparse;

#define AI_PACK_OVERLAYSIG_1	"2f30ee1f5e4ee51e"
#define AI_PACK_OVERLAYSIG_2	"d0cf11e0a1b11ae1"
#define AI_PACK_EPSIG_1			"e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb........3bc774..85c374..f7d0"
#define AI_PACK_EPSIG_2			"558bec83ec..5657ff15........8bf085f675..6a..eb..e8........8a063c..8b3d........75..56ffd7"
#define AI_PACK_EPSIG_3			"e8$$$$$$$$8b0d........5657bf........be........3bcf74..85ce75..e8........8bc83bcf75..b9........eb"
#define AI_PACK_EPSIG_4			"558bec83ec..5657ff15........8bf085f675..6a..eb..e8........8a063c..8b3d........75..56ffd7"
#define AI_PACK_EPSIG_5			"e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb........3bc774..85c374..f7d0"

CAdvancedInstallerUnpacker::CAdvancedInstallerUnpacker()
{
	m_szUnpackerName = "AdvancedInstall";
}

CAdvancedInstallerUnpacker::~CAdvancedInstallerUnpacker()
{
}

bool CAdvancedInstallerUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	bool bIsMatch = false;
	*pIsMatch = false;
	if (IsMatchOverlaySig((const uint8_t*)AI_PACK_OVERLAYSIG_1, 0, strlen(AI_PACK_OVERLAYSIG_1), &bIsMatch) && bIsMatch)
	{
		bIsMatch = false;
		if (IsMatchEPSig((const uint8_t*)AI_PACK_EPSIG_1, 0, strlen(AI_PACK_EPSIG_1), &bIsMatch) && bIsMatch)
		{
			*pIsMatch = true;
			return true;
		}

		bIsMatch = false;
		if (IsMatchEPSig((const uint8_t*)AI_PACK_EPSIG_2, 0, strlen(AI_PACK_EPSIG_2), &bIsMatch) && bIsMatch)
		{
			*pIsMatch = true;
			return true;
		}

		bIsMatch = false;
		if (IsMatchEPSig((const uint8_t*)AI_PACK_EPSIG_3, 0, strlen(AI_PACK_EPSIG_3), &bIsMatch) && bIsMatch)
		{
			*pIsMatch = true;
			return true;
		}

		return false;
	}

	if (IsMatchOverlaySig((const uint8_t*)AI_PACK_OVERLAYSIG_2, 0, strlen(AI_PACK_OVERLAYSIG_2), &bIsMatch) && bIsMatch)
	{
		bIsMatch = false;
		if (IsMatchEPSig((const uint8_t*)AI_PACK_EPSIG_4, 0, strlen(AI_PACK_EPSIG_4), &bIsMatch) && bIsMatch)
		{
			*pIsMatch = true;
			return true;
		}

		bIsMatch = false;
		if (IsMatchEPSig((const uint8_t*)AI_PACK_EPSIG_5, 0, strlen(AI_PACK_EPSIG_5), &bIsMatch) && bIsMatch)
		{
			*pIsMatch = true;
			return true;
		}

		return false;
	}

	return false;

	/*else if (PE.findSignature(PE.getSize() - 0x50, 0x50, "'ADVINSTSFX'") != -1)
	{
		if (PE.compareEP("558bec837d....75..e8$$$$$$$$8b0d........5657bf........be........3bcf74"))
		{
			bDetected = 1;
		}
		else if (PE.compareEP("e8$$$$$$$$558bec83ec..8365....8365....a1........5657bf........be........3bc7"))
		{
			bDetected = 1;
		}
	}*/
}

void CAdvancedInstallerUnpacker::PrintInfo()
{
	//printf("Summary: installer: Advanced Installer(-)[-]\n");
	printf("Advanced Installer\n");
}

INSTALLER_TYPE CAdvancedInstallerUnpacker::GetEnumType()
{
	return INST_ADVANCED;
}

bool CAdvancedInstallerUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:Advanced Installer..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::string sCommandLine = szTarFile;
	sCommandLine += " /extract:\"";
	std::string sDstPath = szDstPath;
	sCommandLine += sDstPath + "\"";

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