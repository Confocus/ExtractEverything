#include "BaseUnpacker.h"

CBaseUnpacker::CBaseUnpacker():
	m_uPriority(0)
{
}

CBaseUnpacker::~CBaseUnpacker()
{
}

uint32_t CBaseUnpacker::GetUnpackPriority()
{
	return m_uPriority;
}