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

	evenry->updata();
	othello->updata(input->mousePosition);

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
	evenry->draw2D();
}

void resultScene::draw3D()
{
	evenry->draw3D();
	othello->Draw();
}

void resultScene::draw2D()
{
	
}

void resultScene::clearScene()
{
	//

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