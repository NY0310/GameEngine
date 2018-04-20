#pragma once
#include "../../GameScene/Charactor/Target.h"


class PlayUI : public Target
{
public:
	PlayUI();
	//������
	void Initialize();
	// �R�s�[�R���X�g���N�^�֎~
	PlayUI(const PlayUI&) = delete;
	// ����֎~
	PlayUI& operator=(const PlayUI&) = delete;
	//�X�V
	void Update();
	//��ꂽ��
	void OnBreakEnd();
private:
	//���񂾂Ƃ�
	void OnDied();
	//HP���
	static const int MAX_HP = 10;

};