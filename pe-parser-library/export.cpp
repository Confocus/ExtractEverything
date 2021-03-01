#include <stdio.h>
#include "stdafx.h"
#include <iostream>
#include "UnpackManager.h"
#include "export.h"

bool InstallerTypeDetect(const char* szTarFile, INSTALLER_TYPE* pInstallerType)
{
	std::shared_ptr<CUnpackManager> pUnpackManager = nullptr;
	if (!(pUnpackManager = CUnpackManager::GetInstance()))
	{
		return false;
	}

	PACK_INFO PackInfo;
	PackInfo.eInstallerType = INST_NONE;
	PackInfo.bUnpackSuccess = false;
	if (!pUnpackManager->ScanFile(szTarFile, nullptr, &PackInfo, false))
	{
		return false;
	}

	*pInstallerType = PackInfo.eInstallerType;

	return true;
}

bool InstallerExtract(const char* szTarFile, const char* szOutputPath, PPACK_INFO pPackInfo)
{
	std::shared_ptr<CUnpackManager> pUnpackManager = nullptr;
	if (!(pUnpackManager = CUnpackManager::GetInstance()))
	{
		return false;
	}

	return pUnpackManager->ScanFile(szTarFile, szOutputPath, pPackInfo, true);
}

void SetErrorMessageCallback(PERROR_MSG_CALLBACK pCallbackFunc)
{
	if (g_ErrorMsgCallbackFunc)//∑¿÷π÷ÿ∏¥∏≥÷µ
	{
		return;
	}

	g_ErrorMsgCallbackFunc = pCallbackFunc;
}

bool InstallerTest()
{
	printf("InstallerTest\n");
	return true;
}