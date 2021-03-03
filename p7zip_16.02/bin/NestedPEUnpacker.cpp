#include "NestedPEUnpacker.h"
#include "parse.h"

using namespace peparse;

CNestedPEUnpacker::CNestedPEUnpacker()
{
	m_szUnpackerName = "nestedpe";
	ResetUnpackMethod();
}

CNestedPEUnpacker::~CNestedPEUnpacker()
{
}

bool CNestedPEUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	*pIsMatch = m_pPE->HasNestedFileInRsrc(NESTED_TYPE_PE);
	return true;
	//uint32_t uRsrcNum = m_pPE->GetRsrcNum();
	//uint32_t uFileSize = 0;
	//m_pPE->GetPEFileSize(&uFileSize);
	//for (uint32_t i = 0; i < uRsrcNum; i++)
	//{
	//	uint32_t uOffset = m_pPE->GetRsrcOffset(i);
	//	std::string name = m_pPE->GetRsrcStrName(i);
	//	uint32_t uRsrcSize = m_pPE->GetRsrcSize(i);
	//	uint32_t uRsrcFileOffset = 0;
	//	m_pPE->VirtOffsetToFileOffset(uOffset, &uRsrcFileOffset);
	//	if (uRsrcFileOffset > uFileSize)
	//	{
	//		printf("suspected packed\n");
	//		return false;
	//	}

	//	const char* pRsrcBuff = reinterpret_cast<const char*>(m_pPE->m_pParsedPE->fileBuffer->buf + uRsrcFileOffset);
	//	if ('M' != pRsrcBuff[0] && 'Z' != pRsrcBuff[1])
	//	{
	//		continue;
	//	}

	//	PIMAGE_DOS_HEADER pDosHeader = NULL;
	//	PIMAGE_NT_HEADERS32 pNtHeader32 = NULL;
	//	pDosHeader = (PIMAGE_DOS_HEADER)pRsrcBuff;
	//	if (IMAGE_DOS_SIGNATURE != pDosHeader->e_magic || pDosHeader->e_lfanew > uRsrcSize)//todo：PE不一定在资源节的起始位置
	//	{
	//		continue;
	//	}
	//	//NT_OPTIONAL_64_MAGIC
	//	pNtHeader32 = (PIMAGE_NT_HEADERS32)((DWORD)pDosHeader + pDosHeader->e_lfanew);//todo：兼容64位程序
	//	if (IMAGE_NT_SIGNATURE == pNtHeader32->Signature)
	//	{
	//		*pIsMatch = true;
	//		break;
	//	}

	//	//printf("No:%d	Offset:%X	%X\n",i, uOffset, uRsrcFileOffset);
	//}

	return true;
}

void CNestedPEUnpacker::PrintInfo()
{
	printf("NestedPE\n");
}

INSTALLER_TYPE CNestedPEUnpacker::GetEnumType()
{
	return INST_NESTED_PE;
}

bool CNestedPEUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	return true;
}