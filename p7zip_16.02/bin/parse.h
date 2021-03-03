/*
The MIT License (MIT)

Copyright (c) 2013 Andrew Ruef

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string.h>
#ifdef _WIN32
#include "../../stdafx.h"
#else
#include "stdafx.h"
#endif
#include "nt-headers.h"
#include "to_string.h"

#define NO_OVERLAY_SIGNAL	static_cast<uint32_t>(-1)

//get the type of x and remove the reference of it.
#ifdef _MSC_VER
#define __typeof__(x) std::remove_reference<decltype(x)>::type
#endif

#define PE_ERR(x)																	\
  err = static_cast<pe_err>(x);														\
  err_loc.assign(__func__);															\
  err_loc += ":" + to_string<std::uint32_t>(__LINE__, std::dec);

#define CHECK_NULLPTR_ERR(x)														\
	if(nullptr == x){																\
		err_loc.assign(__func__);													\
		err_loc += ":" + to_string<std::uint32_t>(__LINE__, std::dec);				\
		/*std::cout << "NULLPTR_ERR :" << err_loc << std::endl;*/					\
		return false;}

#define CHECK_NULLPTR_RET(x)														\
	if(nullptr == x){																\
		return;}

#define CHECK_NULLSIZE_ERR(x)														\
	if(0 >= x){																		\
		err_loc.assign(__func__);													\
		err_loc += ":" + to_string<std::uint32_t>(__LINE__, std::dec);				\
		/*std::cout << "NULLSIZE_ERR :" << err_loc << std::endl;*/					\
		return false;}

#define CHECK_RET_FALSE(x)															\
	if(0 == x){																		\
		err_loc.assign(__func__);													\
		err_loc += ":" + to_string<std::uint32_t>(__LINE__, std::dec);				\
		std::cout << "RET_FALSE :" << err_loc << std::endl;							\
		return false;}

#define DELETE_PTR(x)																\
	if(x){																			\
		delete x;																	\
		x = nullptr;}															

#define DELETE_ARRAY(x)																\
	if(x){																			\
		delete []x;																	\
		x = nullptr;}													

#define INIT_ARRAY(x, n)															\
	if(x){																			\
		memset(x, 0, n);															\
	}

#define READ_WORD(b, o, inst, member)									  \
  if (!readWord(b, o + _offset(__typeof__(inst), member), inst.member)) { \
    PE_ERR(PEERR_READ);                                                   \
    return false;                                                         \
  }

#define READ_DWORD(b, o, inst, member)                                     \
  if (!readDword(b, o + _offset(__typeof__(inst), member), inst.member)) { \
    PE_ERR(PEERR_READ);                                                    \
    return false;                                                          \
  }

#define READ_QWORD(b, o, inst, member)                                     \
  if (!readQword(b, o + _offset(__typeof__(inst), member), inst.member)) { \
    PE_ERR(PEERR_READ);                                                    \
    return false;                                                          \
  }

#define READ_BYTE(b, o, inst, member)                                     \
  if (!readByte(b, o + _offset(__typeof__(inst), member), inst.member)) { \
    PE_ERR(PEERR_READ);                                                   \
    return false;                                                         \
  }

#define TEST_MACHINE_CHARACTERISTICS(h, m, ch) \
  ((h.FileHeader.Machine == m) && (h.FileHeader.Characteristics & ch))

typedef bool (*PFUNC_TRAVERSE_NESTED_FILE)(void*);

typedef struct CB_NESTED_FILE_INFO
{
public:
	CB_NESTED_FILE_INFO():
		uOffset(0),
		uRsrcSize(0),
		uRsrcFileOffset(0),
		uFuncBuffSize(0),
		pFuncBuff(nullptr)
	{
	}

	~CB_NESTED_FILE_INFO()
	{
	}

	uint32_t uOffset;
	uint32_t uRsrcSize;
	uint32_t uRsrcFileOffset;
	uint32_t uFuncBuffSize;
	char* pFuncBuff;
}CB_NESTED_FILE_INFO, *PCB_NESTED_FILE_INFO;

enum VERSION_INFO
{
	ProductName = 0,
	CompanyName = 1
};

namespace peparse {

	typedef std::uint32_t RVA;
	typedef std::uint64_t VA;

	struct buffer_detail;

	typedef struct _bounded_buffer {
		std::uint8_t *buf;
		std::uint32_t bufLen;
		bool copy;
		bool swapBytes;
		buffer_detail *detail;
	} bounded_buffer;

	struct resource {
		resource()
			: type(0), name(0), lang(0), codepage(0), RVA(0), size(0), buf(nullptr) {
		}

		std::string type_str;
		std::string name_str;
		std::string lang_str;
		std::uint32_t type;
		std::uint32_t name;
		std::uint32_t lang;
		std::uint32_t codepage;
		std::uint32_t RVA;
		std::uint32_t size;
		bounded_buffer *buf;
	};

#ifndef _PEPARSE_WINDOWS_CONFLICTS
	// http://msdn.microsoft.com/en-us/library/ms648009(v=vs.85).aspx
	enum resource_type {
		RT_CURSOR = 1,
		RT_BITMAP = 2,
		RT_ICON = 3,
		RT_MENU = 4,
		RT_DIALOG = 5,
		RT_STRING = 6,
		RT_FONTDIR = 7,
		RT_FONT = 8,
		RT_ACCELERATOR = 9,
		RT_RCDATA = 10,
		RT_MESSAGETABLE = 11,
		RT_GROUP_CURSOR = 12, // MAKEINTRESOURCE((ULONG_PTR)(RT_CURSOR) + 11)
		RT_GROUP_ICON = 14,   // MAKEINTRESOURCE((ULONG_PTR)(RT_ICON) + 11)
		RT_VERSION = 16,
		RT_DLGINCLUDE = 17,
		RT_PLUGPLAY = 19,
		RT_VXD = 20,
		RT_ANICURSOR = 21,
		RT_ANIICON = 22,
		RT_HTML = 23,
		RT_MANIFEST = 24
	};
#endif

	enum pe_err {
		PEERR_NONE = 0,
		PEERR_MEM = 1,
		PEERR_HDR = 2,
		PEERR_SECT = 3,
		PEERR_RESC = 4,
		PEERR_SECTVA = 5,
		PEERR_READ = 6,
		PEERR_OPEN = 7,
		PEERR_STAT = 8,
		PEERR_MAGIC = 9,
		PEERR_BUFFER = 10,
		PEERR_ADDRESS = 11,
		PEERR_SIZE = 12,
		PEERR_INIT = 13,
	};

	enum nested_file_type {
		NESTED_TYPE_NONE = 0,
		NESTED_TYPE_PE = 1,
		NESTED_TYPE_ZIP = 2,
	};

	struct parsed_pe_internal;

	typedef struct _pe_header {
		dos_header dos;
		rich_header rich;
		nt_header_32 nt;
	} pe_header;

	typedef struct _parsed_pe {
		bounded_buffer *fileBuffer;
		parsed_pe_internal *internal;
		pe_header peHeader;
	} parsed_pe;

	bool readByte(bounded_buffer *b, std::uint32_t offset, std::uint8_t &out);
	bool readWord(bounded_buffer *b, std::uint32_t offset, std::uint16_t &out);
	bool readDword(bounded_buffer *b, std::uint32_t offset, std::uint32_t &out);
	bool readQword(bounded_buffer *b, std::uint32_t offset, std::uint64_t &out);
	bool readChar16(bounded_buffer *b, std::uint32_t offset, char16_t &out);

	bounded_buffer *readFileToFileBuffer(const char *filePath);
	bounded_buffer *makeBufferFromPointer(std::uint8_t *data, std::uint32_t sz);
	bounded_buffer *splitBuffer(bounded_buffer *b, std::uint32_t from, std::uint32_t to);
	void deleteBuffer(bounded_buffer *b);
	uint64_t bufLen(bounded_buffer *b);

	// Resolve a Rich header product id / build number pair to a known
	// product name
	typedef std::pair<std::uint16_t, std::uint16_t> ProductKey;
	const std::string &GetRichObjectType(std::uint16_t prodId);
	const std::string &GetRichProductName(std::uint16_t buildNum);

	// get parser error status as integer
	std::uint32_t GetPEErr();

	// get parser error status as string
	std::string GetPEErrString();

	// get parser error location as string
	std::string GetPEErrLoc();

	// get a PE parse context from a file
	parsed_pe *ParsePEFromFile(const char *filePath);

	parsed_pe *ParsePEFromPointer(std::uint8_t *buffer, std::uint32_t sz);
	parsed_pe *ParsePEFromBuffer(bounded_buffer *buffer);

	// destruct a PE context
	void DestructParsedPE(parsed_pe *p);

	// iterate over Rich header entries
	typedef int(*iterRich)(void *, const rich_entry &);
	void IterRich(parsed_pe *pe, iterRich cb, void *cbd);

	// iterate over the resources
	typedef int(*iterRsrc)(void *, const resource &);
	void IterRsrc(parsed_pe *pe, iterRsrc cb, void *cbd);

	// iterate over the imports by RVA and string
	typedef int(*iterVAStr)(void *,
		const VA &,
		const std::string &,
		const std::string &);
	void IterImpVAString(parsed_pe *pe, iterVAStr cb, void *cbd);

	// iterate over relocations in the PE file
	typedef int(*iterReloc)(void *, const VA &, const reloc_type &);
	void IterRelocs(parsed_pe *pe, iterReloc cb, void *cbd);

	// Iterate over symbols (symbol table) in the PE file
	typedef int(*iterSymbol)(void *,
		const std::string &,
		const std::uint32_t &,
		const std::int16_t &,
		const std::uint16_t &,
		const std::uint8_t &,
		const std::uint8_t &);
	void IterSymbols(parsed_pe *pe, iterSymbol cb, void *cbd);

	// iterate over the exports
	typedef int(*iterExp)(void *,
		const VA &,
		const std::string &,
		const std::string &);
	void IterExpVA(parsed_pe *pe, iterExp cb, void *cbd);

	// iterate over sections
	typedef int(*iterSec)(void *,
		const VA &,
		const std::string &,
		const image_section_header &,
		const bounded_buffer *);
	void IterSec(parsed_pe *pe, iterSec cb, void *cbd);

	// get byte at VA in PE
	bool ReadByteAtVA(parsed_pe *pe, VA v, std::uint8_t &b);

	// get entry point into PE
	bool GetEntryPoint(parsed_pe *pe, VA &v);

	// get machine as human readable string
	const char *GetMachineAsString(parsed_pe *pe);

	// get subsystem as human readable string
	const char *GetSubsystemAsString(parsed_pe *pe);

	// get a table or string by its data directory entry
	bool GetDataDirectoryEntry(parsed_pe *pe,
		data_directory_kind dirnum,
		std::vector<std::uint8_t> &raw_entry);

	//variable declare below:
	class CPEParser
	{
	public:
		CPEParser();
		~CPEParser();
		//************************************
		// Method:    Parse
		// FullName:  peparse::CPEParser::Parse
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: const char * szFilePath
		// Description：解析文件
		//************************************
		bool Parse(const char* szFilePath);

		//************************************
		// Method:    Clear
		// FullName:  peparse::CPEParser::Clear
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Description：清空解析文件生成的数据，下一次解析前必须调用以免内存泄漏
		//************************************
		void Clear();

		//************************************
		// Method:    GetBuffer
		// FullName:  peparse::CPEParser::GetBuffer
		// Access:    public 
		// Returns:   bool
		// Qualifier:
		// Parameter: const char * szFilePath
		// Description：解析文件，对于非PE格式如msi，Parse会失败返回，需要使用该函数加载数据
		//************************************
		bool GetNPEBuffer(const char* szFilePath);

		//************************************
		// Method:    GetBufferPtr
		// FullName:  peparse::CPEParser::GetBufferPtr
		// Access:    public 
		// Returns:   const uint8_t*
		// Qualifier:
		// Description：拿到由GetBuffer生成的内存数据的指针
		//************************************
		const char* GetNPEBufferPtr();

		//todo:m_pPE->m_pParsedPE->fileBuffer->buf change to GetBuffer()
		const uint8_t* GetBuffer();

		bool GetOverlaySize(uint32_t* pSize);

		bool GetOverlayOffset(uint32_t* nOffset);

		bool GetEPVirtOffset(uint32_t* uOffset);

		bool GetEPFileOffset(uint32_t * pOffset);

		bool GetPEFileSize(uint32_t* pFileSize);

		bool GetPEPESize(uint32_t* pFileSize);

		bool ReadDWord(uint32_t uOffset, uint32_t * pReadDWord);

		bool ReadByte(uint32_t uOffset, uint8_t * pReadByte);

		bool HasSecName(const char* szSecName) const;
		
		bool HasRsrcName(const char* szRsrcName) const;

		bool GetVersionInfoName(const VERSION_INFO eKey, std::string& sName);

		bool HasNestedFileInRsrc(const nested_file_type uType);

		bool GetNestedFileInRsrc(const nested_file_type uType, PFUNC_TRAVERSE_NESTED_FILE pFunc, char* pFuncBuff, const int32_t uFuncBuffSize);

		uint32_t GetRsrcNum() const;

		uint32_t GetRsrcType(const uint32_t uIndex) const;

		uint32_t GetRsrcSize(const uint32_t uIndex) const;

		uint32_t GetRsrcOffset(const uint32_t uIndex) const;

		std::string GetRsrcStrName(const uint32_t uIndex) const;

		const char* GetTargetFilePath() const;

	//unused：
		bool IsOverlayPresent();

		bool GetManifest();

		bool VirtOffsetToFileOffset(uint32_t uVirtOffset, uint32_t * pFileOffset);

	private:
		bool HasNestedPE();

		bool HasNestedWinzip();

		bool TraverseNestedFile(const nested_file_type FileType, bool bMatchOnce = true, PFUNC_TRAVERSE_NESTED_FILE pFunc = nullptr, char* pFuncBuff = nullptr, const int32_t uFuncBuffSize = 0);

		bool IsMatchPESig(const char* pbuff, const uint32_t uRsrcSize);

		bool IsMatchWinZipSig(const char* pbuff, const uint32_t uRsrcSize);

		bool HasNestedZip();

		inline uint8_t DecToHex(uint8_t uDec);

		bool SearchVersionInfoNameInBuffer(const std::uint8_t* buf, const uint32_t bufLen, const VERSION_INFO eKey, std::string& sName);

		uint8_t DecToHex(std::uint8_t uDec) const
		{
			if (0 <= uDec && uDec <= 9)
			{
				return uDec + '0';
			}

			return uDec - 10 + 'a';
		}

	public:
		parsed_pe *m_pParsedPE;
		char* m_pBuf;
		uint32_t m_uBufSize;

	private:
		const char* m_szTargetFilePath;
		uint32_t m_uRsrcNum;
		uint32_t m_uSecNum;
		uint64_t m_uFileSize;
		uint64_t m_uPESize;
		uint64_t m_uOverlaySize;
		uint64_t m_uOverlayOffset;
		uint64_t m_uEPVirtOffset;
		uint64_t m_uEPFileOffset;
		std::string m_sProductName;
		std::string m_sCompanyName;
	};

	typedef bool(CPEParser::*PFUNC_MATCH_FILE_SIG)(const char* pRsrcBuff, const uint32_t uRsrcSize);

	/*extern const char* m_szTargetFilePath;
	extern parsed_pe *m_pParsedPE;
	extern uint32_t m_uSecNum;
	extern uint64_t m_uFileSize;
	extern uint64_t m_uPESize;*/
} // namespace peparse
