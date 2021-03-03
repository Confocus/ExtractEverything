#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
using namespace peparse;

class CWiseInstallerUnpacker :
	public IPackerMatcher
{
public:
	CWiseInstallerUnpacker();
	~CWiseInstallerUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};
