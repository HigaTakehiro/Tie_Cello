#pragma once
#include "primitiveScene.h"

class playScene
	:public primitiveScene
{
public:
	//コンストラクタ
	playScene();
	~playScene();

	//リソースの読み込み
	void loadResources() override;

	//初期化
	void initialize() override;

	//パラメータのリセット
	void setParameter() override;

	//更新
	void updata() override;

	//背景描画
	void drawBack() override;

	//描画(3D)
	void draw3D() override;

	//描画(2D)
	void draw2D() override;

private:
	//チュートリアル
	void tutorial();

	void pauseMenu();

private:
	//演出用カメラ
	std::unique_ptr<Camera> stagingCamera;

	int white = 0;
	int black = 0;

	bool isFinished = false;
	bool isClear = false;

	//----------------------ポーズメニュー----------------------
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