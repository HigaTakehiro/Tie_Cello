#include "EvenlyStaging.h"
#include"../Base/WindowGenerate.h"
#include<random>
PostEffect* EvenlyStaging::post = nullptr;
directX* EvenlyStaging::dx = nullptr;
float EvenlyStaging::clearRatio = 0.5f;
float EvenlyStaging::nowRatio = 0;
int EvenlyStaging::whiteCount = 0;
int EvenlyStaging::blackCount = 0;
int EvenlyStaging::oldWhiteCount = 0;
int EvenlyStaging::oldBlackCount = 0;

void EvenlyStaging::setstaticdata(PostEffect* p, directX* d)
{
	post = p;
	dx = d;
	SingleParticle::loadTexInMap("enemyWaveBar.png");
}

void EvenlyStaging::init()
{
	clearLine.anchorpoint = { 0.5f,0 };
	clearLine.size = { 5,windowHight };
	clearLine.generateSprite("boostGauge.png");

	nowLine.anchorpoint = { 0.5f,0 };
	nowLine.size = { 5,windowHight };
	nowLine.generateSprite("bossHPGauge.png");

	whiteBack.anchorpoint = { 0,0 };
	whiteBack.color = { 0.8f,0.8f ,0.83f ,1 };
	whiteBack.generateSprite("enemyWaveBar.png");

	blackBack.anchorpoint = { 0,0 };
	blackBack.generateSprite("black_color.png");
}

void EvenlyStaging::updata()
{
	changeBackSprite();

	setParticle();

	clearLine.spriteUpdata();
	nowLine.spriteUpdata();
	whiteBack.spriteUpdata();
	blackBack.spriteUpdata();

	countWhiteBlack();

	for (std::unique_ptr<SingleSprite>& newp : whiteCountNum)
	{
		newp->spriteUpdata(true);
	}

	for (std::unique_ptr<SingleSprite>& newp : blackCountNum)
	{
		newp->spriteUpdata(true);
	}

	particleList.remove_if([](std::unique_ptr<SingleParticle>& newset)
		{
			return newset->getIsActive() == false;
		});

	for (std::unique_ptr<SingleParticle>& newp : particleList)
	{
		newp->updata();
	}
}

void EvenlyStaging::changeBackSprite()
{
	//クリアラインのセット
	float clearLineX = windowWidth * clearRatio;
	clearLine.position.x = clearLineX;

	//現在のラインのセット
	float nowLineX = windowWidth * nowRatio;
	nowLine.position.x = nowLineX;

	//白側の背景のセット
	whiteBack.position = { 0,0,0 };
	whiteBack.size = { nowLineX,windowHight };

	//黒側の背景のセット
	blackBack.position = { nowLineX,0,0 };
	blackBack.size = { windowWidth - nowLineX,windowHight };
}

void EvenlyStaging::setParticle()
{
	std::unique_ptr<SingleParticle> newp = std::make_unique<SingleParticle>();

	std::random_device seed;
	std::mt19937 rnd(seed());
	std::uint32_t Result = rnd();

	int lange = 260;

	float nowratioline = nowRatio * lange - 130;

	if (isWhite)
	{
		float leftlimittonowratioline = fabsf(nowratioline - (-130));
		int startX = Result % (int)(leftlimittonowratioline);

		newp->generate();
		newp->color = { 0.5f,0.5f,0.5f,0.8f };
		newp->set(360, { -130 + (float)startX,-30,-70 },
			{ 0.0f,0.0f,0.4f },
			{ 0,0,0 }, 1.5f, 1.5f);
		particleList.push_back(std::move(newp));
	}
	else
	{
		float nowratiolinetorightlimit = fabsf(130 - nowratioline);
		int startX = Result % (int)(nowratiolinetorightlimit);

		newp->generate();
		newp->color = { 0.5f,0.5f,0.5f,0.8f };
		newp->set(360, { nowratioline + (float)startX,-30,-70 },
			{ 0.0f,0.0f,0.4f },
			{ 0,0,0 }, 1.5f, 1.5f);
		particleList.push_back(std::move(newp));
	}
}

void EvenlyStaging::countWhiteBlack()
{
	if (oldWhiteCount == whiteCount)
	{
		return;
	}

	whiteCountNum.clear();
	blackCountNum.clear();

	float whiteOffsetX = 252;
	float whiteOffsetY = 360;
	float blackOffsetX = 932;

	int wsp1L = (whiteCount % 10) % 5;
	int wsp1T = (whiteCount % 10) / 5;
	int bsp1L = (blackCount % 10) % 5;
	int bsp1T = (blackCount % 10) / 5;
	int wsp10L = (whiteCount / 10) % 5;
	int wsp10T = (whiteCount / 10) / 5;
	int bsp10L = (blackCount / 10) % 5;
	int bsp10T = (blackCount / 10) / 5;

	std::unique_ptr<SingleSprite> wsp1 = std::make_unique<SingleSprite>();
	wsp1->generateSprite("Number_pattern01.png");
	wsp1->anchorpoint = { 0.5f,0.5f };
	wsp1->texLeftTop = { (float)wsp1L * 192,(float)wsp1T * 192 };
	wsp1->texSize = { 192,192 };
	wsp1->size = { 96,96 };
	wsp1->position = { whiteOffsetX + 96,whiteOffsetY,0 };
	wsp1->spriteUpdata(true);

	std::unique_ptr<SingleSprite> bsp1 = std::make_unique<SingleSprite>();
	bsp1->generateSprite("Number_pattern02.png");
	bsp1->anchorpoint = { 0.5f,0.5f };
	bsp1->texLeftTop = { (float)bsp1L * 192,(float)bsp1T * 192 };
	bsp1->texSize = { 192,192 };
	bsp1->size = { 96,96 };
	bsp1->position = { blackOffsetX + 96,whiteOffsetY,0 };
	bsp1->spriteUpdata(true);

	std::unique_ptr<SingleSprite> wsp10 = std::make_unique<SingleSprite>();
	wsp10->generateSprite("Number_pattern01.png");
	wsp10->anchorpoint = { 0.5f,0.5f };
	wsp10->texLeftTop = { (float)wsp10L * 192,(float)wsp10T * 192 };
	wsp10->texSize = { 192,192 };
	wsp10->size = { 96,96 };
	wsp10->position = { whiteOffsetX,whiteOffsetY,0 };
	wsp10->spriteUpdata(true);

	std::unique_ptr<SingleSprite> bsp10 = std::make_unique<SingleSprite>();
	bsp10->generateSprite("Number_pattern02.png");
	bsp10->anchorpoint = { 0.5f,0.5f };
	bsp10->texLeftTop = { (float)bsp10L * 192,(float)bsp10T * 192 };
	bsp10->texSize = { 192,192 };
	bsp10->size = { 96,96 };
	bsp10->position = { blackOffsetX,whiteOffsetY,0 };
	bsp10->spriteUpdata(true);

	whiteCountNum.push_back(std::move(wsp1));
	blackCountNum.push_back(std::move(bsp1));
	whiteCountNum.push_back(std::move(wsp10));
	blackCountNum.push_back(std::move(bsp10));
}

void EvenlyStaging::reSetBuffer()
{
	post->depthClear(dx->cmdList.Get());
}

void EvenlyStaging::draw3D()
{
	for (std::unique_ptr<SingleParticle>& newp : particleList)
	{
		newp->drawSpecifyTex("enemyWaveBar.png");
	}

	post->depthClear(dx->cmdList.Get());
}

void EvenlyStaging::draw2D()
{
	whiteBack.drawSprite(dx->cmdList.Get());
	blackBack.drawSprite(dx->cmdList.Get());
	clearLine.drawSprite(dx->cmdList.Get());
	nowLine.drawSprite(dx->cmdList.Get());

	for (std::unique_ptr<SingleSprite>& newp : whiteCountNum)
	{
		newp->drawSprite(dx->cmdList.Get());
	}

	for (std::unique_ptr<SingleSprite>& newp : blackCountNum)
	{
		newp->drawSprite(dx->cmdList.Get());
	}
}
