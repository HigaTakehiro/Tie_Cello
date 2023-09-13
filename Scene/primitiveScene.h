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

	//リソースの読み込み
	virtual void loadResources() = 0;

	//初期化
	virtual void initialize() = 0;

	//パラメータのリセット
	virtual void setParameter() = 0;

	//更新
	virtual void updata() = 0;

	//背景描画
	virtual void drawBack() = 0;

	//描画(3D)
	virtual void draw3D() = 0;

	//描画(2D)
	virtual void draw2D() = 0;

	//次のシーンに移るかどうか
	bool getIsNextScene() { return isNextScene; }

public:
	//main.cppから参照するので生ポインタ
	static directX* directx;
	static dxinput* input;
	static Audio* audio;

	//シーンの種類
	gameSceneType thisType = gameSceneType::title;

	//次のシーンに移行するフラグ
	bool isNextScene = false;

	//-------複数のシーンをまたいで使う物はここに書く-------
	//背景
	static std::unique_ptr<SingleSprite> sample_back;
	//ライト
	static Light* light;
	//カメラ
	static Camera* camera;
	//ステージ番号
	static int stageNum;
	//最大ステージ番号
	static const int maxStageNum = 3;
	//ステージアイコンの基準座標
	static XMFLOAT3 stageIconOffset;
	//チュートリアル
	static bool isTutorial;
	//クリアかオーバーか(true:クリア/false:オーバー)
	static bool isClearOrOver;
	//タイトルかセレクトか(true:セレクト/false:タイトル)
	static bool isSelectOrTitle;

	static bool isTitle;
	static bool isSelect;
	static bool isResult;

	//オセロとスコア背景
	static std::unique_ptr<Othello> othello;
	static std::unique_ptr<EvenlyStaging> evenry;

	//音

	static SoundData titleBGM;
};