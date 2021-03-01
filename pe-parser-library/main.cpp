#include <stdio.h>
#include "stdafx.h"
#include "UnpackManager.h"
#include <iostream>

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "Console.lib")
//extern "C" int _cdecl IsPackedBy7z(
//	int numArgs, char *args[]);
#endif

#define INST_EXT	L"exe"

#ifdef _WIN32
BOOL ConvertWStringToString(std::wstring &wstr, std::string &str)
{
	int nRet = 0;
	char *szBuf = NULL;
	DWORD dwBufSize = 0;
	dwBufSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, NULL, 0, NULL, FALSE);

	if (NULL == (szBuf = new(std::nothrow) char[dwBufSize]))
	{
		return FALSE;
	}
	memset(szBuf, 0, dwBufSize);

	if ((nRet = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, szBuf, dwBufSize, NULL, FALSE)) <= 0)
	{
		return FALSE;
	}

	str = szBuf;
	if (szBuf)
	{
		delete[]szBuf;
	}
}
#endif

//todo�����ִ������ͣ������PE������Ҳ����ļ�����
//todo��Ƕ�׵ĵݹ��ѹ��ûʵ��
//todo����Ϊ�첽���У�����Ч�ʣ����������������ͨ���������ã�;��ͬ��װ��֮����̣߳�ͬһ��֮�䵥�̣߳�
//todo:У�������·���ĺϷ��Լ��ָ���
//todo��makefile
//todo������̨�����Ȼ���ǰ�װ����ʶ��ʱ�ѿ���̨�����ų���

int main(int argc, char * argv[])
{
	std::shared_ptr<CUnpackManager> pUnpackManager = nullptr;
	if (!(pUnpackManager = CUnpackManager::GetInstance()))
	{
		return false;
	}

	if (!pUnpackManager->ParseCmdline(argc, argv))
	{
		printf("Please input \"--help\" to consult the usage..\n");
		return 1;
	}

	if (pUnpackManager->IsOnlyRecognize())
	{
		//pUnpackManager->GetOutputPath().c_str()
		pUnpackManager->ScanDir(pUnpackManager->GetTargetPath().c_str(), nullptr, nullptr);//argv[1]
	}
	else
	{
		pUnpackManager->ScanDirAndUnpack(pUnpackManager->GetTargetPath().c_str(), pUnpackManager->GetOutputPath().c_str(), nullptr);
	}

	printf("Scanning is over, press any key to exit.,.\n");

#ifdef _WIN32
	system("pause");
#endif

	return 1;
}