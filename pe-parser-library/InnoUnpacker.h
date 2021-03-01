#pragma once
#include "ipackermatch.h"
#include "stdafx.h"
#include "parse.h"
#include "BaseUnpacker.h"

using namespace peparse;

class CInnoUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CInnoUnpacker();
	~CInnoUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};