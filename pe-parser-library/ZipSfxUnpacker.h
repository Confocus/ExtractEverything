#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"
using namespace peparse;

class CZipSfxUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CZipSfxUnpacker();
	~CZipSfxUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	bool Is64OS();
};