#include "SmartInstUnpacker.h"

CSmartInstUnpacker::CSmartInstUnpacker()
{
	m_szUnpackerName = "smart";
	ResetUnpackMethod();
}

CSmartInstUnpacker::~CSmartInstUnpacker()
{
}

bool CSmartInstUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>("558bec83c4..b8........e8$$$$$$$$538bd833c0a3........6a..e8........a3........a1........a3........33c0a3"), 0, strlen("558bec83c4..b8........e8$$$$$$$$538bd833c0a3........6a..e8........a3........a1........a3........33c0a3"), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//Smart Install Maker v//536d61727420496e7374616c6c204d616b65722076
		if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>("536d61727420496e7374616c6c204d616b65722076"), 0, strlen("536d61727420496e7374616c6c204d616b65722076"), pIsMatch) && *pIsMatch)
		{
			//sVersion = PE.getString(PE.getOverlayOffset() + 0x17);
			return true;
		}
		/*else if (PE.compareOverlay("..........87000001........5d0000800000"))
		{
			bDetected = 1;
		}*/
	}
	return true;
}

void CSmartInstUnpacker::PrintInfo()
{
	//printf("Summary: installer: Smart Install Maker[-]\n");
	printf("Smart Install Maker\n");
}

INSTALLER_TYPE CSmartInstUnpacker::GetEnumType()
{
	return INST_SMART;
}

bool CSmartInstUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	return true;
}