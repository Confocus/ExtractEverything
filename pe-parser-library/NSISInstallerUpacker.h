#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "C7zOfficalUnpacker.h"
#include "BaseUnpacker.h"
#include <memory>

using namespace peparse;
class CNSISInstallerUpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CNSISInstallerUpacker();
	~CNSISInstallerUpacker();

	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);

private:
	bool UnpackInternal(const char* szTarFile, const char* szDstPath);

	//************************************
	// Method:    UnpackWithThirdTool
	// FullName:  CNSISInstallerUpacker::UnpackWithThirdTool
	// Function:  调用第三方工具解压
	// Access:    virtual private 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szTarFile
	// Parameter: const char * szDstPath
	//************************************
	virtual bool UnpackWithThirdTool(const char* szTarFile, const char* szDstPath);

private:
	bool Is64OS();

private:
	std::shared_ptr<C7zOfficalUnpacker> m_p7zUnpacker;
};