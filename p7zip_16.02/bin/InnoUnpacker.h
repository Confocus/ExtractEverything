#pragma once
#include "ipackermatch.h"
#include "stdafx.h"
#include "parse.h"
using namespace peparse;

class CInnoUnpacker :
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