#include "GenteeUnpacker.h"

CGenteeUnpacker::CGenteeUnpacker()
{
	m_szUnpackerName = "Gentee";
	ResetUnpackMethod();
}


CGenteeUnpacker::~CGenteeUnpacker()
{
}

bool CGenteeUnpacker::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const std::uint8_t *>("558bec81ec........538d85........5633db578d8d........68........895d..518945..53ff15"), 0, strlen("558bec81ec........538d85........5633db578d8d........68........895d..518945..53ff15"), pIsMatch) && *pIsMatch)
	{
		return true;
		/*if (PE.findSignature(PE.getOverlayOffset(), 0x80, "'GEA'") != -1)
		{
			bDetected = 1;
		}
		else if (PE.getNumberOfResources() > 0)
		{
			nOffset = PE.getResourceNameOffset("SETUP_TEMP");
			{
				if (PE.compare("'GEA'", nOffset))
				{
					bDetected = 1;
				}
			}
		}*/
	}
	else if (IsMatchEPSig(reinterpret_cast<const std::uint8_t *>("558bec6a..68........68........64a1........50648925........83ec..5356578965..33db895d..6a..ff15"), 0, strlen("558bec6a..68........68........64a1........50648925........83ec..5356578965..33db895d..6a..ff15"), pIsMatch) && *pIsMatch)
	{
		return true;
		/*if (PE.findSignature(PE.getOverlayOffset(), 0x80, "'GEA'") != -1)
		{
			bDetected = 1;
		}*/
	}
	else if (IsMatchEPSig(reinterpret_cast<const std::uint8_t *>("81ec........5356575568........6a..6a..ff15........8bf0ff15........3d........75..56"), 0, strlen("81ec........5356575568........6a..6a..ff15........8bf0ff15........3d........75..56"), pIsMatch) && *pIsMatch)
	{
		*pIsMatch = false;
		//OWS9G1//但是内存中是8e5753394731
		if (IsMatchOverlaySig(reinterpret_cast<const std::uint8_t *>("8e5753394731"), 22, strlen("8e5753394731"), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}

	return IsMatchEPSig(reinterpret_cast<const std::uint8_t *>("558bec81ec........5356576a..ff15........68........ff15........85c074..6a..a1........50ff15........8bf06a..56ff15"), 0, strlen("558bec81ec........5356576a..ff15........68........ff15........85c074..6a..a1........50ff15........8bf06a..56ff15"), pIsMatch);
}

void CGenteeUnpacker::PrintInfo()
{
	//printf("Summary: installer: Gentee Installer(-)[-]\n");
	printf("Gentee Installer\n");
}

INSTALLER_TYPE CGenteeUnpacker::GetEnumType()
{
	return INST_GENTEE;
}

bool CGenteeUnpacker::Unpack(const char* szTarFile, const char* szDstPath)
{
	return true;
}