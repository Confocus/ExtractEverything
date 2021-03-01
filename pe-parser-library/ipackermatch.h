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
	// Parameter: pIsMatch �����Ƿ�ƥ��
	// Description��ִ��ƥ�书��
	//************************************
	virtual bool IsMatchPackerSig(bool *pIsMatch) = 0;

	//************************************
	// Method:    PrintInfo
	// FullName:  IPackerMatcher::PrintInfo
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Description������Զ�����Ϣ
	//************************************
	virtual void PrintInfo() = 0;


	virtual INSTALLER_TYPE GetEnumType() = 0;

	//************************************
	// Method:    Unpack
	// FullName:  IPackerMatcher::Unpack
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Description��ʵ�ֽ�ѹ����
	//************************************
	virtual bool Unpack(const char* szTarFile, const char* szDstPath) = 0;

	//************************************
	// Method:    BoundParsedPE
	// FullName:  IPackerMatcher::BoundParsedPE
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: pParsedPE ָ��PE�������ָ��
	// Description����һ��PE�����࣬���������н���PE
	//************************************
	bool SetParsedPE(peparse::CPEParser* pParsedPE);

	//������װ���������õ��ķŵ��������װ�������õ��ķŵ��Լ���C����
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
	// Description: �ڶ���ƫ��ƥ���Ӧ����
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
	// Description: ƥ����ڵ�����
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
	// Description: ƥ�丽�����ݴ�������
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
	// Description: ���ڴ�������ĳ�������Ƿ����
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
	// Description: �Ƿ������ĳ�����������Ľ���
	//************************************
	bool IsMatchSecName(const char* szSecName);

	void SetUnpackMethod(const uint32_t uMethod);

	void ResetUnpackMethod();

	uint32_t GetUnpackMethod();

private:
	/*
	��ȡoverlay�����������
	prarm pOverlaySig������ָ��
	uSigSize��������С
	ret��
	*/
	//todo���Ƿ��Ƶ�Parser����
	bool GetOverlaySig(unsigned char* pOverlaySig, uint32_t uSigOffset, uint32_t uSigSize);

protected:
	peparse::CPEParser* m_pPE;//todo������ָ��

protected:
	uint32_t m_uUnpackMethod;
	const char* m_szUnpackerName;
};