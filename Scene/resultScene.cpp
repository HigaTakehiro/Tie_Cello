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
	clearText = std::make_unique<SingleSprite>();
	clearText->generateSprite("Clear.png");
	clearText->anchorpoint = { 0.5f,0.5f };
	clearText->size = { 498,196.5f };
	clearText->position = { 640,270,0 };

	titleText = std::make_unique<SingleSprite>();
	titleText->generateSprite("Title.png");
	titleText->anchorpoint = { 0.5f,0.5f };
	titleText->size = { 219,130 };
	titleText->baceSize = { 219,130 };
	titleText->choosingSize = { 240.9,143 };
	titleText->position = { 440,470,0 };

	selectText = std::make_unique<SingleSprite>();
	selectText->generateSprite("Select.png");
	selectText->anchorpoint = { 0.5f,0.5f };
	selectText->size = { 326,137 };
	selectText->baceSize = { 326,137 };
	selectText->choosingSize = { 358.6,150.7 };
	selectText->position = { 840,470,0 };

	restartText = std::make_unique<SingleSprite>();
	restartText->generateSprite("Restart.png");
	restartText->anchorpoint = { 0.5f,0.5f };
	restartText->size = { 480,127 };
	restartText->baceSize = { 326,127 };
	restartText->choosingSize = { 528,139.7 };
	restartText->position = { 340,470,0 };
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
	isPlay = false;
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

	selectText->spriteUpdata();
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
	if (isClearOrOver)
	{
		clearText->drawSprite(directx->cmdList.Get());
		titleText->drawSprite(directx->cmdList.Get());
		selectText->drawSprite(directx->cmdList.Get());
	}
	else
	{
		restartText->drawSprite(directx->cmdList.Get());
		selectText->drawSprite(directx->cmdList.Get());
	}
}

void resultScene::clearScene()
{
	//
	clearText->spriteUpdata();
	titleText->spriteUpdata();

	//次のシーンへの移行条件
	if (titleText->isSpriteMouseInput())
	{
		isSelectOrTitle = false;
		isNextScene = true;
	}
	else if (selectText->isSpriteMouseInput())
	{
		isSelectOrTitle = true;
		isNextScene = true;
	}
}

void resultScene::overScene()
{
	restartText->spriteUpdata();

	//次のシーンへの移行条件
	if (restartText->isSpriteMouseInput())
	{
		isPlay = true;
		isNextScene = true;
	}
	else if (selectText->isSpriteMouseInput())
	{
		isSelectOrTitle = true;
		isNextScene = true;
	}
}