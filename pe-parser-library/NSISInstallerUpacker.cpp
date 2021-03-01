#include "NSISInstallerUpacker.h"
#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

CNSISInstallerUpacker::CNSISInstallerUpacker():
	m_p7zUnpacker(new C7zOfficalUnpacker())
{
	m_szUnpackerName = "nsis";
	ResetUnpackMethod();
}

CNSISInstallerUpacker::~CNSISInstallerUpacker()
{
}

bool CNSISInstallerUpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(m_pPE);
	CHECK_NULLPTR_ERR(pIsMatch);

	*pIsMatch = false;
	/*uint32_t uOverlayOffset = 0;
	if (!m_pPE->GetOverlayOffset(&uOverlayOffset))
	{
		return false;
	}*/
	//var nOffset = PE.getOverlayOffset();
	if (!(IsMatchOverlaySig((const uint8_t*)"efbeadde4e756c6c..6f6674496e7374", 8, strlen("efbeadde4e756c6c..6f6674496e7374"), pIsMatch)  && *pIsMatch) 
		 && !(IsMatchOverlaySig((const uint8_t*)"efbeadde6e736973696e7374616c6c", 0, strlen("efbeadde6e736973696e7374616c6c"), pIsMatch) && *pIsMatch))
	{
		*pIsMatch = false;
		return true;

		//	if (!PE.section[".ndata"])
		//	{
		//		return "";
		//	}
		//	if (PE.isOverlayPresent())
		//	{
		//		nOffset += PE.readDword(nOffset);
		//		if (nOffset + 4 >= PE.getSize() || !PE.compare("EFBEADDE'Null'..'oftInst'", nOffset + 4))
		//		{
		//			nOffset = 0;
		//		}
		//	}
	}
	//todo：这个安装包识别的版本很多，暂以这两条作为判断
	*pIsMatch = true;
	return true;

	
	//下边是判断一些子版本信息
	//if (nOffset&&PE.isOverlayPresent())
	//{
	//	// Method detection adapted from 7-Zip.
	//	nOffset += 0x1C;
	//	if (PE.compare("5D0000..00", nOffset))
	//	{
	//		sOptions = sOptions.append("lzma", "solid");
	//	}
	//	else if (PE.compare("5D0000....00", nOffset + 4))
	//	{
	//		sOptions = sOptions.append("lzma");
	//	}
	//	else
	//	{
	//		function BorZ(nOffset)
	//		{
	//			if (PE.readByte(nOffset) == 0x31 && PE.readByte(nOffset + 1) < 14)
	//			{
	//				return "bzip2";
	//			}
	//			else
	//			{
	//				return "zlib";
	//			}
	//		}
	//		if (PE.compare("8", nOffset + 3))
	//		{
	//			sOptions = sOptions.append(BorZ(nOffset + 4));
	//		}
	//		else
	//		{
	//			sOptions = sOptions.append(BorZ(nOffset), "solid");
	//		}
	//	}
	//	bDetected = 1;
	//}
	
	//var aVersion = PE.getManifest().match(/ Null[sS]oft Install System v ? (.* ? ) < / );
	//if (aVersion)
	//{
	//	sVersion = aVersion[1];
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("558BEC83EC2C535633F657568975DC8975F4BBA49E4000FF1560704000BFC0B24000"))
	//{
	//	sVersion = "1.xx";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("558BEC81EC....000056576A..BE........598DBD"))
	//{
	//	sVersion = "1.3x";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC5C53555657FF15"))
	//{
	//	sVersion = "1.x";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC0C535657FF15....40000"))
	//{
	//	switch (PE.readWord(PE.nEP + 8))
	//	{
	//	case 0x812C: sVersion = "1.98";  break;
	//	case 0x10B4: sVersion = "2.0a0"; break;
	//	default:     sVersion = "1.xx";
	//	}
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC0C53555657FF15..7040008B35..92400005E803000089442414B320FF152C704000"))
	//{
	//	sVersion = "2.0b2/2.0b3";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC14836424040053555657C644241320FF1530704000BE00207A00"))
	//{
	//	sVersion = "2.0b4";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC1053555657C7442414....400033EDC644241320FF152C704000"))
	//{
	//	switch (PE.readWord(PE.nEP + 11))
	//	{
	//	case 0x91F0: sVersion = "2.0b4";   break;
	//	case 0x9270: sVersion = "2.0 RC2"; break;
	//	}
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC0C53555657C7442410........33DBC644241420FF15........53FF15"))
	//{
	//	sVersion = "2.0";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("83EC2053555633DB57895C2418C7442410........C644241420FF15"))
	//{
	//	sVersion = "2.06";
	//	bDetected = 1;
	//}
	//else if (PE.compareEP("558bec83ec..535633f657568975..8975..bb........ff15........bf........68........5750a3........ff15"))
	//{
	//	sVersion = "0.98";
	//	bDetected = 1;
	//}
}

void CNSISInstallerUpacker::PrintInfo()
{
	//printf("Summary: Nullsoft Scriptable Install System\n");
	printf("nsis\n");
}

INSTALLER_TYPE CNSISInstallerUpacker::GetEnumType()
{
	return INST_NSIS;
}

bool CNSISInstallerUpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

	bool bRet = false;
	bool bIsUnpackWithThirdTool = false;
	//依赖第三方工具解压的方案貌似被取代了，这个逻辑暂不执行了
	if (bIsUnpackWithThirdTool)
	{
		bRet = UnpackWithThirdTool(szTarFile, szDstPath);
	}
	else
	{
		bRet = UnpackInternal(szTarFile, szDstPath);
	}

	return bRet;
}

bool CNSISInstallerUpacker::UnpackInternal(const char* szTarFile, const char* szDstPath)
{
	return m_p7zUnpacker->Unpack(szTarFile, szDstPath);
}

bool CNSISInstallerUpacker::UnpackWithThirdTool(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szDstPath);
	CHECK_NULLPTR_ERR(szTarFile);

#ifdef _WIN32
	printf("Unpacking %s, type:NSIS..\n", szTarFile);
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

bool CNSISInstallerUpacker::Is64OS()
{
#ifdef _WIN32
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return ((si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_IA64) || (si.wProcessorArchitecture & PROCESSOR_ARCHITECTURE_AMD64) == 64);
#endif
	return 1;
}