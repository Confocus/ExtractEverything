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

//todo：区分错误类型，如解析PE错误和找不到文件错误
//todo：嵌套的递归解压还没实现
//todo：改为异步并行，提升效率（限制最大并行数，可通过参数配置）;不同安装包之间多线程，同一种之间单线程；
//todo:校验输入的路径的合法性及分隔符
//todo：makefile
//todo：控制台程序必然不是安装包，识别时把控制台程序排除掉

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