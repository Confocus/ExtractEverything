#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"
#include "FileAssist.h"
#include <memory>
#include "C7zOfficalUnpacker.h"

using namespace peparse;

class CDotnetInstallerUnpacker:
	public CBaseUnpacker,
	public IPackerMatcher,
	public CFileAssist
{
public:
	CDotnetInstallerUnpacker();
	~CDotnetInstallerUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	uint32_t HexToDec(const uint8_t*, const uint32_t uSize);
	bool HasUnprocessed7zPackage(const uint32_t  uOffset);
	bool CreateTempOutputPathString(const std::string& sOutputDir, std::string& sExtractPath);

private:
	uint32_t m_uExtractNum;
	std::vector<std::string> m_vecDump7zFile;
	std::shared_ptr<C7zOfficalUnpacker> m_p7zUnpacker;
};