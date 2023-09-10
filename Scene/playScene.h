#pragma once
#include "primitiveScene.h"
#include"../staging/EvenlyStaging.h"
#include"../Othello/Othello.h"

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

private:
	//演出用カメラ
	std::unique_ptr<Camera> stagingCamera;

	std::unique_ptr<EvenlyStaging> evenry;

	std::unique_ptr<Othello> othello;

	int white = 0;
	int black = 0;
};