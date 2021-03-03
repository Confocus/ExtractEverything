// MainAr.cpp

#include "StdAfx.h"

#ifdef _WIN32
#include "../../../../C/DllSecur.h"
#endif

#include "../../../Common/MyException.h"
#include "../../../Common/StdOutStream.h"

#include "../../../Windows/ErrorMsg.h"
#include "../../../Windows/NtCheck.h"

#include "../Common/ArchiveCommandLine.h"
#include "../Common/ExitCode.h"

#include "ConsoleClose.h"
#ifdef _WIN32
#include "D:/WangTeng/code/pe-parse-master2/pe-parse-master/pe-parser-library/global.h"//todo：记得改为相对路径
#else
#include "global.h"
#endif

using namespace NWindows;

CStdOutStream *g_StdStream = NULL;
CStdOutStream *g_ErrStream = NULL;
//
//#ifdef _WIN32
//extern "C" int _cdecl UnpackMain(
//	//#ifndef _WIN32
//	int numArgs, char *args[]
//	//#endif
//);
//#else//7z16.02
//extern "C" int UnpackMain(
//	//#ifndef _WIN32
//	int numArgs, char *args[]
//	//#endif
//);
//#endif

#ifdef _WIN32
extern "C" int _cdecl UnpackBy7z(
	//#ifndef _WIN32
	int numArgs, char *args[]
	//#endif
);
#else//7z16.02
extern "C" int UnpackBy7z(
	//#ifndef _WIN32
	int numArgs, char *args[]
	//#endif
);
#endif


static const char * const kException_CmdLine_Error_Message = "Command Line Error:";
static const char * const kExceptionErrorMessage = "ERROR:";
static const char * const kUserBreakMessage  = "Break signaled";
static const char * const kMemoryExceptionMessage = "ERROR: Can't allocate required memory!";
static const char * const kUnknownExceptionMessage = "Unknown Error";
static const char * const kInternalExceptionMessage = "\n\nInternal Error #";

static void FlushStreams()
{
  if (g_StdStream)
    g_StdStream->Flush();
}

static void PrintError(const char *message)
{
  FlushStreams();
  if (g_ErrStream)
    *g_ErrStream << "\n\n" << message << endl;
}

#define NT_CHECK_FAIL_ACTION *g_StdStream << "Unsupported Windows version"; return NExitCode::kFatalError;

extern "C" PERROR_MSG_CALLBACK g_ErrorMsgCallbackFunc;

#define MAX_ERROR_MSG_SIZE	0x100

#ifdef _WIN32
_declspec(thread) char szErrMsg[MAX_ERROR_MSG_SIZE] = { 0 };
#else
__thread char szErrMsg[MAX_ERROR_MSG_SIZE] = { 0 };
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

void RecordErrorMsg(const char* pErrorMsg, void* pReserve)
{
	if (!g_ErrorMsgCallbackFunc)
	{
		return;
	}

	int nSize = min(MAX_ERROR_MSG_SIZE - 1, strlen(pErrorMsg));
#ifdef _WIN32
	memcpy_s((void*)szErrMsg, nSize, pErrorMsg, nSize);
#else
	memcpy(szErrMsg, pErrorMsg, nSize);
#endif
	szErrMsg[nSize] = 0;

	g_ErrorMsgCallbackFunc(pErrorMsg, pReserve);//都跑到一个地址来执行
	szErrMsg[0] = 0;
}

#ifdef _WIN32
extern "C" int _cdecl UnpackMain(
	int numArgs, char *args[]
)
#else//Linux:
extern "C" int UnpackMain(
	int numArgs, char *args[]
)
#endif
{
	int res = 0;

	try
	{
		res = UnpackBy7z(
			//#ifndef _WIN32
			numArgs, args
			//#endif
		);
	}
	catch (const CNewException &)
	{
		//PrintError(kMemoryExceptionMessage);
		RecordErrorMsg(kMemoryExceptionMessage, NULL);
		return (NExitCode::kMemoryError);
	}
	catch (const NConsoleClose::CCtrlBreakException &)
	{
		//PrintError(kUserBreakMessage);
		RecordErrorMsg(kUserBreakMessage, NULL);
		return (NExitCode::kUserBreak);
	}
#ifdef _WIN32
	catch (const CMessagePathException &e)
	{
		//PrintError(kException_CmdLine_Error_Message);
		RecordErrorMsg(kException_CmdLine_Error_Message, NULL);
		/*if (g_ErrStream)
			*g_ErrStream << e << endl;*/
		return (NExitCode::kUserError);
	}
#endif
	catch (const CSystemException &systemError)
	{
		if (systemError.ErrorCode == E_OUTOFMEMORY)
		{
			//PrintError(kMemoryExceptionMessage);
			RecordErrorMsg(kMemoryExceptionMessage, NULL);
			return (NExitCode::kMemoryError);
		}
		if (systemError.ErrorCode == E_ABORT)
		{
			//PrintError(kUserBreakMessage);
			RecordErrorMsg(kUserBreakMessage, NULL);
			return (NExitCode::kUserBreak);
		}
		if (g_ErrStream)
		{
			/*PrintError("System ERROR:");
			*g_ErrStream << NError::MyFormatMessage(systemError.ErrorCode) << endl;*/
			RecordErrorMsg("System Error", NULL);
		}
		return (NExitCode::kFatalError);
	}
	catch (NExitCode::EEnum &exitCode)
	{
		RecordErrorMsg(kInternalExceptionMessage, NULL);
		FlushStreams();
		if (g_ErrStream)
			*g_ErrStream << kInternalExceptionMessage << exitCode << endl;
		return (exitCode);
	}
	catch (const UString &s)
	{
		RecordErrorMsg(kExceptionErrorMessage, NULL);
		/*if (g_ErrStream)
		{
			PrintError(kExceptionErrorMessage);
			*g_ErrStream << s << endl;
		}*/
		return (NExitCode::kFatalError);
	}
	catch (const AString &s)
	{
		RecordErrorMsg(kExceptionErrorMessage, NULL);
		/*if (g_ErrStream)
		{
			PrintError(kExceptionErrorMessage);
			*g_ErrStream << s << endl;
		}*/
		return (NExitCode::kFatalError);
	}
	catch (const char *s)
	{
		RecordErrorMsg(kExceptionErrorMessage, NULL);
		/*if (g_ErrStream)
		{
			PrintError(kExceptionErrorMessage);
			*g_ErrStream << s << endl;
		}*/
		return (NExitCode::kFatalError);
	}
	catch (const wchar_t *s)
	{
		RecordErrorMsg(kExceptionErrorMessage, NULL);
		/*if (g_ErrStream)
		{
			PrintError(kExceptionErrorMessage);
			*g_ErrStream << s << endl;
		}*/
		return (NExitCode::kFatalError);
	}
	catch (int t)
	{
		RecordErrorMsg(kInternalExceptionMessage, NULL);
		/*if (g_ErrStream)
		{
			FlushStreams();
			*g_ErrStream << kInternalExceptionMessage << t << endl;
			return (NExitCode::kFatalError);
		}*/
	}
	catch (...)
	{
		RecordErrorMsg(kUnknownExceptionMessage, NULL);
		//PrintError(kUnknownExceptionMessage);
		return (NExitCode::kFatalError);
	}

	return res;
}
//
//int MY_CDECL main1
//(
//  //#ifndef _WIN32
//  int numArgs, char *args[]
//  //#endif
//)
//{
//	//暂时屏蔽，移动到UnpackMain之中
//	 /*g_ErrStream = &g_StdErr;
//	 g_StdStream = &g_StdOut;*/
//
//  NT_CHECK
//
//  NConsoleClose::CCtrlHandlerSetter ctrlHandlerSetter;
//  int res = 0;
//  
//  try
//  {
//    #ifdef _WIN32
//    My_SetDefaultDllDirectories();
//    #endif
//
//    res = UnpackMain(
//    //#ifndef _WIN32
//    numArgs, args
//    //#endif
//    );
//  }
//  catch(const CNewException &)
//  {
//    PrintError(kMemoryExceptionMessage);
//    return (NExitCode::kMemoryError);
//  }
//  catch(const NConsoleClose::CCtrlBreakException &)
//  {
//    PrintError(kUserBreakMessage);
//    return (NExitCode::kUserBreak);
//  }
//  catch(const CMessagePathException &e)
//  {
//    PrintError(kException_CmdLine_Error_Message);
//    if (g_ErrStream)
//      *g_ErrStream << e << endl;
//    return (NExitCode::kUserError);
//  }
//  catch(const CSystemException &systemError)
//  {
//    if (systemError.ErrorCode == E_OUTOFMEMORY)
//    {
//      PrintError(kMemoryExceptionMessage);
//      return (NExitCode::kMemoryError);
//    }
//    if (systemError.ErrorCode == E_ABORT)
//    {
//      PrintError(kUserBreakMessage);
//      return (NExitCode::kUserBreak);
//    }
//    if (g_ErrStream)
//    {
//      PrintError("System ERROR:");
//      *g_ErrStream << NError::MyFormatMessage(systemError.ErrorCode) << endl;
//    }
//    return (NExitCode::kFatalError);
//  }
//  catch(NExitCode::EEnum &exitCode)
//  {
//	  FlushStreams();
//	  if (g_ErrStream)
//		*g_ErrStream << kInternalExceptionMessage << exitCode << endl;
//    return (exitCode);
//  }
//  catch(const UString &s)
//  {
//    if (g_ErrStream)
//    {
//      PrintError(kExceptionErrorMessage);
//      *g_ErrStream << s << endl;
//    }
//    return (NExitCode::kFatalError);
//  }
//  catch(const AString &s)
//  {
//    if (g_ErrStream)
//    {
//      PrintError(kExceptionErrorMessage);
//      *g_ErrStream << s << endl;
//    }
//    return (NExitCode::kFatalError);
//  }
//  catch(const char *s)
//  {
//    if (g_ErrStream)
//    {
//      PrintError(kExceptionErrorMessage);
//      *g_ErrStream << s << endl;
//    }
//    return (NExitCode::kFatalError);
//  }
//  catch(const wchar_t *s)
//  {
//    if (g_ErrStream)
//    {
//      PrintError(kExceptionErrorMessage);
//      *g_ErrStream << s << endl;
//    }
//    return (NExitCode::kFatalError);
//  }
//  catch(int t)
//  {
//    if (g_ErrStream)
//    {
//      FlushStreams();
//      *g_ErrStream << kInternalExceptionMessage << t << endl;
//      return (NExitCode::kFatalError);
//    }
//  }
//  catch(...)
//  {
//    PrintError(kUnknownExceptionMessage);
//    return (NExitCode::kFatalError);
//  }
//
//  return res;
//}
