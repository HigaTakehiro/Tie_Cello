#pragma once
#include "primitiveScene.h"

class playScene
	:public primitiveScene
{
public:
	//�R���X�g���N�^
	playScene();
	~playScene();

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
	//�`���[�g���A��
	void tutorial();

	void pauseMenu();

private:
	//���o�p�J����
	std::unique_ptr<Camera> stagingCamera;

	int white = 0;
	int black = 0;

	bool isFinished = false;
	bool isClear = false;

	//----------------------�|�[�Y���j���[----------------------
	bool isPause = false;
	easingManager pauseScleenEase;

	std::unique_ptr<SingleSprite> pauseScleen;
	std::unique_ptr<SingleSprite> pauseText;
	std::unique_ptr<SingleSprite> restartText;
	std::unique_ptr<SingleSprite> selecText;
	std::unique_ptr<SingleSprite> backText;
	std::unique_ptr<SingleSprite> arrow;

	std::string loadStageFileName;
};