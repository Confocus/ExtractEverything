#include "ViseUnpacker.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

#include <algorithm>
#include<string>
#include<iostream>

CViseUnpacker::CViseUnpacker()
{
	m_szUnpackerName = "vise";
	ResetUnpackMethod();
}

CViseUnpacker::~CViseUnpacker()
{
}

bool CViseUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(m_pPE->m_pParsedPE);
	*pIsMatch = false;

	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>("558bec6a..68........68........64a1........50648925........83ec..5356578965..ff15"), 0, strlen("558bec6a..68........68........64a1........50648925........83ec..5356578965..ff15"), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//ESIV//45534956
		if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("45534956"), 0, strlen("45534956"), pIsMatch) && *pIsMatch)
		{
			return true;
		}
		else if (IsMatch(reinterpret_cast<const uint8_t*>("5349564d"), m_pPE->m_pParsedPE->fileBuffer->buf, 0xf000, strlen("5349564d"), pIsMatch) && *pIsMatch)//SIVM//5349564d
		{
			return true;
		}
	}

	return true;

}

void CViseUnpacker::PrintInfo()
{
	//printf("Summary: Vise Installer data(-)[-]\n");
	printf("Vise Installer data\n");
}

INSTALLER_TYPE CViseUnpacker::GetEnumType()
{
	return INST_VISE;
}

bool CViseUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:Vise Install..\n", szTarFile);
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