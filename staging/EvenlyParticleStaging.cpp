#include "EvenlyParticleStaging.h"
PostEffect* EvenlyParticleStaging::post = nullptr;

void EvenlyParticleStaging::setstaticdata(PostEffect* p)
{
	post = p;
}

void EvenlyParticleStaging::init()
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

void EvenlyParticleStaging::updata()
{

}

void EvenlyParticleStaging::clearBufferStaging()
{
}

void EvenlyParticleStaging::draw()
{
}
