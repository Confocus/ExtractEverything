#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "FileAssist.h"
#include "BaseUnpacker.h"
using namespace peparse;

typedef struct Cab_Info
{
	std::string sCabName;
	std::string sCabRelativePath;
	std::string sCabUnknowInfo;
	std::string sCabSize;
	std::string sCabAbsPath;
	uint32_t uCabSize;
	uint32_t uCabOffset;
	uint32_t uInfoLength;
}CAB_INFO, *PCAB_INFO;

class CShieldInstUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher,
	public CFileAssist
{
public:
	CShieldInstUnpacker();
	~CShieldInstUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	bool ExtractAllCabs(const char* szTarFile, const char* szDstPath, std::vector<CAB_INFO>& vCabInfos);
	bool UnpackAllCabs(std::vector<CAB_INFO>& vCabInfos);
	bool UnpackCab(const char* const szCabPath, const char* const szOutputDir);
	bool SearchAllCabsInOverlay(const uint32_t uOverlayOffset, const uint32_t uOverlaySize, std::vector<CAB_INFO>& vCabInfos);
	bool EncapsulateCabInfo(const uint32_t uOffset, PCAB_INFO pCabInfo);
	bool CreateCabOutputPath(const char* const szOutputPath) const;
	void ConvertPathSlash(char* szPath);
	void ConvertStrPathSlash(std::string& sPath);
	bool UnpackInternal(const char* szTarFile, const char* szDstPath);
};