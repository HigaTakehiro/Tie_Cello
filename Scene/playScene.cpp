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

	pauseScleen = std::make_unique<SingleSprite>();
	pauseScleen->generateSprite("PauseMenu_Back.png");
	pauseScleen->anchorpoint = { 0.0f,0.0f };
	pauseScleen->size = { 480,512 };
	pauseScleen->position = { 640,270,0 };

	pauseText = std::make_unique<SingleSprite>();
	pauseText->generateSprite("Pause.png");
	pauseText->anchorpoint = { 0.0f,0.0f };
	pauseText->size = { 480,512 };
	pauseText->position = { 640,270,0 };

	restartText = std::make_unique<SingleSprite>();
	restartText->generateSprite("Restart.png");
	restartText->anchorpoint = { 0.0f,0.0f };
	restartText->size = { 480,512 };
	restartText->position = { 640,270,0 };

	selecText = std::make_unique<SingleSprite>();
	selecText->generateSprite("Select.png");
	selecText->anchorpoint = { 0.0f,0.0f };
	selecText->size = { 480,512 };
	selecText->position = { 640,270,0 };

	backText = std::make_unique<SingleSprite>();
	backText->generateSprite("Back.png");
	backText->anchorpoint = { 0.0f,0.0f };
	backText->size = { 480,512 };
	backText->position = { 640,270,0 };

	arrow = std::make_unique<SingleSprite>();
	arrow->generateSprite("Arrow.png");
	arrow->anchorpoint = { 0.0f,0.0f };
	arrow->size = { 480,512 };
	arrow->position = { 640,270,0 };
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
