#pragma once
//************************************
// Function: ��Ϣ����ص�����
// Parameter: const char* szMessage���������ش�����Ϣ
// Parameter: void* pReserve�������ֶ��Ժ���չ�ã���ʱ���ùܣ���NULL�Ϳ���
//************************************
typedef void(*PERROR_MSG_CALLBACK)(const char* szMessage, void* pReserve);
extern "C" PERROR_MSG_CALLBACK g_ErrorMsgCallbackFunc;