#pragma once
#include"primitiveScene.h"
#include <array>

class selectScene
	:public primitiveScene
{
public:
	selectScene();
	~selectScene();

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
	//ステージ読み込み
	bool loadStage();
	// テクスチャとマウス座標の当たり判定
	bool IsTex2Mouse(const SingleSprite& sprite);

private:
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	
	//背景
	SingleSprite selectBack;
	// 数字
	std::array<SingleSprite, 2> number;
	// ←
	SingleSprite arrowL;
	// →
	SingleSprite arrowR;

	// ステージの数
	const int stageCount = 12;
	// 選択中のステージ
	int nowStageNumber = 1;
	
	//ステージアイコンを動かしているかどうか
	bool isMoveStageIcon = false;
	
	//入力フラグ
	bool isPushStart = false;
	//ステージ読み込みフラグ
	bool isLoadStage = false;
};