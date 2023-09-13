#pragma once
#include"primitiveScene.h"
#include <array>

class selectScene
	:public primitiveScene
{
public:
	selectScene();
	~selectScene();

	//���\�[�X�̓ǂݍ���
	void loadResources() override;

	//������
	void initialize() override;

	//�p�����[�^�̃��Z�b�g
	void setParameter() override;

	//�X�V
	void updata() override;

	//�w�i�`��
	void drawBack() override;

	//�`��(3D)
	void draw3D() override;

	//�`��(2D)
	void draw2D() override;

private:
	//�X�e�[�W�ǂݍ���
	bool loadStage();
	// �e�N�X�`���ƃ}�E�X���W�̓����蔻��
	bool IsTex2Mouse(const SingleSprite& sprite);

private:
	//�}�E�X���W
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	
	//�w�i
	SingleSprite selectBack;
	// ����
	std::array<SingleSprite, 2> number;
	// ��
	SingleSprite arrowL;
	// ��
	SingleSprite arrowR;

	// �X�e�[�W�̐�
	const int stageCount = 12;
	// �I�𒆂̃X�e�[�W
	int nowStageNumber = 1;
	
	//�X�e�[�W�A�C�R���𓮂����Ă��邩�ǂ���
	bool isMoveStageIcon = false;
	
	//���̓t���O
	bool isPushStart = false;
	//�X�e�[�W�ǂݍ��݃t���O
	bool isLoadStage = false;
};