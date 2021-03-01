#include "WinRar.h"

#ifdef _WIN32
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

CWinRar::CWinRar()
{
	m_szUnpackerName = "winrar";
	ResetUnpackMethod();
}

CWinRar::~CWinRar()
{
}

bool CWinRar::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;

	//Rar!//52617231
	if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("52617221"), 0, strlen("52617221"), pIsMatch) && *pIsMatch)
	{
		if ((m_pPE->HasRsrcName("STARTDLG") && m_pPE->HasRsrcName("LICENSEDLG")) || (m_pPE->HasRsrcName("INSTALL") && m_pPE->HasRsrcName("SETUP")))
		{
			*pIsMatch = true;
		}
	}

	return true;
}

void CWinRar::PrintInfo()
{
	//todo：后期添加输出子版本等信息的功能
	//printf("Summary: sfx: WinRAR(-)[-]\n");
	printf("sfx: WinRAR\n");
}

INSTALLER_TYPE CWinRar::GetEnumType()
{
	return INST_SFX_WINRAR;
}

bool CWinRar::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:WinRar..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	GetModuleFileNameA(NULL, szExePath, MAX_PATH);
	PathRemoveFileSpec(szExePath);
	std::string sExePath = szExePath;
	sExePath += "/unrar.exe";
	std::string sDstPath = szDstPath;
	std::string sTarFile = szTarFile;
	std::string sCommandLine = sExePath + " x \"" + sTarFile + "\" \"" + sDstPath + "\"";

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