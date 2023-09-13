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

	isTitle = false;
	isSelect = false;
	isResult = false;
	isNextScene = false;
}

void playScene::updata()
{
	//ライト更新
	light->Update();

	EvenlyStaging::setWhiteBlackCount(othello->GetWhiteCount(), othello->GetBlackCount());

	EvenlyStaging::ratioSet(0.5f);

	if (othello->GetNowColor() == ColorFlag::WHITE)
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

		isResult = true;
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

void playScene::pauseMenu()
{
	if (!isPause)
	{
		return;
	}
}
