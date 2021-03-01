#pragma once
#include "stdafx.h"
#include "iunpacker.h"
#include "ipackermatch.h"
#include "parse.h"
#include "BaseUnpacker.h"
//
//typedef struct AI_PACKER_SIG
//{
//	OVERLAY_SIG *pOverlaySig;
//}AI_PACKER_SIG, *PAI_PACKER_SIG;

class CAdvancedInstallerUnpacker :
	public CBaseUnpacker,
	public IPackerMatcher
{
public:
	CAdvancedInstallerUnpacker();
	~CAdvancedInstallerUnpacker();

	/*
	ƥ������ǵ�����
	param pIsMatch���Ƿ�ƥ�䵽������ǵ�����
	ret��
	*/
	virtual bool IsMatchPackerSig(bool *pIsMatch);
	virtual void PrintInfo();
	virtual INSTALLER_TYPE GetEnumType();
	virtual bool Unpack(const char* szTarFile, const char* szDstPath);
};

