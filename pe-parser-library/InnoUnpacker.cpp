#include "InnoUnpacker.h"
#include "parse.h"

const char* const g_szInnoSig_1 = "7a6c621a";
const char* const g_szInnoSig_2 = "6964736b6133321a";

CInnoUnpacker::CInnoUnpacker()
{
	m_szUnpackerName = "inno";
	ResetUnpackMethod();
}

CInnoUnpacker::~CInnoUnpacker()
{
}

bool CInnoUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	uint32_t uOverlayOffset = 0;
	uint32_t uOverlaySize = 0;
	m_pPE->GetOverlayOffset(&uOverlayOffset);
	m_pPE->GetOverlaySize(&uOverlaySize);
	if ((uOverlaySize < strlen(g_szInnoSig_1) / 2) || (uOverlaySize < strlen(g_szInnoSig_2) / 2))
	{
		return false;
	}
	if ((IsMatch(reinterpret_cast<const uint8_t*>(g_szInnoSig_1), m_pPE->m_pParsedPE->fileBuffer->buf + uOverlayOffset, 0, strlen(g_szInnoSig_1), pIsMatch) && *pIsMatch) ||
		(IsMatch(reinterpret_cast<const uint8_t*>(g_szInnoSig_2), m_pPE->m_pParsedPE->fileBuffer->buf + uOverlayOffset, 0, strlen(g_szInnoSig_2), pIsMatch) && *pIsMatch))
	{
		return true;
	}
	/*else
	{
		if (PE.getString(0x30, 4) == "InUn")
		{
			sOptions = "uninstall";
			var nOffset = PE.findString(PE.getOverlayOffset(), Math.min(0x100, PE.getOverlaySize()), "Inno Setup Messages");
			if (nOffset != -1)
			{
				getVersionAndOptions(nOffset + 21);
				bDetected = 1;
			}
			if (!bDetected)
			{
				nOffset = PE.findString(PE.section[0].FileOffset, PE.section[0].FileSize, "Inno Setup version");
				if (nOffset != -1)
				{
					getVersionAndOptions(nOffset + 19);
					bDetected = 1;
				}
			}
			if (!bDetected)
			{
				if (getVersion("DATA") || getVersion(".data"))
				{
					bDetected = 1;
				}
			}
		}
	}
	if (!bDetected)
	{
		if (PE.compareOverlay("'Inno Setup Messages'"))
		{
			sOptions = "uninstall";
			getVersionAndOptions(PE.getOverlayOffset() + 21);
			bDetected = 1;
		}
		else if (PE.compareOverlay("'Inno Setup Setup Data'"))
		{
			getVersionAndOptions(PE.getOverlayOffset() + 23);
			bDetected = 1;
		}
	}
	if (PE.compareOverlay("78da"))
	{
		sOptions = "zlib";
		if (PE.compareEP("558bec83c4..53565733c08945..8945"))
		{
			bDetected = 1;
		}
		else if (PE.compareEP("558bec83c4..e8........e8........e8"))
		{
			bDetected = 1;
		}
	}
	else if (PE.compareOverlay("'Inno'", 0x34))
	{
		sOptions = "with stub";
		bDetected = 1;
	}
	if (!bDetected)
	{
		if (PE.isNET())
		{
			var sComments = PE.getVersionStringInfo("Comments");
			if (sComments == "This installation was built with Inno Setup.")
			{
				bDetected = 1;
			}
		}
	}*/

	return false;
}

void CInnoUnpacker::PrintInfo()
{
	printf("Inno\n");
}

INSTALLER_TYPE CInnoUnpacker::GetEnumType()
{
	return INST_INNO;
}

bool CInnoUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	return true;
}