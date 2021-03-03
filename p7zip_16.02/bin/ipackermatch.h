#pragma once
#include "stdafx.h"
#include "parse.h"
#include "export.h"

class IPackerMatcher
{
public:
	IPackerMatcher();
	virtual ~IPackerMatcher();

	//************************************
	// Method:    IsMatchPackerSig
	// FullName:  IPackerMatcher::IsMatchPackerSig
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: pIsMatch 返回是否匹配
	// Description：执行匹配功能
	//************************************
	virtual bool IsMatchPackerSig(bool *pIsMatch) = 0;

	//************************************
	// Method:    PrintInfo
	// FullName:  IPackerMatcher::PrintInfo
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Description：输出自定义信息
	//************************************
	virtual void PrintInfo() = 0;


	virtual INSTALLER_TYPE GetEnumType() = 0;

	//************************************
	// Method:    Unpack
	// FullName:  IPackerMatcher::Unpack
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Description：实现解压功能
	//************************************
	virtual bool Unpack(const char* szTarFile, const char* szDstPath) = 0;

	//************************************
	// Method:    BoundParsedPE
	// FullName:  IPackerMatcher::BoundParsedPE
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: pParsedPE 指向PE解析类的指针
	// Description：绑定一个PE解析类，无需再自行解析PE
	//************************************
	bool SetParsedPE(peparse::CPEParser* pParsedPE);

	//各个安装包都可能用到的放到这里，个别安装包独自用到的放到自己的C类里
protected:
	uint8_t HexToDec(uint8_t uHex);
	uint8_t DecToHex(uint8_t uDec);

	//************************************
	// Method:    IsMatch
	// FullName:  IPackerMatcher::IsMatch
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: const uint8_t * szSig
	// Parameter: const uint8_t * szBuff
	// Parameter: const uint32_t uOffset
	// Parameter: const uint32_t uLength
	// Parameter: bool * pIsMatch
	// Description: 在对用偏移匹配对应特征
	//************************************
	bool IsMatch(const uint8_t * szSig, const uint8_t * szBuff, const uint32_t uOffset, const uint32_t uLength, bool * pIsMatch);

	//************************************
	// Method:    IsMatchEPSig
	// FullName:  IPackerMatcher::IsMatchEPSig
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: const uint8_t * szEPSig
	// Parameter: const uint32_t uEPSigOffset
	// Parameter: const uint32_t uEPSigSize
	// Parameter: bool * pIsMatch
	// Description: 匹配入口点特征
	//************************************
	bool IsMatchEPSig(const uint8_t * szEPSig, const uint32_t uEPSigOffset, const uint32_t uEPSigSize, bool * pIsMatch);

	//************************************
	// Method:    IsMatchOverlaySig
	// FullName:  IPackerMatcher::IsMatchOverlaySig
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: const uint8_t * szOverlaySig
	// Parameter: const uint32_t uOverlayOffset
	// Parameter: const uint32_t uOverlaySize
	// Parameter: bool * pIsMatch
	// Description: 匹配附加数据处的特征
	//************************************
	bool IsMatchOverlaySig(const uint8_t * szOverlaySig, const uint32_t uOverlayOffset, const uint32_t uOverlaySize, bool * pIsMatch);

	bool IsOverlayPresented(uint32_t *pOverlayOffset);

	bool IsMatchOverlaySig_deprecated(const uint8_t* szOverlaySig, const uint32_t uOverlayOffset, const uint32_t uOverlaySize, bool *pIsMatch);

	//************************************
	// Method:    IsFindSig
	// FullName:  IPackerMatcher::IsFindSig
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: const uint8_t * szFindSig
	// Parameter: const uint32_t uFindOffset
	// Parameter: const uint32_t uFindSize
	// Parameter: bool * pIsMatch
	// Description: 在内存中搜索某条特征是否存在
	//************************************
	bool IsFindStringSig(const char * szFindSig, const uint32_t uFindOffset, const uint32_t uFindSize, bool * pIsMatch);

	bool IsFindHexSig(const char * szFindSig, const uint32_t uFindOffset, const uint32_t uFindSize, bool * pIsMatch);


	//************************************
	// Method:    IsMatchSecName
	// FullName:  IPackerMatcher::IsMatchSecName
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * szSecName
	// Description: 是否存在以某个名称命名的节区
	//************************************
	bool IsMatchSecName(const char* szSecName);

	void SetUnpackMethod(const uint32_t uMethod);

	void ResetUnpackMethod();

	uint32_t GetUnpackMethod();

private:
	/*
	获取overlay部分里的特征
	prarm pOverlaySig：特征指针
	uSigSize：特征大小
	ret：
	*/
	//todo：是否移到Parser类中
	bool GetOverlaySig(unsigned char* pOverlaySig, uint32_t uSigOffset, uint32_t uSigSize);

protected:
	peparse::CPEParser* m_pPE;//todo：保护指针

protected:
	uint32_t m_uUnpackMethod;
	const char* m_szUnpackerName;
};