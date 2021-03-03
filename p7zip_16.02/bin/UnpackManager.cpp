#include "UnpackManager.h"
#include "parse.h"
#include <assert.h>

#ifdef _WIN32
#include "shlwapi.h"
#else
#include <stdio.h>
#include <unistd.h>
#endif

#include "AdvancedInstallerUnpacker.h"
#include "WiseInstallerUnpacker.h"
#include "NSISInstallerUpacker.h"
#include "MSIUnpacker.h"
#include "GenteeUnpacker.h"
#include "ViseUnpacker.h"
#include "ZipSfxUnpacker.h"
#include "SmartInstUnpacker.h"
#include "SetupFactory.h"
#include "MoleBoxUnpacker.h"
#include "C7zip.h"
#include "WinRar.h"
#include "C7zOffical.h"
#include "ShieldInstUnpacker.h"
#include "InnoUnpacker.h"
#include "WinZip.h"
#include "NestedPEUnpacker.h"
#include "C7zOfficalUnpacker.h"
#include "DotnetInstallerUnpacker.h"

std::shared_ptr<CUnpackManager> CUnpackManager::m_pSingleInstance = nullptr;

std::shared_ptr<CUnpackManager> CUnpackManager::GetInstance()
{
	if (nullptr == m_pSingleInstance)
	{
		m_pSingleInstance = std::shared_ptr<CUnpackManager>(new CUnpackManager);
	}

	return m_pSingleInstance;
}

//todo：Unpack单实例
//todo：增加智能指针
//todo：禁止拷贝
CUnpackManager::CUnpackManager():
	m_uPEPackMatcherCount(0),
	m_uNPEPackMatcherCount(0),
	m_uPackerNum(0),
	m_pCmdLineParser(new CCmdLineParser)
{
	memset(m_szPEPackerMatcher, 0, PE_MATCHER_ARRAY_SIZE * sizeof(IPackerMatcher*));
	memset(m_szNPEPackerMatcher, 0, NPE_MATCHER_ARRAY_SIZE * sizeof(IPackerMatcher*));
	
	RegisterAllPEPackMatcher();
	RegisterAllNPEPackMatcher();
#ifdef _WIN32
	GetSystemTimeAsFileTime(&m_ft);
	m_ft.dwLowDateTime -= 5;
#else
	time(&m_timep);
	m_timep -= 5;
#endif
}

CUnpackManager::~CUnpackManager()
{
	if (m_pCmdLineParser)
	{
		delete m_pCmdLineParser;
	}

	for (IPackerMatcher* p : m_szPEPackerMatcher)
	{
		if (p)
		{
			delete p;
		}
	}

	for (IPackerMatcher* p : m_szNPEPackerMatcher)
	{
		if (p)
		{
			delete p;
		}
	}
}

bool CUnpackManager::ScanFile(const char* szTarPath, const char* szOutputPath, PPACK_INFO pPackInfo, bool bUnpack)
{
	CHECK_NULLPTR_ERR(szTarPath);
	CheckPathSlash(const_cast<char*>(szTarPath));
	CheckPathSlash(const_cast<char*>(szOutputPath));

	CPEParser PEParser;
	pPackInfo->eInstallerType = INST_NONE;
	pPackInfo->bUnpackSuccess = false;
	bool bIsPEPacker = PEParser.Parse(szTarPath);
	std::vector<IPackerMatcher*> vUnpackers;

	//测试用的Dump内存编码
	/*FILE *fp = NULL;
	fopen_s(&fp, "e:/VMware Share/20210130/tmp0205", "wb");
	if (!fp)
	{
		return false;
	}

	fwrite(PEParser.m_pParsedPE->fileBuffer->buf + 4319957, 54024168, 1, fp);
	fclose(fp);

	return true;*/


#ifdef _WIN32
	if (!PathFileExists(szTarPath))
	{
		return false;
	}
#else
	if (access(szTarPath, 0) != F_OK)
	{
		return false;
	}
#endif

	if (!IsValidOutputPath(szOutputPath))
	{
		return false;
	}

	if (!bIsPEPacker)
	{
		PEParser.GetNPEBuffer(szTarPath);

		//todo：应该匹配所有的类型，然后调用所有匹配上的方法依次去解压
		for (IPackerMatcher* p : m_szNPEPackerMatcher)
		{
			bool bIsMatch = false;
			if (p)
			{
				p->SetParsedPE(&PEParser);
				p->IsMatchPackerSig(&bIsMatch);
				if (bIsMatch)
				{
					pPackInfo->eInstallerType = p->GetEnumType();
					p->PrintInfo();
					if (bUnpack)
					{
						//pPackInfo->bUnpackSuccess = p->Unpack(szTarPath, "D:/WangTeng/code/pe-parse-master2/wisetest2");
					}
					goto PEPARSE_CLEAR;
				}
			}
		}
		printf("Pack recognizing failed..\n");
		//todo：Invalid PE 尝试检测msi等非PE的格式
	}
	else
	{
		if (!IsNeedScanFile(&PEParser))//目前里边的判断条件都是针对PE格式的，所以没在NPE的逻辑里加这句
		{
			goto PEPARSE_CLEAR;
		}

		for (IPackerMatcher* p : m_szPEPackerMatcher)
		{
			bool bIsMatch = false;
			if (p)
			{
				p->SetParsedPE(&PEParser);
				p->IsMatchPackerSig(&bIsMatch);
				if (bIsMatch)
				{
					//todo：用所有的安装包类型去识别一次，然后用一个vector保存有识别到的类型；
					//todo：然后在这些类型中进行一种比较，来选取使用哪种进行解压
					pPackInfo->eInstallerType = p->GetEnumType();
					p->PrintInfo();
					if (bUnpack)
					{
						pPackInfo->bUnpackSuccess = p->Unpack(szTarPath, szOutputPath);
					}
					goto PEPARSE_CLEAR;
				}
			}
		}

		printf("Pack recognizing failed..\n");
	}

PEPARSE_CLEAR:
	PEParser.Clear();

	return true;
}

bool CUnpackManager::ScanDirAndUnpack(const char* szTarDir, const char* szOutputDir, PPACK_INFO ppPackInfo)
{
	return ScanDir(szTarDir, szOutputDir, ppPackInfo, true);
}

bool CUnpackManager::ScanFileAndUnpack(const char* szTarFile, const char* szOutputDir, PPACK_INFO ppPackInfo)
{
	return ScanFile(szTarFile, szOutputDir, ppPackInfo, true);
}

bool CUnpackManager::ParseCmdline(const uint32_t argc, char** argv)
{
	CHECK_NULLPTR_ERR(argv);
	return m_pCmdLineParser->Parse(argc, argv);
}

bool CUnpackManager::ScanDir(const char* szTarDir, const char* szOutputDir, PPACK_INFO ppPackInfo, bool bUnpack)
{
	CHECK_NULLPTR_ERR(szTarDir);
	CheckPathSlash(const_cast<char*>(szTarDir));
	CheckPathSlash(const_cast<char*>(szOutputDir));

#ifdef _WIN32
	std::string sDir = szTarDir;
	std::string sFormatDir = sDir + "/*.*";

	if (!PathFileExists(szTarDir))
	{
		return false;
	}

	if (!IsValidOutputPath(szOutputDir))
	{
		return false;
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(sFormatDir.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Invalid file handle. Error is %u ", GetLastError());
		return false;
	}
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{   //找到文件
				std::string sFilePath = sDir + "/";
				sFilePath += FindFileData.cFileName;
				printf("%s\n", sFilePath.c_str());
				PACK_INFO PackInfo;
				ScanFile(sFilePath.c_str(), szOutputDir, &PackInfo, bUnpack);
			}
			else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
				&& strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0
				&& IsExistedDir(FindFileData.ftCreationTime))
			{ //找到目录
				std::string sCurDir = sDir + "/";
				sCurDir += FindFileData.cFileName;
				ScanDir(sCurDir.c_str(), szOutputDir, NULL, bUnpack);
			}
		}
	}

	FindClose(hFind);
#else
	DIR * dir; //目录流指针
	struct dirent * entry; //目录项结构体指针
	struct stat statbuf;
	std::string sDir = szTarDir;

	if (!(dir = opendir(szTarDir)))
	{
		printf("Error:No such directory..\n");
		return false;
	}

	if (!IsValidOutputPath(szOutputDir))
	{
		return false;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		std::string sTmpDir = sDir;
		sTmpDir += "/";
		sTmpDir += entry->d_name;
		lstat(sTmpDir.c_str(), &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || !IsExistedDir(statbuf))
			{
				continue;
			}
			//printf("dir: %s\n", sTmpDir.c_str());
			ScanDir(sTmpDir.c_str(), szOutputDir, NULL, bUnpack);
		}
		else if (S_ISREG(statbuf.st_mode))
		{
			std::string sTmpFile = sDir + "/";
			sTmpFile += entry->d_name;
			printf("Scanning:%s\n", sTmpFile.c_str());
			//IsPackedBy7z(sTmpFile.c_str());
			PACK_INFO PackInfo;
			ScanFile(sTmpFile.c_str(), szOutputDir, &PackInfo, bUnpack);
		}
	}

	closedir(dir);
#endif
	return true;
}

bool CUnpackManager::IsNeedScanFile(peparse::CPEParser * pParsedPE)
{
	//暂时只有这一个条件：加了壳的先不去判断
	CHECK_NULLPTR_ERR(pParsedPE);
	CHECK_NULLPTR_ERR(pParsedPE->m_pParsedPE);
	/*if (pParsedPE->HasSecName(".aspack"))
	{
		printf("aspack\n");
		return false;
	}

	if (pParsedPE->HasSecName(".enigma1"))
	{
	printf("enigma\n");
	return false;
	}

	if (pParsedPE->HasSecName("UPX0"))
	{
		printf("upx\n");
		return false;
	}

	if (pParsedPE->HasSecName(".pklstb"))
	{
		printf("pklstb\n");
		return false;
	}*/

	return true;
}

void CUnpackManager::RegisterAllPEPackMatcher()
{
	//todo：按权重排序
	RegisterPEPackMatcher(new CDotnetInstallerUnpacker);//
	RegisterPEPackMatcher(new C7zOfficalUnpacker);//
	RegisterPEPackMatcher(new CWinRar);
	RegisterPEPackMatcher(new C7zip);
	RegisterPEPackMatcher(new CMoleBoxUnpacker);
	RegisterPEPackMatcher(new CSetupFactory);
	RegisterPEPackMatcher(new CSmartInstUnpacker);
	RegisterPEPackMatcher(new CZipSfxUnpacker);
	RegisterPEPackMatcher(new CViseUnpacker);
	RegisterPEPackMatcher(new CGenteeUnpacker);
	RegisterPEPackMatcher(new CAdvancedInstallerUnpacker);
	RegisterPEPackMatcher(new CWiseInstallerUnpacker);
	RegisterPEPackMatcher(new CNSISInstallerUpacker);
	RegisterPEPackMatcher(new CShieldInstUnpacker);
	RegisterPEPackMatcher(new CInnoUnpacker);
	RegisterPEPackMatcher(new CWinZip);
	RegisterPEPackMatcher(new CNestedPEUnpacker);
}

void CUnpackManager::RegisterAllNPEPackMatcher()
{
	RegisterNPEPackMatcher(new CMSIUnpacker);
}

void CUnpackManager::RegisterPEPackMatcher(IPackerMatcher* p)
{
	CHECK_NULLPTR_RET(p);
	m_szPEPackerMatcher[m_uPEPackMatcherCount++] = p;
}

void CUnpackManager::RegisterNPEPackMatcher(IPackerMatcher* p)
{
	CHECK_NULLPTR_RET(p);
	m_szNPEPackerMatcher[m_uNPEPackMatcherCount++] = p;
}

bool CUnpackManager::IsValidOutputPath(const char* szPath) const
{
	//如果OutputPath为nullptr或者""，则使用默认路径
	if (nullptr == szPath || 0 == strlen(szPath))
	{
		return true;
	}

#ifdef _WIN32
	if (!PathFileExists(szPath))
	{
		return false;
	}
#else
	if (access(szPath, 0) != F_OK)
	{
		return false;
	}
#endif

	return true;
}

void CUnpackManager::CheckPathSlash(char* szPath)
{
	if (nullptr == szPath || 0 == strlen(szPath))
	{
		return;
	}

	char *p = szPath;
	while (*p)
	{
		if ('\\' == *p)
		{
			*p = '/';
		}
		p++;
	}
	--p;
	if ('\"' == *p)
	{
		*p = 0;
	}
}

#ifdef _WIN32
bool CUnpackManager::IsExistedDir(FILETIME &ft) const
{
	return (ft.dwHighDateTime < m_ft.dwHighDateTime || ((ft.dwHighDateTime == m_ft.dwHighDateTime) && (ft.dwLowDateTime < m_ft.dwLowDateTime)));
}
#else
bool CUnpackManager::IsExistedDir(struct stat &t) const
{
	return (t.st_ctime < m_timep);
}
#endif

const std::string& CUnpackManager::GetTargetPath() const
{
	//todo：增加路径合法性校验
	return m_pCmdLineParser->GetTargetPath();
}

const std::string& CUnpackManager::GetOutputPath() const
{
	return m_pCmdLineParser->GetOutputPath();
}

bool CUnpackManager::IsOnlyRecognize() const
{
	return m_pCmdLineParser->IsOnlyRecognize();
}
