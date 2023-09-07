#pragma once
#include"primitiveScene.h"

class resultScene
	:public primitiveScene
{
public:
	//コンストラクタ
	resultScene();
	~resultScene();

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
	//クリアシーン
	void clearScene();

	//オーバーシーン
	void overScene();

private:
	//マウス座標
	XMFLOAT3 MOUSE_POS = { 0,0,0 };

	bool isMoveSelectIcon = false;

	bool isPushTitle = false;
};