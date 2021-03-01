#include "SetupFactory.h"
#define SETUP_FACTORY_EPSIG_1		"558bec6aff68..61400068..43400064a1000000005064892500000000"
#define SETUP_FACTORY_EPSIG_5		"558bec6a..68........68........64a1........50648925........83ec..5356578965..ff15"
#define SETUP_FACTORY_EPSIG_6		"64a1........558bec6a..68........68........50648925........83ec..5356578965..ff15"
#define SETUP_FACTORY_OVERLAYSIG_1	"e0e0e1e1e2e2e3e3e4e4e5e5e6e6e7e7"
#define SETUP_FACTORY_OVERLAYSIG_2	"e0e1e2e3e4e5e6"

CSetupFactory::CSetupFactory()
{
	m_szUnpackerName = "setu_factory";
	ResetUnpackMethod();
}

CSetupFactory::~CSetupFactory()
{
}

bool CSetupFactory::IsMatchPackerSig(bool *pIsMatch)
{
	CHECK_NULLPTR_ERR(pIsMatch);
	*pIsMatch = false;
	if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(SETUP_FACTORY_EPSIG_1), 0, strlen(SETUP_FACTORY_EPSIG_1), pIsMatch) && *pIsMatch)
	{
		/*if (PE.compareEP("90614000", 6))
		{
			sVersion = "6.0.0.3";
		}
		else
		{
			sVersion = "6.x";
		}*/
		return true;
	}
	else if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(SETUP_FACTORY_OVERLAYSIG_1), 0, strlen(SETUP_FACTORY_OVERLAYSIG_1), pIsMatch) && *pIsMatch)
	{
		/*if (PE.compareEP("e8$$$$$$$$558bec83ec..a1........8365....8365....5357bf........3bc7bb........74..85c374..f7d0a3........eb"))
		{
			sVersion = "8.0";
			bDetected = 1;
		}
		else if (PE.compareEP("e8$$$$$$$$8bff558bec83ec..a1........8365....8365....5357bf........bb........3bc774..85c374..f7d0a3........eb"))
		{
			sVersion = "9.0";
			bDetected = 1;
		}
		else if (PE.compareEP("4883ec..e8$$$$$$$$48895c24..574883ec..488b05........48836424....48bf................483bc774..48f7d0488905........eb"))
		{
			sVersion = "9.5";
			bDetected = 1;
		}*/
	}
	else if (IsMatchOverlaySig(reinterpret_cast<const uint8_t*>(SETUP_FACTORY_OVERLAYSIG_2), 0, strlen(SETUP_FACTORY_OVERLAYSIG_2), pIsMatch) && *pIsMatch)
	{
		if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(SETUP_FACTORY_EPSIG_5), 0, strlen(SETUP_FACTORY_EPSIG_5), pIsMatch) && *pIsMatch)
		{
			return true;
		}
		else if (IsMatchEPSig(reinterpret_cast<const uint8_t*>(SETUP_FACTORY_EPSIG_6), 0, strlen(SETUP_FACTORY_EPSIG_6), pIsMatch) && *pIsMatch)
		{
			return true;
		}
	}

	/*if (bDetected)
	{
		var sInfoString = PE.getVersionStringInfo("Comments");
		if (sInfoString.indexOf("Trial") >= 0)
		{
			sOptions = "trial";
		}
	}*/

	return true;
}

void CSetupFactory::PrintInfo()
{
	//printf("Summary: installer: Setup Factory[-]\n");
	printf("Setup Factory\n");
}

INSTALLER_TYPE CSetupFactory::GetEnumType()
{
	return INST_SETUP_FACTORY;
}

bool CSetupFactory::Unpack(const char* szTarFile, const char* szDstPath)
{
	printf("Can not unpack..\n");
	return true;
}