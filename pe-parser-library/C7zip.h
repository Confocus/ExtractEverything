#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
using namespace peparse;

class C7zip :
	public IPackerMatcher
{
public:
	C7zip();
	~C7zip();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
private:
	bool Is64OS();
};

