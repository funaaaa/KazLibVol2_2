#include "LineFlashLight.h"
#include"../KazLibrary/Helper/KazHelper.h"

LineFlashLight::LineFlashLight() :finishFlag(true)
{
	RESOURCE_HANDLE lHandle = boxR.CreateConstBuffer(sizeof(DirectX::XMFLOAT4), "XMFLOAT4", GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA);
	DirectX::XMFLOAT4 colorData = { 1.0f,0.0f,0.0f,0.0f };
	boxR.TransData(&colorData, lHandle, "XMFLOAT4");
}

void LineFlashLight::Init(const std::vector<KazMath::Vec3<float>*> &POS_ARRAY, int TIMER)
{
	posArray = POS_ARRAY;

	//�S���̎Z�o
	float distance = 0.0f;
	for (int i = 0; i < posArray.size(); ++i)
	{
		if (i + 1 < posArray.size())
		{
			distance += posArray[i + 1]->Distance(*posArray[i]);
		}
		else
		{
			break;
		}
	}
	//1Flame�̃X�s�[�h
	speed = distance / static_cast<float>(TIMER);

	lineIndex = 0;
	boxR.data.transform.pos = *posArray[lineIndex];
	boxR.data.transform.scale = { 5.0f,5.0f,5.0f };
	boxR.data.color = { 0,255,0,255 };
	finishFlag = false;
	boxR.data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
}

void LineFlashLight::Finalize()
{
	finishFlag = true;
}

void LineFlashLight::Update()
{
	if (!finishFlag)
	{
		//�{�b�N�X���ǂ̕����Ɍ����킹�邩���߂�
		CheckWhereToGo();


		if (KazHelper::IsitInAnArray(lineIndex, posArray.size()))
		{
			if (useXPosFlag)
			{
				boxR.data.transform.pos.y = posArray[lineIndex]->y;
				boxR.data.transform.pos.z = posArray[lineIndex]->z;
			}
			else if (useYPosFlag)
			{
				boxR.data.transform.pos.x = posArray[lineIndex]->x;
				boxR.data.transform.pos.z = posArray[lineIndex]->z;
			}
			else if (useZPosFlag)
			{
				boxR.data.transform.pos.x = posArray[lineIndex]->x;
				boxR.data.transform.pos.y = posArray[lineIndex]->y;
			}
		}


		//�{�b�N�X�𓮂���
		if (nowPos != nullptr)
		{
			*nowPos += speed;
		}

		bool lIsReachToGoalDistanceFlag = false;
		float lDistance = CaluDisntace();

		lIsReachToGoalDistanceFlag = lDistance <= 0.0f;

		//���𒴂����玟�̐��ɍs��
		if (nowPos != nullptr && nextPos != nullptr && lIsReachToGoalDistanceFlag)
		{
			++lineIndex;

			int lLoopCount = 0;

			//��x�ɐ����z�����񐔂�100��ȉ�&&�����z���Ȃ����������[�v���I����
			while (lDistance <= 0.0f)
			{
				//���𒴂����͈͂Ƀ{�b�N�X������̂Œ����������C������
				if (lDistance <= -0.1f)
				{
					float subDistance = posArray[lineIndex]->Distance(boxR.data.transform.pos);
					boxR.data.transform.pos = *posArray[lineIndex];
					CheckWhereToGo();
					lDistance = CaluDisntace();
					*nowPos += subDistance;
				}
				//�҂����肮�炢�̈ʒu�Ƀ{�b�N�X������ꍇ�͂Ȃɂ����Ȃ�
				else
				{
					//�Ō�̐���_�Ȃ�I������
					if (!KazHelper::IsitInAnArray(lineIndex + 1, posArray.size()))
					{
						finishFlag = true;
						break;
					}
					break;
				}

				//�Ō�̐���_�Ȃ�I������
				if (!KazHelper::IsitInAnArray(lineIndex + 1, posArray.size()))
				{
					finishFlag = true;
					break;
				}

				++lLoopCount;

				//�������[�v�h�~
				if (100 <= lLoopCount)
				{
					assert(0);
				}
			}
		}
	}
}

void LineFlashLight::Draw()
{
	if (!finishFlag)
	{
		boxR.Draw();
	}
}

bool LineFlashLight::IsFinish()
{
	return finishFlag;
}

void LineFlashLight::CheckWhereToGo()
{
	if (KazHelper::IsitInAnArray(lineIndex + 1, posArray.size()))
	{
		KazMath::Vec3<float> lDistance = *posArray[lineIndex + 1] - *posArray[lineIndex];


		useXPosFlag = false;
		useYPosFlag = false;
		useZPosFlag = false;

		if (0.1f <= abs(lDistance.x))
		{
			//X���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
			if (signbit(lDistance.x) && !signbit(speed))
			{
				speed *= -1.0f;
			}
			//X���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
			else if (!signbit(lDistance.x) && signbit(speed))
			{
				speed *= -1.0f;
			}
			nowPos = &boxR.data.transform.pos.x;
			nextPos = &posArray[lineIndex + 1]->x;

			useXPosFlag = true;
		}
		else if (0.1f <= abs(lDistance.y))
		{
			//Y���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
			if (signbit(lDistance.y) && !signbit(speed))
			{
				speed *= -1.0f;
			}
			//Y���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
			else if (!signbit(lDistance.y) && signbit(speed))
			{
				speed *= -1.0f;
			}
			nowPos = &boxR.data.transform.pos.y;
			nextPos = &posArray[lineIndex + 1]->y;

			useYPosFlag = true;
		}
		else if (0.1f <= abs(lDistance.z))
		{
			//Z���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
			if (signbit(lDistance.z) && !signbit(speed))
			{
				speed *= -1.0f;
			}
			//Z���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
			else if (!signbit(lDistance.z) && signbit(speed))
			{
				speed *= -1.0f;
			}
			nowPos = &boxR.data.transform.pos.z;
			nextPos = &posArray[lineIndex + 1]->z;

			useZPosFlag = true;
		}
	}
}

float LineFlashLight::CaluDisntace()
{
	if (signbit(*nextPos))
	{
		return *nowPos - *nextPos;
	}
	else
	{
		return *nextPos - *nowPos;
	};
}
