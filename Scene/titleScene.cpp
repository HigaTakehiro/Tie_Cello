#include "titleScene.h"

titleScene::titleScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::title;
}

titleScene::~titleScene()
{

}

void titleScene::loadResources()
{
	//背景
	titleBack.size = { 1280,720 };
	titleBack.generateSprite("black_color.png");

	//タイトル
	
}

void titleScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void titleScene::setParameter()
{
	//タイトルアニメーション準備
	isPushStart = false;
	isNextScene = false;
}

void titleScene::updata()
{
	//タイトル画面更新処理
	
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	titleBack.spriteUpdata();

	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void titleScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void titleScene::draw3D()
{
}

void titleScene::draw2D()
{
	titleBack.drawSprite(directx->cmdList.Get());
}
