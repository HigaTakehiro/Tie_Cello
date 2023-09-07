#include "selectScene.h"

selectScene::selectScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::select;
}

selectScene::~selectScene()
{

}

void selectScene::loadResources()
{
	//背景
	selectBack.size = { 1280,720 };
	selectBack.generateSprite("black_color.png");
}

void selectScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void selectScene::setParameter()
{
	isNextScene = false;
}

void selectScene::updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void selectScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void selectScene::draw3D()
{
}

void selectScene::draw2D()
{
	
}

bool selectScene::loadStage()
{
	//ステージ読み込み処理
	//CSV読み込みとかはそっちからもってきて

	return true;
}