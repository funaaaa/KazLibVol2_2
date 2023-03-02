#include "AttackLog.h"

AttackLog::AttackLog()
{
}

void AttackLog::Init(const KazMath::Vec2<float> &POS, bool TIMEOVER_FLAG, int LOG_MAX)
{
	logArrayIndex = 0;
	logDataIndex = 0;
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].logIndex = -1;
	}

	pos = POS;

	useTimeOverFlag = TIMEOVER_FLAG;
	if (LOG_MAX == -1)
	{
		logMaxNum = static_cast<int>(logStringArray.size());
	}
	else
	{
		logMaxNum = LOG_MAX;
	}
}

void AttackLog::Finalize()
{
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].log.Finalize();
		logStringArray[i].logIndex = -1;;
	}
}

void AttackLog::Update()
{
	//���Ԍo�߂Ń��O������
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (!useTimeOverFlag)
		{
			break;
		}

		if (logStringArray[i].log.TimeOver() && 0 <= logStringArray[i].logIndex)
		{
			logStringArray[i].logIndex = -1;
			for (int logIndex = 0; logIndex < logStringArray.size(); ++logIndex)
			{
				--logStringArray[logIndex].logIndex;
			}
			--logDataIndex;
		}
	}

	//���O�̔p��
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (logStringArray[i].logIndex <= -1)
		{
			logStringArray[i].log.Finalize();
		}
	}

	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (0 <= logStringArray[i].logIndex)
		{
			logStringArray[i].log.Update(logStringArray[i].logIndex);
		}
	}
}

void AttackLog::Draw()
{
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].log.Draw();
	}
}

void AttackLog::WriteLog(const std::string &STRING, float FONT_SIZE)
{
	logStringArray[logArrayIndex].log.Init(pos, STRING, FONT_SIZE);
	logStringArray[logArrayIndex].logIndex = logDataIndex;

	//���O�̏��Ԃ��ő�l�܂œ��B�����炻�̍ő�l�̂܂܂ɂ���
	++logDataIndex;
	if (logMaxNum <= logDataIndex)
	{
		logDataIndex = static_cast<int>(logMaxNum - 1);
		//�ő�l�܂ōs������S�Ẵ��O�̃C���f�b�N�X��������A�����̍��W�����炷
		for (int i = 0; i < logMaxNum; ++i)
		{
			--logStringArray[i].logIndex;
		}
	}

	//���O�z��̓Y�������ő�l�܂œ��B������ŏ��̃��O���g����
	++logArrayIndex;
	if (logMaxNum <= logArrayIndex)
	{
		logArrayIndex = 0;
	}
}
