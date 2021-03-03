#pragma once
#include <stdint.h>

class IUnPacker
{
public:
	/*
	�ӿ�˵��
	*/
	virtual int GetPackerType(const char* szFilePath, uint32_t* uFileType) = 0;
	virtual int GetPackerType(const char* szFileBuff, uint64_t uFileSizse, uint32_t* uFileType) = 0;
	virtual int UnPack(const char* szFilePath) = 0;
	virtual int UnPack(const char* szFileBuff, uint64_t uFileSizse) = 0;
};
