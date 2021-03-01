#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"

using namespace peparse;

class CGenteeUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CGenteeUnpacker();
	~CGenteeUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};

