#pragma once
#include "global.h"

enum INSTALLER_TYPE
{
	INST_NONE = -1,
	INST_7Z_CHECK = 0,	//7z安装包
	INST_SHIELD_ARCHIVE = 1,	//InstallShield Archive, 非InstallShield Data
	INST_WINZIP = 2,	//WinZip安装包，多出现于腾讯游戏助手
	INST_SETUP_FACTORY = 3,		
	INST_INNO = 4,
	INST_NESTED_PE = 5,
	INST_WISE = 6,
	INST_GENTEE = 7,
	INST_VISE = 8,
	INST_SMART = 9,
	INST_NSIS = 10,		//Nullsoft Scriptable Install System
	INST_ADVANCED = 11,
	INST_MOLEBOX = 12,
	INST_SFX_WINRAR = 13,
	INST_MSI = 14,
	INST_7ZIP = 15,
	INST_SFX_ZIP = 16,
	INST_DOTNET = 17,	//Microsoft dotnet installer
};

typedef struct PACK_INFO
{
	bool bUnpackSuccess;	//安装包是否解压成功
	INSTALLER_TYPE eInstallerType;	//安装包类型
}PACK_INFO, *PPACK_INFO;

#ifdef _WIN32
#ifdef BUILD_DLL
#define DLL_API extern "C" __declspec(dllexport) 
#else
#define DLL_API extern "C" __declspec(dllimport) 
#endif 
#else
#define DLL_API extern "C" __attribute__ ((visibility("default")))
#endif

//************************************
// Method:    InstallerTypeDetect
// FullName:  InstallerTypeDetect
// Function:  安装包类型识别
// Access:    public 
// Returns:   bool 用来判断函数执行过程中是否出现异常
// Qualifier:
// Parameter: const char * szTarFile：待解压安装包文件的绝对路径
// Parameter: INSTALLER_TYPE* pInstallerType：安装包识别后返回的类型
// Note：未支持多线程，外部调用者做好同步
//************************************
DLL_API bool InstallerTypeDetect(const char* szTarFile, INSTALLER_TYPE* pInstallerType);

//************************************
// Method:    InstallerExtract
// FullName:  InstallerExtract
// Function:  安装包类型解压
// Access:    public 
// Returns:   bool 用来判断函数执行过程中是否出现异常，返回true并不表示解压成功，要通过PACK_INFO结构体中的bUnpackSuccess来判断
// Qualifier:
// Parameter: const char * szTarFile：待解压安装包文件的绝对路径
// Parameter: const char * szOutputPath：安装包解压后的输出路径，如果传入""或NULL则在待解压安装包文件当前路径下生成
// Parameter: PPACK_INFO pPackInfo：安装包识别后返回的信息
// Note：未支持多线程，外部调用者做好同步
//************************************
DLL_API bool InstallerExtract(const char* szTarFile, const char* szOutputPath, PPACK_INFO pPackInfo);

//************************************
// Method:    SetErrorMessageCallback
// FullName:  SetErrorMessageCallback
// Function:  注册错误信息处理的回调函数
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: PERROR_MSG_CALLBACK pCallbackFunc：错误信息处理的回调函数
// Note：未支持多线程，尽可能在主线程中使用，一旦注册了一个回调函数，下一次注册将失效。
//************************************
DLL_API void SetErrorMessageCallback(PERROR_MSG_CALLBACK pCallbackFunc);

//************************************
// Method:    InstallerTest
// FullName:  InstallerTest
// Function:  测试接口，用来测试python加载so是否成功
// Access:    public 
// Returns:   DLLEXPORT bool
// Qualifier:
//************************************
DLL_API bool InstallerTest();