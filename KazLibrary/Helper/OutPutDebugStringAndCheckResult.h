#pragma once
#include"../DirectXCommon/Base.h"
#include<string>
/*
*��Ȗ���
�������ȒP�ɁA�o�͌��ɕ\��������
*/
//�o�����ʂɂ���āA�����ɓ�����������ƌ��ʂ̕�������Ȃ��ďo�͂���
HRESULT CheckResult(std::string OUTPUT_DEBUG_STRING, HRESULT RESULT);

void OutPutStartDebugString(std::string OUTPUT_DEBUG_STRING);
void OutPutEndDebugString(std::string OUTPUT_DEBUG_STRING);

HRESULT CheckResult(HRESULT RESULT, std::string SUCCEED_STRING, std::string FAIL_STRING);
void FailCheck(std::string FAIL_STRING);
void SucceedCheck(std::string SUCCEED_STRING);
void ErrorCheck(std::string ERROR_STRING);