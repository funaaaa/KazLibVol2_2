#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

/// <summary>
/// ��������̓�̒l���g���Đ��`�⊮���s��
/// </summary>
class ComputeLerp
{
public:
	ComputeLerp(const ResouceBufferHelper::BufferData &BASE_DATA, const ResouceBufferHelper::BufferData &LERP_DATA);
	void Compute();

private:
	ResouceBufferHelper computeHelper;
};

