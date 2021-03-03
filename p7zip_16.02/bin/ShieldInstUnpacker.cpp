#include "ShieldInstUnpacker.h"
#include "libunshield.h"
#include <algorithm>
#ifndef _WIN32
#include<algorithm>
#endif

#ifdef _WIN32
#pragma comment(lib, "libunshield")
#pragma comment(lib, "convert_utf")
#pragma comment(lib, "md5")
#endif

const char* const g_szInstSheildSig_0 = "ISc(";
const char* const g_szInstSheildSig_1 = "64a1........558bec6a..68........68........50648925........83ec..5356578965..ff15";
const char* const g_szInstSheildSig_2 = "558bec6a..68........68........64a1........50648925........83ec..5356578965..ff15";
const char* const g_szInstSheildSig_2_1 = "496e7374616c6c536869656c64204e617469766520496e7374616c6c6572";
const char* const g_szInstSheildSig_2_2 = "73657475702e636c617373";
const char* const g_szInstSheildSig_3 = "558bec83ec4456ff15........8bf085f675086affff15........8a06578b3d";
const char* const g_szInstSheildSig_4 = "558bec6a..68........68........64a1........50648925........83ec..5356578965..ff15........33d2";
const char* const g_szInstSheildSig_5 = "e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb........3bc774..85c374..f7";
const char* const g_szInstSheildSig_5_1 = "4953536574757053747265616d";
const char* const g_szInstSheildSig_6 = "e8$$$$$$$$558bec83ec..a1........8365....8365....5657bf........be........3bc774..85c674..f7";
const char* const g_szInstSheildSig_7 = "135d658c";//todo：注意区分这里是字符串特征还是十六进制特征
const char* const g_szInstSheildSig_8 = "504b0304";//todo：这里的0304匹配的是十六进制的0304//'PK'0304
const char* const g_szInstSheildSig_9 = "496e7374616c6c536869656c6400";
const char* const g_szInstSheildSecNameSig = "_cabinet";

extern "C" int unshield_extract_helper(Unshield* unshield, const char* prefix, int first, int last);
typedef int(*ActionHelper)(Unshield* unshield, const char* prefix, int first, int last);
extern "C" bool unshield_do_action(Unshield* unshield, ActionHelper helper);
extern "C" void unshield_set_output_directory(const char* const path);

#define MAX_INSTSHIELD_SIG_OFFSET	0x100

#ifndef _WIN32
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

CShieldInstUnpacker::CShieldInstUnpacker()
{
	m_szUnpackerName = "instshield";
	ResetUnpackMethod();
}

CShieldInstUnpacker::~CShieldInstUnpacker()
{
}

//todo：解决这类安装包类型识别错误的问题
//sample:21c4f650d2583e5633d587e8ce142223
bool CShieldInstUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	std::string sProductName;
	std::string sCompanyName;
	/*m_pPE->GetVersionInfoName(ProductName, sProductName);
	m_pPE->GetVersionInfoName(CompanyName, sCompanyName);*/

	//这条特征要综合InstallSheild.sg和overlay.sg考虑
	uint32_t uOverlayOffset = 0;
	uint32_t uOverlaySize = 0;
	m_pPE->GetOverlayOffset(&uOverlayOffset);
	m_pPE->GetOverlaySize(&uOverlaySize);
	if (IsFindStringSig(g_szInstSheildSig_0, uOverlayOffset, min(MAX_INSTSHIELD_SIG_OFFSET, uOverlaySize), pIsMatch) && *pIsMatch)
	{
		return true;
	}

	//一般命中这条特征基本上就可以说是installshield类型了.因为这条特征表明overlay中包含了InstallShield
	if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_9), 0, strlen(g_szInstSheildSig_9), pIsMatch) && *pIsMatch)
	{
		return true;
	}

	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_1), 0, strlen(g_szInstSheildSig_1), pIsMatch) && *pIsMatch)
	{
		
		//todo：由于缺少对应的样本，这里暂时没验证过
		if (uOverlayOffset > 0)
		{
			uint8_t uOffset = 0;
			uint32_t uFileSize = 0;
			m_pPE->ReadByte(uOverlayOffset, &uOffset);
			uOffset = uOffset + uOverlayOffset + 12;
			m_pPE->GetPEFileSize(&uFileSize);

			if ((uOffset + strlen(g_szInstSheildSig_7) <= uFileSize) &&
				IsMatch(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_7), m_pPE->m_pParsedPE->fileBuffer->buf, uOffset, strlen(g_szInstSheildSig_7), pIsMatch) && *pIsMatch)
			{
				return true;
			}
			else if ((uOffset + strlen(g_szInstSheildSig_8) <= uFileSize) &&
				IsMatch(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_8), m_pPE->m_pParsedPE->fileBuffer->buf, uOffset, strlen(g_szInstSheildSig_8), pIsMatch) && *pIsMatch)
			{
				return true;
			}
		}
		else
		{
			//	for (var i = 0; i < PE.resource.length; i++)
			//	{
			//		if (PE.resource[i].Type == 3000) //IS2
			//		{
			//			if (PE.compare("'SZDD'", PE.resource[i].Offset))
			//			{
			//				sVersion = "2.x"
			//					bDetected = 1;
			//				break;
			//			}
			//		}
			//	}
		}

		if (IsMatchSecName(g_szInstSheildSecNameSig))
		{
			*pIsMatch = true;
			return true;
		}
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_2), 0, strlen(g_szInstSheildSig_2), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//todo：这里没找到对应的样本，暂时没验证过
		if (IsFindStringSig(g_szInstSheildSig_2_1, uOverlayOffset, min(0x100, uOverlaySize), pIsMatch) && *pIsMatch)
		{
			return true;
		}
		else if (IsFindStringSig(g_szInstSheildSig_2_2, uOverlayOffset, min(0x100, uOverlaySize), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}
	//else if ((PE.getVersionStringInfo("ProductName").substr(0, 13) == "InstallShield") || (PE.getVersionStringInfo("CompanyName") == "InstallShield Software Corporation"))
	//{
		
	//	sVersion = PE.getVersionStringInfo("FileVersion").replace(/ , / g, ".").trim();
	//	bDetected = 1;
	//}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_3), 0, strlen(g_szInstSheildSig_3), pIsMatch) && *pIsMatch)
	{
		return true;
	}
	//else if (/ InstallShield / .test(PE.getManifest()))
	//{
	//	bDetected = 1;
	//}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_4), 0, strlen(g_szInstSheildSig_4), pIsMatch) && *pIsMatch)
	{
		return true;
		//	if (PE.findSignature(PE.getOverlayOffset(), Math.min(0x2000, PE.getOverlaySize()), "'ISc('") != -1)
		//	{
		//		sVersion = "19.x";
		//		bDetected = 1;
		//	}
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_5), 0, strlen(g_szInstSheildSig_5), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//todo：这里没找到对应的样本，暂时没验证过
		if (IsFindStringSig(g_szInstSheildSig_5_1, uOverlayOffset, min(0x2000, uOverlaySize), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}
	else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_6), 0, strlen(g_szInstSheildSig_6), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//todo：这里没找到对应的样本，暂时没验证过
		if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(g_szInstSheildSig_5_1), 0, strlen(g_szInstSheildSig_5_1), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}

	return true;
}

void CShieldInstUnpacker::PrintInfo()
{
	printf("InstallShield\n");
	return ;
}

INSTALLER_TYPE CShieldInstUnpacker::GetEnumType()
{
	return INST_SHIELD_ARCHIVE;
}

bool CShieldInstUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);
	
	return UnpackInternal(szTarFile, szDstPath) && !IsEmptyDirectory(szDstPath);
}

bool CShieldInstUnpacker::UnpackInternal(const char* szTarFile, const char* szDstPath)
{
	CHECK_NULLPTR_ERR(szTarFile);
	std::string sOutputPath;

	CheckOutputDir(szDstPath, sOutputPath);
	/*uint32_t ttt = 0;
	uint32_t tttt = 0;
	m_pPE->GetOverlayOffset(&ttt);
	m_pPE->GetOverlaySize(&tttt);
	DumpToLocal(reinterpret_cast<char*>(m_pPE->m_pParsedPE->fileBuffer->buf), ttt, tttt, "e:/VMware Share/2021012002/tmp");*/

	std::vector<CAB_INFO> vCabInfos;
	if (!ExtractAllCabs(szTarFile, sOutputPath.c_str(), vCabInfos))
	{
		return false;
	}

	if (!UnpackAllCabs(vCabInfos))
	{
		return false;
	}

	return true;
}

bool CShieldInstUnpacker::ExtractAllCabs(const char* szTarFile, const char* szDstPath, std::vector<CAB_INFO>& vCabInfos)
{
	CHECK_NULLPTR_ERR(szTarFile);
	CHECK_NULLPTR_ERR(szDstPath);
	uint32_t uOverlayOffset = 0;
	uint32_t uOverlaySize = 0;

	if (!m_pPE->GetOverlayOffset(&uOverlayOffset) || 0 == uOverlayOffset)
	{
		return false;
	}

	if (!m_pPE->GetOverlaySize(&uOverlaySize) || 0 == uOverlaySize)
	{
		return false;
	}

	if(!SearchAllCabsInOverlay(uOverlayOffset, uOverlaySize, vCabInfos))
	{
		return false;
	}

	for (CAB_INFO& info : vCabInfos)
	{
		int nPos = 0;
		
		info.sCabAbsPath = szDstPath;
		info.sCabAbsPath += "/" + info.sCabRelativePath;

		if (-1 != (nPos = info.sCabRelativePath.find_last_of('/')))
		{
			CreateCabOutputPath(info.sCabAbsPath.c_str());
		}
		else
		{
			return false;
		}
		
		DumpToLocal(reinterpret_cast<char*>(m_pPE->m_pParsedPE->fileBuffer->buf), info.uCabOffset + info.uInfoLength, info.uCabSize, info.sCabAbsPath.c_str());
	}

	return true;
}

bool CShieldInstUnpacker::UnpackAllCabs(std::vector<CAB_INFO>& vCabInfos)
{
	std::vector<std::string> vDirs;

	for (CAB_INFO info : vCabInfos)
	{
		int nPos = -1;
		if (-1 == (nPos = info.sCabAbsPath.find_last_of('/')))
		{
			continue;
		}

		std::string sDir = info.sCabAbsPath.substr(0, nPos + 1);
		std::vector<std::string>::const_iterator it = find(vDirs.begin(), vDirs.end(), sDir);
		if (it != vDirs.end())
		{
			continue;
		}

		ConvertPathSlash(const_cast<char*>(info.sCabAbsPath.c_str()));
		if (UnpackCab(info.sCabAbsPath.c_str(), sDir.c_str()))
		{
			vDirs.push_back(sDir);
		}
	}

	return true;
}

bool CShieldInstUnpacker::UnpackCab(const char* const szCabPath, const char* const szOutputDir)
{
	CHECK_NULLPTR_ERR(szCabPath);
	CHECK_NULLPTR_ERR(szOutputDir);

	unshield_set_log_level(UNSHIELD_LOG_LEVEL_LOWEST);
	//外部只配置了cab的输出位置，这里就默认使用cab所在的位置来释放cab解压出来的文件吧
	bool success = false;
	Unshield* unshield = NULL;

	setlocale(LC_ALL, "");

	//传进来同一目录下任意一个文件即可
	unshield = unshield_open_force_version(szCabPath, -1);
	if (!unshield)
	{
		fprintf(stderr, "Failed to open %s as an InstallShield Cabinet File\n", szCabPath);
		goto exit;
	}

	printf("Cabinet: %s\n", szCabPath);
	ConvertPathSlash(const_cast<char*>(szOutputDir));
	unshield_set_output_directory(szOutputDir);
	success = unshield_do_action(unshield, unshield_extract_helper);

exit:
	unshield_close(unshield);

	if (!success)
		return false;

	return true;
}

//todo:Do not ensure if cab is adjacent and first cab is next to the last section.
//I guess it's so.
bool CShieldInstUnpacker::SearchAllCabsInOverlay(const uint32_t uOverlayOffset, const uint32_t uOverlaySize, std::vector<CAB_INFO>& vCabInfos)
{
	CHECK_NULLSIZE_ERR(uOverlayOffset);
	CHECK_NULLSIZE_ERR(uOverlaySize);

	bool bIsMatch = false;
	uint32_t uCabNum = 0;
	uint32_t uOffset = uOverlayOffset;
	uint32_t uSize = uOverlaySize;
	CAB_INFO CabInfo;

	while (IsFindStringSig(g_szInstSheildSig_0, uOffset, min(MAX_INSTSHIELD_SIG_OFFSET, uSize - strlen(g_szInstSheildSig_0)), &bIsMatch) && bIsMatch)
	{
		uCabNum++;
		bIsMatch = false;
		CAB_INFO CabInfo;

		//todo:移入EncapsulateCabInfo
		
		if (!EncapsulateCabInfo(uOffset, &CabInfo))
		{
			return false;
		}

		vCabInfos.push_back(CabInfo);
		uOffset += CabInfo.uCabSize + CabInfo.uInfoLength;
		uSize -= uOffset;
	}

	return 0 == uCabNum ? false: true;
}

bool CShieldInstUnpacker::EncapsulateCabInfo(const uint32_t uOffset, PCAB_INFO pCabInfo)
{
	CHECK_NULLSIZE_ERR(uOffset);
	CHECK_NULLPTR_ERR(pCabInfo);

	const char* pBuff = reinterpret_cast<const char*>(m_pPE->GetBuffer());
	uint32_t i = 0;
	const char *pHead = pBuff + uOffset;
	const char *pTail = pBuff + uOffset;
	std::string sCabName;
	const char * p[4] = { 0, 0, 0, 0 };

	while (i < 4)
	{
		while (0 != *pTail)
		{
			pTail++;
		}

		if (pTail - pHead >= MAX_INSTSHIELD_SIG_OFFSET)
		{
			return false;
		}

		p[i] = pHead;
		pHead = ++pTail;
		i++;
	}

	pCabInfo->sCabName = p[0];
	pCabInfo->sCabRelativePath = p[1];
	ConvertStrPathSlash(pCabInfo->sCabRelativePath);
	pCabInfo->sCabUnknowInfo = p[2];
	pCabInfo->sCabSize = p[3];
	pCabInfo->uCabSize = atoi(pCabInfo->sCabSize.c_str());
	pCabInfo->uCabOffset = uOffset;
	pCabInfo->uInfoLength = pTail - pBuff - uOffset;

	return true;
}

bool CShieldInstUnpacker::CreateCabOutputPath(const char* const szOutputPath) const
{
	CHECK_NULLPTR_ERR(szOutputPath);
	std::string sOutputPath = szOutputPath;
	int nPos = 0;

	if (-1 == (nPos = sOutputPath.find_last_of('/')))
	{
		return false;
	}

	std::string sDir = sOutputPath.substr(0, nPos + 1);

	return CreateDir(sDir.c_str());
}

void CShieldInstUnpacker::ConvertPathSlash(char* szPath)
{
	char *p = szPath;
	while (*p)
	{
		if ('\\' == *p)
		{
			*p = '/';
		}
		p++;
	}
}

void CShieldInstUnpacker::ConvertStrPathSlash(std::string& sPath)
{
	std::replace(sPath.begin(), sPath.end(), '\\', '/');
}