#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "CmdLineParser.h"
#include <memory>

#ifndef _WIN32
#include   <time.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define PE_MATCHER_ARRAY_SIZE		20
#define NPE_MATCHER_ARRAY_SIZE		20

class CUnpackManager
{
public:
	static std::shared_ptr<CUnpackManager> GetInstance();
	~CUnpackManager();

private:
	CUnpackManager();
	CUnpackManager(const CUnpackManager& p);
	CUnpackManager& operator=(const CUnpackManager& p);
		   
public:
	//************************************
	// Method:    ScanDir
	// FullName:  CUnpackManager::ScanDir
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szTarDir ��ɨ���Ŀ¼
	// Parameter: PPACK_INFO *ppPackInfo ������ɨ����
	// Description: ɨ��ĳ��Ŀ¼�µİ�װ��������
	//************************************
	bool ScanDir(const char* szTargetDir, const char* szOutputDir, PPACK_INFO ppPackInfo, bool bUnpack = false);

	//************************************
	// Method:    ScanFile
	// FullName:  CUnpackManager::ScanFile
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szTarFile ��ɨ���Ŀ¼
	// Parameter: PPACK_INFO p ������ɨ����
	// Description: ʶ��ĳ���ļ��İ�װ������
	//************************************
	bool ScanFile(const char* szTarFile, const char* szOutputPath, PPACK_INFO pPackInfo, bool bUnpack = false);

	//************************************
	// Method:    ScanDirAndUnpack
	// FullName:  CUnpackManager::ScanDirAndUnpack
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szTarDir
	// Parameter: PPACK_INFO * ppPackInfo
	// Description: ɨ��ĳ��Ŀ¼�µİ�װ�������Ͳ���ѹ
	//************************************
	bool ScanDirAndUnpack(const char* szTarDir, const char* szOutputDir, PPACK_INFO ppPackInfo);

	//************************************
	// Method:    ScanFileAndUnpack
	// FullName:  CUnpackManager::ScanFileAndUnpack
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szTarFile
	// Parameter: PPACK_INFO pPackInfo
	// Description: ʶ��ĳ���ļ��İ�װ�����Ͳ���ѹ
	//************************************
	bool ScanFileAndUnpack(const char* szTarFile, const char* szOutputDir, PPACK_INFO pPackInfo);

	bool ParseCmdline(const uint32_t argc, char** argv);

	const std::string& GetTargetPath() const;
	const std::string& GetOutputPath() const;

	bool IsOnlyRecognize() const;

private:
	bool IsNeedScanFile(peparse::CPEParser * pParsedPE);
	void RegisterAllPEPackMatcher();
	void RegisterAllNPEPackMatcher();
	inline void RegisterPEPackMatcher(IPackerMatcher* p);
	inline void RegisterNPEPackMatcher(IPackerMatcher* p);
	bool IsValidOutputPath(const char* szPath) const;
	void CheckPathSlash(char* szPath);
#ifdef _WIN32
	bool IsExistedDir(FILETIME &ft) const;
#else
	bool IsExistedDir(struct stat &t) const;
#endif

private:
	static std::shared_ptr<CUnpackManager> m_pSingleInstance;
	uint32_t m_uPEPackMatcherCount;
	uint32_t m_uNPEPackMatcherCount;
	uint32_t m_uPackerNum;
	CCmdLineParser* m_pCmdLineParser;
	IPackerMatcher* m_szPEPackerMatcher[PE_MATCHER_ARRAY_SIZE];
	IPackerMatcher* m_szNPEPackerMatcher[NPE_MATCHER_ARRAY_SIZE];
#ifdef _WIN32
	FILETIME m_ft;
#else
	time_t   m_timep;
#endif
};