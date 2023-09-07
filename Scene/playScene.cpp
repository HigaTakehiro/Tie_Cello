#include "playScene.h"

playScene::playScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::play;
}

playScene::~playScene()
{

}

void playScene::loadResources()
{

}

void playScene::initialize()
{
	loadResources();

	setParameter();
}

void playScene::setParameter()
{
	isNextScene = false;
}

void playScene::updata()
{
	//ライト更新
	light->Update();

	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void playScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void playScene::draw3D()
{
}

void playScene::draw2D()
{
}

void playScene::tutorial()
{
	
}