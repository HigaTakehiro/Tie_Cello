#include "EvenlyStaging.h"
PostEffect* EvenlyStaging::post = nullptr;
directX* EvenlyStaging::dx = nullptr;
float EvenlyStaging::clearRatio = 0;
float EvenlyStaging::nowRatio = 0;

void EvenlyStaging::setstaticdata(PostEffect* p, directX* d)
{
	post = p;
}

void EvenlyStaging::init()
{
	clearLine.anchorpoint = { 0.5f,0 };
	clearLine.generateSprite("boostGauge.png");
	nowLine.anchorpoint = { 0.5f,0 };
	nowLine.generateSprite("bossHPGauge.png");
	whiteBack.anchorpoint = { 0,0 };
	whiteBack.generateSprite("enemyWaveBar.png");
	blackBack.anchorpoint = { 0,0 };
	blackBack.generateSprite("black_color.png");
}

void EvenlyStaging::updata()
{
	clearLine.spriteUpdata();
	nowLine.spriteUpdata();
	whiteBack.spriteUpdata();
	blackBack.spriteUpdata();
}

void EvenlyStaging::reSetBuffer()
{
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
