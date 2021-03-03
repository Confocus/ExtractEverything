#pragma once
typedef void(*PERROR_MSG_CALLBACK)(const char* szMessage, void* pReserve);
extern "C" PERROR_MSG_CALLBACK g_ErrorMsgCallbackFunc;
