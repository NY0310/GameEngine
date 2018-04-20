/// <summary>
/// �q���Ƃ��ēo�^���邱�ƂŎ�����Lerp�������s��
/// ���c�@�N��
/// 2018/04/19
/// </summary>
#pragma once
#include "../SceneGraph/Node/GameObjectNode.h"
#include "../Math/Math.h"

namespace NYLibrary
{
	class Lerp : public GameObjectNode
	{
	public:
		//�n�_�A�I�_�A�v���C����
		Lerp(float start, float end, float playTime) :
			start(start), end(end), frameCnt(0.0f), playTime(playTime) {}
		
		//�X�V
		void Update()
		{
			if (useUpdateTime)
			{
				frameCnt++;


				time = frameCnt / playTime;
			}
			
			now = Math::Lerp(start, end, time);

			if (time >= 1.0f)
			{
				DisableUpdate();
				listener();
			}
		//	useUpdateTime = true;
		}
		//���Ԃ����Z�b�g
		void TimeReset()
		{
			frameCnt = 0;
		}

		//���݂̒l���擾
		float GetNow() { return now; }
		//�i�s�x���擾
		float GetTime() { return time; }
		//�i�s�x��ݒ�
		void SetTime(float time) { this->time = time; useUpdateTime = false; }
		//�ŏ���ݒ�
		void SetStart(float start) { this->start = start; useUpdateTime = true; }
		//�ŏI��ݒ�
		void SetEnd(float end) { this->end = end; }
		//���[�v���I��������
		bool IsLerpEnd() { return time >= 1; DisableUpdate(); }
		//�������󂯎��
		void addListener(std::function<void()> listener) {
			this->listener = listener;
		}
		//�N���A
		void Clear() { frameCnt = 0; now = start; Update(); }
	private:
		float start;//�n�_
		float end;//�I�_
		float frameCnt;//�t���[���J�E���g
		const float playTime;//���s����
		float now;//����
		float time;//�i�s�x
		std::function<void()> listener = [&]() {};				   //�����_�����󂯎��
		bool useUpdateTime;
	};
}