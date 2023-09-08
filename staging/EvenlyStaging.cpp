#include "EvenlyStaging.h"
#include"../Base/WindowGenerate.h"
PostEffect* EvenlyStaging::post = nullptr;
directX* EvenlyStaging::dx = nullptr;
float EvenlyStaging::clearRatio = 0;
float EvenlyStaging::nowRatio = 0;

void EvenlyStaging::setstaticdata(PostEffect* p, directX* d)
{
	post = p;
	dx = d;
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

void EvenlyStaging::updata(bool isWhite)
{
	changeBackSprite();

	clearLine.spriteUpdata();
	nowLine.spriteUpdata();
	whiteBack.spriteUpdata();
	blackBack.spriteUpdata();
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
}

void EvenlyStaging::reSetBuffer()
{
	post->depthClear(dx->cmdList.Get());
}

void EvenlyStaging::draw3D()
{
}

void EvenlyStaging::draw2D()
{
	whiteBack.drawSprite(dx->cmdList.Get());
	blackBack.drawSprite(dx->cmdList.Get());
	clearLine.drawSprite(dx->cmdList.Get());
	nowLine.drawSprite(dx->cmdList.Get());
}
