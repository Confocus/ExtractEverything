#include "WinZip.h"

const char* const g_szWinZipSig_1 = "ff15........b1..380874..b1..4080....74..380874..4080....75..80....74..4033";
const char* const g_szWinZipSig_2 = "53ff15........b3..38..74..80c3..4033d28a083aca74..3acb74..408a083aca75";
const char* const g_szWinZipSig_3 = "53ff15........b3..38..74..80c3..8a48..4033d23aca74..3acb74..8a48..40";
const char* const g_szWinZipSig_4 = "e8$$$$$$$$558bec83ec..a1........8365....8365....5357bf........3bc7bb........74..85c374..f7d0";
const char* const g_szWinZipSig_5 = "504b0304";
const char* const g_szWinZipSecNameSig_1 = "_winzip_";

#define RSRC_TYPE_RT_DIALOG		5
#define MAX_NESTED_FILE_SIZE	0x10

#ifndef _WIN32
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

CWinZip::CWinZip():
	m_uExtractNum(0)
{
	m_szUnpackerName = "winzip";
	ResetUnpackMethod();
}

CWinZip::~CWinZip()
{
}

static bool CbGetPEInRsrcInfo(void* pCbParam)
{
	CHECK_NULLPTR_ERR(pCbParam);
	PCB_NESTED_FILE_INFO pCbNestedFileInfo = (PCB_NESTED_FILE_INFO)pCbParam;
	PNESTED_FILE_INFO pNestedFileInfo = reinterpret_cast<PNESTED_FILE_INFO>(pCbNestedFileInfo->pFuncBuff);
	while (!pNestedFileInfo->IsNull())
	{
		pNestedFileInfo++;
	}

	pNestedFileInfo->uFileOffset = pCbNestedFileInfo->uRsrcFileOffset;
	pNestedFileInfo->uSize = pCbNestedFileInfo->uRsrcSize;

	return true;
}

bool CWinZip::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	//用不同的判断方式判断出来的，对应的解压方式也不同
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szWinZipSig_1), 0, strlen(g_szWinZipSig_1), pIsMatch) && *pIsMatch)
	{
		return true;
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szWinZipSig_2), 0, strlen(g_szWinZipSig_2), pIsMatch) && *pIsMatch)
	{
		/*if (PE.findSignature(PE.getOverlayOffset(), Math.min(255, PE.getOverlaySize()), "504b0304") != -1)
		{
			sVersion = "2.2";
		}
		else if (PE.compareEP("8A48014033D23ACA740A3ACB74068A4801", 16))
		{
			sVersion = "8.x";
		}
		bDetected = 1;*/
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szWinZipSig_3), 0, strlen(g_szWinZipSig_3), pIsMatch) && *pIsMatch)
	{
		return true;
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szWinZipSig_4), 0, strlen(g_szWinZipSig_4), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		uint32_t nRsrcNum = m_pPE->GetRsrcNum();
		for (uint32_t i = 0; i < nRsrcNum; i++)
		{
			if (RSRC_TYPE_RT_DIALOG != m_pPE->GetRsrcType(i))
			{
				uint32_t uRsrcSize = m_pPE->GetRsrcSize(i);
				uint32_t uRsrcOffset = m_pPE->GetRsrcOffset(i);
				uint32_t uRsrcFileOffset = 0;
				m_pPE->VirtOffsetToFileOffset(uRsrcOffset, &uRsrcFileOffset);
				if(IsFindHexSig(g_szWinZipSig_5, uRsrcFileOffset, min(uRsrcSize, 0x1100), pIsMatch) && *pIsMatch)
				{
					SetUnpackMethod(METHOD_EXTRACT_PE_WINZIP);
					return true;
				}
			}
		}
		
		if (m_pPE->HasSecName(g_szWinZipSecNameSig_1))
		{
			*pIsMatch = true;
			return true;
		}
	}
	
	*pIsMatch = false;
	if (m_pPE->HasSecName(g_szWinZipSecNameSig_1))
	{
		*pIsMatch = true;
		/*var nOffset = PE.getSectionFileOffset(PE.getSectionNumber("_winzip_"));
		var ArcOffset = PE.findSignature(nOffset, 0x1000, "'PK'0304");
		if (ArcOffset != -1)
		{
			sVersion = "3.1";
			bDetected = 1;
		}*/
	}

	return false;
}

void CWinZip::PrintInfo()
{
	printf("WinZip\n");
}

INSTALLER_TYPE CWinZip::GetEnumType()
{
	return INST_WINZIP;
}

bool CWinZip::UnpackInternal(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);

	bool bRet = false;
	uint32_t uUnpackMethod = GetUnpackMethod();
	std::string sOutputPath;
	CheckOutputDir(szDstPath, sOutputPath);

	switch (uUnpackMethod)
	{
	case METHOD_EXTRACT_PE_WINZIP:
		bRet = ExtractPEAndWinzip(szTarFile, sOutputPath);
		break;
	default:
		break;
	}

	ResetUnpackMethod();

	return bRet;
}

bool CWinZip::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);

	return UnpackInternal(szTarFile, szDstPath) && !IsEmptyDirectory(szDstPath);
}

bool CWinZip::ExtractPEAndWinzip(const std::string& sTarFile, const std::string& sDstPath)
{
	bool bRet = true;
	if (!DumpPEFromRsrc(sDstPath) && !DumpWinZipFromRsrc(sDstPath))
	{
		bRet = false;
	}
	ResetExtractNum();

	return bRet;
}

bool CWinZip::DumpPEFromRsrc(const std::string& sDstPath)
{
	std::vector<NESTED_FILE_INFO> vNestedFileInfo;
	PFUNC_TRAVERSE_NESTED_FILE pFunc = CbGetPEInRsrcInfo;
	if (!GetFileInRsrcInfo(NESTED_TYPE_PE, pFunc, vNestedFileInfo))
	{
		return false;
	}

	for (NESTED_FILE_INFO info : vNestedFileInfo)
	{
		std::string sExtractPath;
		CreateTempOutputPathString(sDstPath, sExtractPath);
		DumpToLocal(reinterpret_cast<char*>(m_pPE->m_pParsedPE->fileBuffer->buf), info.uFileOffset, info.uSize, sExtractPath.c_str());
	}

	return true;
}

bool CWinZip::DumpWinZipFromRsrc(const std::string& sDstPath)
{
	std::vector<NESTED_FILE_INFO> vNestedFileInfo;
	PFUNC_TRAVERSE_NESTED_FILE pFunc = CbGetPEInRsrcInfo;
	if (!GetFileInRsrcInfo(NESTED_TYPE_ZIP, pFunc, vNestedFileInfo))
	{
		return false;
	}

	for (NESTED_FILE_INFO info : vNestedFileInfo)
	{
		std::string sExtractPath;
		CreateTempOutputPathString(sDstPath, sExtractPath);
		DumpToLocal(reinterpret_cast<char*>(m_pPE->m_pParsedPE->fileBuffer->buf), info.uFileOffset, info.uSize, sExtractPath.c_str());
	}

	return true;
}

bool CWinZip::GetFileInRsrcInfo(nested_file_type FileType, PFUNC_TRAVERSE_NESTED_FILE pFunc, std::vector<NESTED_FILE_INFO>& vNestedFileInfo)
{
	PNESTED_FILE_INFO pNestedFileInfo = nullptr;
	PNESTED_FILE_INFO pTraverseNestedFileInfo = nullptr;

	if (!(pNestedFileInfo = (PNESTED_FILE_INFO)malloc(MAX_NESTED_FILE_SIZE * sizeof(NESTED_FILE_INFO))))
	{
		return false;
	}

	for (uint32_t i = 0; i < MAX_NESTED_FILE_SIZE; i++)
	{
		(pNestedFileInfo + i)->Clear();
	}

	m_pPE->GetNestedFileInRsrc(FileType, pFunc, reinterpret_cast<char*>(pNestedFileInfo), MAX_NESTED_FILE_SIZE * sizeof(NESTED_FILE_INFO));
	pTraverseNestedFileInfo = pNestedFileInfo;
	while (!pTraverseNestedFileInfo->IsNull())
	{
		vNestedFileInfo.push_back(*pTraverseNestedFileInfo);
		pTraverseNestedFileInfo++;
	}

	free(pNestedFileInfo);
	return true;
}

inline void CWinZip::ResetExtractNum()
{
	m_uExtractNum = 0;
}

bool CWinZip::CreateTempOutputPathString(const std::string& sOutputDir, std::string& sExtractPath)
{
#ifdef _WIN32
	sExtractPath = sOutputDir + "/";
#else
	sExtractPath = sOutputDir + "/";
#endif
	char szNum[0x10] = { 0 };
	std::string sTimeStamp;
	GetTimeStamp(sTimeStamp);
	sprintf(szNum, "%d", m_uExtractNum++);
	sExtractPath += szNum;
	sExtractPath += "_" + sTimeStamp;

	return true;
}