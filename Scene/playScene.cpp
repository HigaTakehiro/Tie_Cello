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
	//othello->~Othello();
}

void playScene::loadResources()
{
	evenry->init();
}

void playScene::initialize()
{
	loadResources();

	setParameter();
}

void playScene::setParameter()
{
	Othello::setInput(input);
	othello->Init();
	std::string num = std::to_string(Othello::GetLoadStageNumber());
	othello->Load(("Resources/StageData/stage" + num + ".csv"));
	isNextScene = false;
}

void playScene::updata()
{
	//ライト更新
	light->Update();

	EvenlyStaging::setWhiteBlackCount(othello->getWhiteCount(), othello->getBlackCount());

	EvenlyStaging::ratioSet(0.3f);

	if (othello->GetNowColor() == Color::WHITE)
	{
		evenry->setIsWhite(true);
	}
	else
	{
		evenry->setIsWhite(false);
	}
	evenry->updata();

	othello->updata(input->mousePosition);

	//次のシーンへの移行条件
	if (othello->GetFinishFlag())
	{
		if (evenry->getIsClear())
		{
			isClearOrOver = true;
		}
		else
		{
			isClearOrOver = false;
		}

		//次のシーンへの演出準備ゾーン

		isNextScene = true;
	}
}

void playScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
	evenry->draw2D();
}

void playScene::draw3D()
{
	evenry->draw3D();
	othello->Draw();
}

void playScene::draw2D()
{
}

void playScene::tutorial()
{

}