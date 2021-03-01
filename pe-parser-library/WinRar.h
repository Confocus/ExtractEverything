#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"
using namespace peparse;

class CWinRar :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CWinRar();
	~CWinRar();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};