#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "FileAssist.h"
#include "BaseUnpacker.h"

using namespace peparse;

#define METHOD_EXTRACT_PE_WINZIP		1

typedef struct NESTED_FILE_INFO
{
	NESTED_FILE_INFO():
		uFileOffset(0),
		uSize(0)
	{
	}

	~NESTED_FILE_INFO()
	{

	}

	bool IsNull()
	{
		return (0 == uFileOffset && 0 == uSize);
	}

	void Clear()
	{
		uFileOffset = 0;
		uSize = 0;
	}

	uint32_t uFileOffset;
	uint32_t uSize;
}NESTED_FILE_INFO, *PNESTED_FILE_INFO;

class CWinZip :
	public CBaseUnpacker,
	public IPackerMatcher,
	public CFileAssist
{
public:
	CWinZip();
	~CWinZip();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	bool ExtractPEAndWinzip(const std::string& sTarFile, const std::string& sDstPath);
	bool DumpPEFromRsrc(const std::string& sDstPath);
	bool DumpWinZipFromRsrc(const std::string& sDstPath);

	bool GetFileInRsrcInfo(nested_file_type FileType, PFUNC_TRAVERSE_NESTED_FILE pFunc, std::vector<NESTED_FILE_INFO>& vNestedFileInfo);
	inline void ResetExtractNum();
	bool UnpackInternal(const char* szTarFile, const char* szDstPath);
	bool CreateTempOutputPathString(const std::string& sOutputDir, std::string& sExtractPath);

private:
	uint32_t m_uExtractNum;
};