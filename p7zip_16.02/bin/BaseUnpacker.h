#pragma once
#include "stdafx.h"

class CBaseUnpacker
{
public:
	CBaseUnpacker();
	~CBaseUnpacker();

	uint32_t GetUnpackPriority();

protected:
	uint32_t m_uPriority;
};