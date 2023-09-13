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
	pauseScleen->anchorpoint = { 0.5f,0.0f };
	pauseScleen->size = { 350,165 };
	pauseScleen->position = { -175,80,0 };

	pauseText = std::make_unique<SingleSprite>();
	pauseText->generateSprite("Pause.png");
	pauseText->anchorpoint = { 0.5f,0.5f };
	pauseText->size = { 163,62.5 };
	pauseText->baceSize = pauseText->size;
	pauseText->choosingSize = { pauseText->size.x * 1.1f,pauseText->size.y * 1.1f };
	pauseText->position = { 90,35,0 };

	restartText = std::make_unique<SingleSprite>();
	restartText->generateSprite("Restart.png");
	restartText->anchorpoint = { 0.5f,0.0f };
	restartText->size = { 240,63.5 };
	restartText->baceSize = restartText->size;
	restartText->choosingSize = { restartText->size.x * 1.1f,restartText->size.y * 1.1f };
	restartText->position = { 0,90,0 };

	selecText = std::make_unique<SingleSprite>();
	selecText->generateSprite("Select.png");
	selecText->anchorpoint = { 0.5f,0.0f };
	selecText->size = { 163,68.5 };
	selecText->baceSize = selecText->size;
	selecText->choosingSize = { selecText->size.x * 1.1f,selecText->size.y * 1.1f };
	selecText->position = { 0,160,0 };

	backText = std::make_unique<SingleSprite>();
	backText->generateSprite("Back.png");
	backText->anchorpoint = { 0.5f,0.5f };
	backText->size = { 136.5,62 };
	backText->baceSize = backText->size;
	backText->choosingSize = { backText->size.x * 1.1f,backText->size.y * 1.1f };
	backText->position = { 72,280,0 };

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
	loadStageFileName = "Resources/StageData/stage" + num + ".csv";

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

	if (pauseScleenEase.getIsActive())
	{
		pauseScleen->position.x = pauseScleenEase.easing();
	}

	if (pauseText->isSpriteMouseInput() && !isPause)
	{
		isPause = true;
		pauseScleenEase.set(easingType::easeOut, easingPattern::Quadratic, 30, -175, 175);
	}

	pauseMenu();

	pauseScleen->spriteUpdata();
	pauseText->spriteUpdata();
	backText->spriteUpdata();

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
	pauseText->drawSprite(directx->cmdList.Get());
	pauseScleen->drawSprite(directx->cmdList.Get());

	if (isPause)
	{
		restartText->drawSprite(directx->cmdList.Get());
		selecText->drawSprite(directx->cmdList.Get());
		backText->drawSprite(directx->cmdList.Get());
	}
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

	restartText->position.x = pauseScleen->position.x;
	selecText->position.x = pauseScleen->position.x;

	restartText->spriteUpdata();
	selecText->spriteUpdata();

	if (pauseScleenEase.getIsActive())
	{
		return;
	}

	if (restartText->isSpriteMouseInput())
	{
		othello->Load(loadStageFileName);
		pauseScleenEase.set(easingType::easeOut, easingPattern::Quadratic, 30, 175, -175);
		isPause = false;
	}
	else if (selecText->isSpriteMouseInput())
	{
		isSelect = true;
		isNextScene = true;
	}
	else if (backText->isSpriteMouseInput())
	{
		pauseScleenEase.set(easingType::easeOut, easingPattern::Quadratic, 30, 175, -175);
		isPause = false;
	}
}
