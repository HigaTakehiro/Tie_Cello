#pragma once
#include"primitiveScene.h"

class resultScene
	:public primitiveScene
{
public:
	//�R���X�g���N�^
	resultScene();
	~resultScene();

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
	//�N���A�V�[��
	void clearScene();

	//�I�[�o�[�V�[��
	void overScene();

private:
	//�}�E�X���W
	XMFLOAT3 MOUSE_POS = { 0,0,0 };

	bool isMoveSelectIcon = false;

	bool isPushTitle = false;

	std::unique_ptr<SingleSprite> clearText;
	std::unique_ptr<SingleSprite> titleText;
	std::unique_ptr<SingleSprite> selectText;

	std::unique_ptr<SingleSprite> restartText;
};