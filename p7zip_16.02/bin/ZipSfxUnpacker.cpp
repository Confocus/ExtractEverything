#include "ZipSfxUnpacker.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

CZipSfxUnpacker::CZipSfxUnpacker()
{
	m_szUnpackerName = "sfx_zip";
	ResetUnpackMethod();
}

CZipSfxUnpacker::~CZipSfxUnpacker()
{
}

bool CZipSfxUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>("558bec6a..68........68........64a1........50648925........83....5356578965..ff15"), 0, strlen("558bec6a..68........68........64a1........50648925........83....5356578965..ff15"), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//todo：PE.findSignature(PE.getOverlayOffset(), Math.min(255, PE.getOverlaySize()), "'PK'0304")
		//这里原始是使用findSignature，而我这里直接匹配Overlay起始处，不知道是否有问题
		//PK//504b
		if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("504b0304"), 0, strlen("504b0304"), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>("558bec83c4..b8........e8........33c05568........64ff30648920e8"), 0, strlen("558bec83c4..b8........e8........33c05568........64ff30648920e8"), pIsMatch) && *pIsMatch)
	{
		//	if (PE.resource.length)
		//	{
		//		if (PE.resource[0].Type == 784) //ZIP
		//		{
		//			bDetected = 1;
		//		}
		//	}
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>("e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb"), 0, strlen("e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb"), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//PK//504B
		//todo：增加特征大小写转换
		if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("504b0304"), 0, strlen("504b0304"), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}
	return true;
}

void CZipSfxUnpacker::PrintInfo()
{
	printf("Summary: sfx: Zip SFX(-)[by Intel]\n");
}

INSTALLER_TYPE CZipSfxUnpacker::GetEnumType()
{
	return INST_SFX_ZIP;
}

bool CZipSfxUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:sfx zip..\n", szTarFile);
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

bool CZipSfxUnpacker::Is64OS()
{
#ifdef _WIN32
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return ((si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_IA64) || (si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_AMD64) == 64);
#endif

	return 1;
}