#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"
#include"../staging/Easing.h"
#include"../Particle/Particle.h"
#include<random>
#include<time.h>
#include<memory>
#include<list>
#include"../staging/EvenlyStaging.h"
#include"../Othello/Othello.h"

enum class gameSceneType
{
	title = 0,
	select = 1,
	play = 2,
	result = 3
};

class primitiveScene
{
public:
	primitiveScene();
	~primitiveScene();

	static void setStaticData(directX* Directx, dxinput* Input, Audio* Audio);

	static void finalize();

	//���\�[�X�̓ǂݍ���
	virtual void loadResources() = 0;

	//������
	virtual void initialize() = 0;

	//�p�����[�^�̃��Z�b�g
	virtual void setParameter() = 0;

	//�X�V
	virtual void updata() = 0;

	//�w�i�`��
	virtual void drawBack() = 0;

	//�`��(3D)
	virtual void draw3D() = 0;

	//�`��(2D)
	virtual void draw2D() = 0;

	//���̃V�[���Ɉڂ邩�ǂ���
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cpp����Q�Ƃ���̂Ő��|�C���^
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//�V�[���̎��
	gameSceneType thisType = gameSceneType::title;

	//���̃V�[���Ɉڍs����t���O
	bool isNextScene = false;

	//-------�����̃V�[�����܂����Ŏg�����͂����ɏ���-------
	//�w�i
	static std::unique_ptr<SingleSprite> sample_back;
	//���C�g
	static Light* light;
	//�J����
	static Camera* camera;
	//�X�e�[�W�ԍ�
	static int stageNum;
	//�ő�X�e�[�W�ԍ�
	static const int maxStageNum = 3;
	//�X�e�[�W�A�C�R���̊���W
	static XMFLOAT3 stageIconOffset;
	//�`���[�g���A��
	static bool isTutorial;
	//�N���A���I�[�o�[��(true:�N���A/false:�I�[�o�[)
	static bool isClearOrOver;
	//�^�C�g�����Z���N�g��(true:�Z���N�g/false:�^�C�g��)
	static bool isSelectOrTitle;

	static bool isTitle;
	static bool isSelect;
	static bool isResult;

	//�I�Z���ƃX�R�A�w�i
	static std::unique_ptr<Othello> othello;
	static std::unique_ptr<EvenlyStaging> evenry;

	//��

	static SoundData titleBGM;
};