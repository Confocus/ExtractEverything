#include "MoleBoxUnpacker.h"

#ifdef _WIN32
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

#include <algorithm>
#include<string>
#include<iostream>

#define MOLEBOX_EPSIG_1		"e80000000060e8$$$$$$$$e8$$$$$$$$e8$$$$$$$$558bec83ec..56576a..ff15........8945..68........6a..ff15"
#define MOLEBOX_EPSIG_2		"60e8$$$$$$$$e8$$$$$$$$e8$$$$$$$$558bec83ec..56576a..ff15........8945..68........6a..ff15"
#define MOLEBOX_EPSIG_3		"eb$$b8........8338..74..50ff70..ff3050830424..e8$$$$$$$$558beca1........53568b75..85c0578bde75..6a..68........68........ff15"
#define MOLEBOX_EPSIG_4		"5589e583ec08c7042401000000ff15........e8b8feffff908db426000000005589e583ec08c7042402000000ff15"

CMoleBoxUnpacker::CMoleBoxUnpacker()
{
	m_szUnpackerName = "MoleBox";
	ResetUnpackMethod();
}

CMoleBoxUnpacker::~CMoleBoxUnpacker()
{
}

bool CMoleBoxUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(MOLEBOX_EPSIG_1), 0, strlen(MOLEBOX_EPSIG_1), pIsMatch) && *pIsMatch)
	{
		//sVersion = "2.3.3-2.6.4";
		return true;
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(MOLEBOX_EPSIG_2), 0, strlen(MOLEBOX_EPSIG_2), pIsMatch) && *pIsMatch)
	{
		//sVersion = "2.0.0-2.3.0";
		return true;
	}
	//todo：未实现对$$的解析，暂时注释掉
	//else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(MOLEBOX_EPSIG_3), 0, strlen(MOLEBOX_EPSIG_3), pIsMatch) && *pIsMatch)
	//{
	//	//sVersion = "2.36";
	//	return true;
	//}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(MOLEBOX_EPSIG_4), 0, strlen(MOLEBOX_EPSIG_4), pIsMatch) && *pIsMatch)
	{
		/*if (PE.isOverlayPresent() && (PE.section.length >= 6))
		{
			var nNumbersOfImports = PE.getNumberOfImports();
			if ((nNumbersOfImports == 4) || (nNumbersOfImports == 3))
			{
				if (PE.isLibraryPresent("KERNEL32.dll") && PE.isLibraryPresent("msvcrt.dll") && PE.isLibraryPresent("USER32.dll"))
				{
					sVersion = "4.X";
					bDetected = 1;
				}
			}
		}*/
		return true;
	}

	return true;
}

void CMoleBoxUnpacker::PrintInfo()
{
	//printf("Summary: protector: MoleBox[-]\n");
	printf("MoleBox\n");
}

INSTALLER_TYPE CMoleBoxUnpacker::GetEnumType()
{
	return INST_MOLEBOX;
}

bool CMoleBoxUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:MoleBox..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	GetModuleFileNameA(NULL, szExePath, MAX_PATH);
	PathRemoveFileSpec(szExePath);
	std::string sExePath = szExePath;
	std::string sExeDir = sExePath;
	sExePath += "/demoleition.exe";
	std::string sDstPath = szDstPath;
	std::string sTarFile = szTarFile;
	std::string sCommandLine = sExePath + " /nogui \"" + sTarFile + "\"";

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