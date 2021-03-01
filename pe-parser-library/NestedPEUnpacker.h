#pragma once
#include "ipackermatch.h"
#include "stdafx.h"
#include "BaseUnpacker.h"

class CNestedPEUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CNestedPEUnpacker();
	~CNestedPEUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
}; 