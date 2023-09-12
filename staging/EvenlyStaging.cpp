#include "EvenlyStaging.h"
#include"../Base/WindowGenerate.h"
#include<random>
PostEffect* EvenlyStaging::post = nullptr;
directX* EvenlyStaging::dx = nullptr;
float EvenlyStaging::clearRatio = 0;
float EvenlyStaging::nowRatio = 0;
int EvenlyStaging::whiteCount = 0;
int EvenlyStaging::blackCount = 0;

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

	particleList.remove_if([](std::unique_ptr<SingleParticle>& newset)
		{
			return newset->getIsActive() == false;
		});

	for (std::unique_ptr<SingleParticle>& newp : particleList)
	{
		newp->updata();
	}

	if (nowRatio == clearRatio)
	{
		isClear = true;
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
		newp->set(300, { -130 + (float)startX,-30,-70 },
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
}
