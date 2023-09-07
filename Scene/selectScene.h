#pragma once
#include"primitiveScene.h"

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

private:
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };
	
	//背景
	SingleSprite selectBack;
	//ステージアイコン
	
	//セレクトアイコン
	
	//プレイスタートボタン
	
	
	//ステージアイコンのイージング
	
	//ステージアイコンを動かしているかどうか
	bool isMoveStageIcon = false;
	
	//入力フラグ
	bool isPushStart = false;
	//ステージ読み込みフラグ
	bool isLoadStage = false;
};