#pragma once

/// <summary>
/// �p�������N���X���V���O���g���ɂ��܂�
/// </summary>
/// <typeparam name="_T">�p������N���X</typeparam>
template <typename _T>
class ISingleton 
{
protected:
	ISingleton() {}
	virtual ~ISingleton() {}
	ISingleton(const ISingleton &r) {}
	ISingleton &operator=(const ISingleton &r) {}

public:
	static _T *Instance()
	{
		static _T inst;
		return &inst;
	};
};