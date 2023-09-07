#include"resultScene.h"

resultScene::resultScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::result;
}

resultScene::~resultScene()
{

}

void resultScene::loadResources()
{
	
}

void resultScene::initialize()
{
	//ライト更新
	light->Update();

	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void resultScene::setParameter()
{
	isNextScene = false;
}

void resultScene::updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	if (isClearOrOver)
	{
		clearScene();
	}
	else
	{
		overScene();
	}

	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void resultScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void resultScene::draw3D()
{
	
}

void resultScene::draw2D()
{
	
}

void resultScene::clearScene()
{
	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void resultScene::overScene()
{
	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}