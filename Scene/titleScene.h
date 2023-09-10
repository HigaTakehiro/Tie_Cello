#pragma once
#include"primitiveScene.h"
#include <array>

class titleScene
	:public primitiveScene
{
public:
	//コンストラクタ
	titleScene();
	~titleScene();

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
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	//背景
	SingleSprite titleBack;
	std::array<SingleSprite, 8 * 2> titleChars;

	//タイトル
	
	//タイトルアニメーション
	bool isTitleAnimation = false;

	//タイトル演出フラグ
	bool isTitleStage = false;
	
	//入力フラグ
	bool isPushStart = false;
};