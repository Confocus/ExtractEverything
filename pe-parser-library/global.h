#pragma once
//************************************
// Function: 消息处理回调函数
// Parameter: const char* szMessage：传入的相关错误消息
// Parameter: void* pReserve：保留字段以后扩展用，暂时不用管，传NULL就可以
//************************************
typedef void(*PERROR_MSG_CALLBACK)(const char* szMessage, void* pReserve);
extern "C" PERROR_MSG_CALLBACK g_ErrorMsgCallbackFunc;