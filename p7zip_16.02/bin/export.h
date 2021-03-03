#pragma once
#include "global.h"

enum INSTALLER_TYPE
{
	INST_NONE = -1,
	INST_7Z_CHECK = 0,	//7z��װ��
	INST_SHIELD_ARCHIVE = 1,	//InstallShield Archive, ��InstallShield Data
	INST_WINZIP = 2,	//WinZip��װ�������������Ѷ��Ϸ����
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
	bool bUnpackSuccess;	//��װ���Ƿ��ѹ�ɹ�
	INSTALLER_TYPE eInstallerType;	//��װ������
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
// Function:  ��װ������ʶ��
// Access:    public 
// Returns:   bool �����жϺ���ִ�й������Ƿ�����쳣
// Qualifier:
// Parameter: const char * szTarFile������ѹ��װ���ļ��ľ���·��
// Parameter: INSTALLER_TYPE* pInstallerType����װ��ʶ��󷵻ص�����
// Note��δ֧�ֶ��̣߳��ⲿ����������ͬ��
//************************************
DLL_API bool InstallerTypeDetect(const char* szTarFile, INSTALLER_TYPE* pInstallerType);

//************************************
// Method:    InstallerExtract
// FullName:  InstallerExtract
// Function:  ��װ�����ͽ�ѹ
// Access:    public 
// Returns:   bool �����жϺ���ִ�й������Ƿ�����쳣������true������ʾ��ѹ�ɹ���Ҫͨ��PACK_INFO�ṹ���е�bUnpackSuccess���ж�
// Qualifier:
// Parameter: const char * szTarFile������ѹ��װ���ļ��ľ���·��
// Parameter: const char * szOutputPath����װ����ѹ������·�����������""��NULL���ڴ���ѹ��װ���ļ���ǰ·��������
// Parameter: PPACK_INFO pPackInfo����װ��ʶ��󷵻ص���Ϣ
// Note��δ֧�ֶ��̣߳��ⲿ����������ͬ��
//************************************
DLL_API bool InstallerExtract(const char* szTarFile, const char* szOutputPath, PPACK_INFO pPackInfo);

//************************************
// Method:    SetErrorMessageCallback
// FullName:  SetErrorMessageCallback
// Function:  ע�������Ϣ����Ļص�����
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: PERROR_MSG_CALLBACK pCallbackFunc��������Ϣ����Ļص�����
// Note��δ֧�ֶ��̣߳������������߳���ʹ�ã�һ��ע����һ���ص���������һ��ע�ὫʧЧ��
//************************************
DLL_API void SetErrorMessageCallback(PERROR_MSG_CALLBACK pCallbackFunc);

//************************************
// Method:    InstallerTest
// FullName:  InstallerTest
// Function:  ���Խӿڣ���������python����so�Ƿ�ɹ�
// Access:    public 
// Returns:   DLLEXPORT bool
// Qualifier:
//************************************
DLL_API bool InstallerTest();