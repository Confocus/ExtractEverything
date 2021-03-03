#include "C7zOffical.h"

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Console.lib")
extern "C" int _cdecl IsPackedBy7z(const char* szFilePath);
extern "C" int _cdecl UnpackMain(int numArgs, char *args[]);
#else
extern "C" int IsPackedBy7z(const char* szFilePath);
extern "C" int UnpackMain(int numArgs, char *args[]);
#endif

#define DEFAULT_ARGC	4

C7zOffical::C7zOffical()
{
}

C7zOffical::~C7zOffical()
{
}

bool C7zOffical::IsPacked(const char* szFilePath)
{
	return IsPackedBy7z(szFilePath);
}

bool C7zOffical::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);

	bool bRet = false;
	int uArgc = DEFAULT_ARGC;
	const char** ppArgv = nullptr;
	std::string sOutputPath;
	std::string sOutputParam;

	if (!(ppArgv = new(std::nothrow) const char*[DEFAULT_ARGC]))
	{
		return false;
	}

	CheckOutputDir(szDstPath, sOutputPath);

	sOutputParam = "-o" + sOutputPath + "";

	ppArgv[0] = "xxxx";
	ppArgv[1] = "x";
	ppArgv[2] = szTarFile;
	ppArgv[3] = sOutputParam.c_str();

	if (S_OK == UnpackMain(uArgc, const_cast<char**>(ppArgv)))
	{
		bRet = true;
	}

	delete[] ppArgv;

	return bRet;
}