#include "C7zip.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

C7zip::C7zip()
{
	m_szUnpackerName = "7zip";
	ResetUnpackMethod();
}

C7zip::~C7zip()
{
}

bool C7zip::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	//;!@Install@!UTF-8!//3b3140496e7374616c6c40315554462d3831
	if ((IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("3b3140496e7374616c6c40315554462d3831"), 0, strlen("3b3140496e7374616c6c40315554462d3831"), pIsMatch) && *pIsMatch) ||
		(IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("efbbbf3b3140496e7374616c6c40315554462d3831"), 0, strlen("efbbbf3b3140496e7374616c6c40315554462d3831"), pIsMatch) && *pIsMatch))
	{
		/*nOffset = PE.findSignature(nOffset, 0x2000, "'7z'BCAF271C");
		if (nOffset != -1)
		{
			bDetected = 1;
		}*/
	}
	else if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("377abcaf271c"), 0, strlen("377abcaf271c"), pIsMatch) && *pIsMatch)
	{
		return true;
	}

	return true;
}

void C7zip::PrintInfo()
{
	//printf("Summary: archive: 7-Zip(0.3)[-].\n");
	printf("7-Zip\n");
}

INSTALLER_TYPE C7zip::GetEnumType()
{
	return INST_7ZIP;
}

bool C7zip::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:7zip..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	GetModuleFileNameA(NULL, szExePath, MAX_PATH);
	PathRemoveFileSpec(szExePath);
	std::string sExePath = szExePath;
	if (Is64OS())
	{
		sExePath += "/x64/7z.exe";
	}
	else
	{
		sExePath += "/x86/7z.exe";
	}
	
	std::string sDstPath = szDstPath;
	std::string sTarFile = szTarFile;
	std::string sCommandLine = sExePath + " x \"" + sTarFile + "\" -o\"" + sDstPath + "\"";

	if (!CreateProcessA(NULL, const_cast<LPSTR>(sCommandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
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

bool C7zip::Is64OS()
{
#ifdef _WIN32

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return ((si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_IA64) || (si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_AMD64) == 64);
#endif
	return 1;
}