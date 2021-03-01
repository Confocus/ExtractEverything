#pragma once
#include "stdafx.h"
#include "iunpacker.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"
//
//typedef struct AI_PACKER_SIG
//{
//	OVERLAY_SIG *pOverlaySig;
//}AI_PACKER_SIG, *PAI_PACKER_SIG;

class CAdvancedInstallerUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CAdvancedInstallerUnpacker();
	~CAdvancedInstallerUnpacker();

	/*
	匹配这个壳的特征
	param pIsMatch：是否匹配到了这个壳的特征
	ret：
	*/
	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};

