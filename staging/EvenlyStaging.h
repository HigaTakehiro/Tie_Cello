#pragma once
#include"../2D/Sprite.h"
#include"../Particle/particleManagerOnTime.h"
#include"../2D/PostEffect.h"

class EvenlyStaging
{
public:

	static void setstaticdata(PostEffect* p, directX* d);

	void init();

	static void ratioSet(float now, float clear)
	{
		nowRatio = now;
		clearRatio = clear;
	}

	void updata();

	static void reSetBuffer();

	void draw3D();

	void draw2D();

private:
	//ê√ìIÉÅÉìÉoïœêî
	static PostEffect* post;
	static directX* dx;
	static float nowRatio;
	static float clearRatio;

	SingleSprite clearLine;
	SingleSprite nowLine;
	SingleSprite whiteBack;
	SingleSprite blackBack;


};