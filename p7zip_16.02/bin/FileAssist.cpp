#include "FileAssist.h"

#ifdef _WIN32
#include <time.h>
#include <shlwapi.h> 
#else
#include<sys/time.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

CFileAssist::CFileAssist()
{
}

CFileAssist::~CFileAssist()
{
}

bool CFileAssist::CreateOutputDirStr(const std::string & sTarFile, const std::string & sDstFile, const bool bDefaultOutput, std::string & sOutputFile)
{
	int nPos = 0;
	std::string sTimeStamp;
	GetTimeStamp(sTimeStamp);

	if (bDefaultOutput)
	{
		sOutputFile = sTarFile + "_" + sTimeStamp;
		return true;
	}

	if (-1 == (nPos = sTarFile.find_last_of('/')))
	{
		sOutputFile = sDstFile + sTarFile + "_" + sTimeStamp;
		return true;
	}

	std::string sFileName = sTarFile.substr(nPos + 1, sTarFile.length());
	sOutputFile = sDstFile + "/" + sFileName + "_" + sTimeStamp;

	return true;
}

bool CFileAssist::CreateDir(const char* const szOutputFile) const
{
	bool bRet = false;

#ifdef _WIN32
	if (false == (bRet = PathFileExists(szOutputFile)))
	{
		bRet = CreateDirectory(szOutputFile, NULL);
	}
#else
	if (-1 == access(szOutputFile, F_OK))
	{
		if (0 == mkdir(szOutputFile, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
		{
			bRet = true;
		}
	}
#endif

	return bRet;
}

bool CFileAssist::IsEmptyDirectory(const char *const szOutputPath) const
{
	bool bIsEmpty = true;
	if (nullptr == szOutputPath)//路径不合法就认为目录为空
	{
		return bIsEmpty;
	}

#ifdef _WIN32
	std::string sDir = szOutputPath;
	std::string sFormatDir = sDir + "/*.*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(sFormatDir.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return bIsEmpty;
	}
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				bIsEmpty = false;
				break;
			}
			else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
				&& strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0)
			{
				std::string sCurDir = sDir + "/";
				sCurDir += FindFileData.cFileName;
				if (false == (bIsEmpty = IsEmptyDirectory(sCurDir.c_str())))
				{
					break;
				}
			}
		}
	}

	FindClose(hFind);
#else
DIR * dir; //目录流指针
struct dirent * entry; //目录项结构体指针
struct stat statbuf;
std::string sDir = szOutputPath;

if (!(dir = opendir(szOutputPath)))
{
	return bIsEmpty;
}

while ((entry = readdir(dir)) != NULL)
{
	std::string sTmpDir = sDir;
	sTmpDir += "/";
	sTmpDir += entry->d_name;
	lstat(sTmpDir.c_str(), &statbuf);
	if (S_ISDIR(statbuf.st_mode))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
		{
			continue;
		}
		//printf("dir: %s\n", sTmpDir.c_str());
		if (false == (bIsEmpty = IsEmptyDirectory(sTmpDir.c_str())))
		{
			break;
		}
	}
	else if (S_ISREG(statbuf.st_mode))
	{
		bIsEmpty = false;
		break;
	}
}

closedir(dir);
#endif

return bIsEmpty;
}

bool CFileAssist::CreateOutputDir(const char* szTarFile, const char* szDstFile, std::string & sOutputFile)
{
	if (nullptr == szDstFile)
	{
		CreateOutputDirStr(szTarFile, "", true, sOutputFile);
	}
	else
	{
		if (0 != strlen(szDstFile))
		{
			CreateOutputDirStr(szTarFile, szDstFile, false, sOutputFile);
		}
		else
		{
			CreateOutputDirStr(szTarFile, szDstFile, true, sOutputFile);
		}
	}

	return CreateDir(sOutputFile.c_str());
}

void CFileAssist::CheckOutputDir(const char* szDstPath, std::string & sOutputFile)
{
	if (nullptr == szDstPath || 0 == strlen(szDstPath))
	{
		sOutputFile = "./";
	}
	else
	{
		sOutputFile = szDstPath;
	}
}

#ifndef _WIN32
int fopen_s(FILE **f, const char *name, const char *mode) {
	int ret = 0;
	*f = fopen(name, mode);
	/* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
	if (!*f)
		ret = errno;
	return ret;
}
#endif

bool CFileAssist::DumpToLocal(const char* buff, uint32_t uOffset, uint32_t uSize, const char * const szOutputPath) const
{
	CHECK_NULLPTR_ERR(buff);
	CHECK_NULLPTR_ERR(szOutputPath);
	FILE *fp = NULL;
	fopen_s(&fp, szOutputPath, "wb");
	if (!fp)
	{
		return false;
	}

	fwrite(buff + uOffset, uSize, 1, fp);
	fclose(fp);

	return true;
}

bool CFileAssist::GetTimeStamp(std::string& sTimeStamp)
{
	long long timestamp = 0;
	char szTimeStamp[0x20] = { 0 };
#ifdef _WIN32
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	timestamp = clock * 1000000 + wtm.wMilliseconds * 1000;  //获取微秒数
	sprintf(szTimeStamp, "%lld", timestamp);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
	sprintf(szTimeStamp, "%lld", timestamp);
#endif
	sTimeStamp = szTimeStamp;

	return true;
}