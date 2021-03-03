#pragma once
#include "stdafx.h"
#include "ipackermatch.h"
#include "parse.h"
#include "FileAssist.h"

using namespace peparse;

class C7zOffical:
	public CFileAssist
{
public:
	C7zOffical();
	~C7zOffical();

public:
	bool IsPacked(const char* szFilePath);
	bool Unpack(const char* szTarFile, const char* szDstPath);

	/*
	private:
		bool CreateOutputDir(const std::string & sTarFile, const std::string & sDstFile, const bool bDefaultOutput, std::string & sOutputFile);
		bool GetTimeStamp(std::string& sTimeStamp);*/
};