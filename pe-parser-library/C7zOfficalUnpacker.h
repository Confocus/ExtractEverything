#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "C7zOffical.h"
#include "BaseUnpacker.h"

using namespace peparse;

class C7zOfficalUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher,
	public CFileAssist
{
public:
	C7zOfficalUnpacker();
	~C7zOfficalUnpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	std::vector<std::string> split(std::string& str, std::string& pattern);
	bool UnpackInternal(const char* szTarFile, const char* szDstPath);

private:
	C7zOffical* m_p7zOffical;
};