#include "MSIUnpacker.h"

CMSIUnpacker::CMSIUnpacker()
{
	m_szUnpackerName = "msi";
	ResetUnpackMethod();
}

CMSIUnpacker::~CMSIUnpacker()
{
}

bool CMSIUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(pIsMatch);

	*pIsMatch = false;
	//bool IsMatch(const uint8_t * szSig, const uint8_t * szBuff, const uint32_t uOffset, const uint32_t uLength, bool * pIsMatch);

	return IsMatch((const uint8_t *)"d0cf11e0a1b11ae1", reinterpret_cast<const uint8_t*>(m_pPE->GetNPEBufferPtr()), 0, strlen("d0cf11e0a1b11ae1"), pIsMatch);
	/*if (Binary.compare("D0CF11E0A1B11AE1"))
	{
		sVersion = "MS Office 97-2003 or MSI etc.";
		bDetected = 1;
	}*/
}

void CMSIUnpacker::PrintInfo()
{
	//printf("Summary: MS Office 97-2003 or MSI etc.\n");
	printf("msi\n");
}

INSTALLER_TYPE CMSIUnpacker::GetEnumType()
{
	return INST_MSI;
}

bool CMSIUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:MSI..\n", szTarFile);
	char szExePath[MAX_PATH] = { 0 };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::string sCommandLine = "msiexec /a ";
	std::string sTarFile = szTarFile;
	sCommandLine += "\"" + sTarFile + "\" ";
	sCommandLine += "/qn TARGETDIR=";
	std::string sDstPath = szDstPath;
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