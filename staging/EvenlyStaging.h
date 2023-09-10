#pragma once
#include"../2D/Sprite.h"
#include"../Particle/particleManagerOnTime.h"
#include"../2D/PostEffect.h"

class EvenlyStaging
{
public:

	static void setstaticdata(PostEffect* p, directX* d);

	void init();

	/// <summary>
	/// 割合の設定
	/// </summary>
	/// <param name="now">現在の割合(0～1)</param>
	/// <param name="clear">クリア条件の割合(0～1)</param>
	static void ratioSet(float clear)
	{
		clearRatio = clear;

		int totalCell = whiteCount + blackCount;

		if (whiteCount == 0 && blackCount == 0)
		{
			nowRatio = 0.5f;
			return;
		}

		nowRatio = (float)(whiteCount / (float)totalCell);
	}

	static void setWhiteBlackCount(int wcount, int bcount)
	{
		whiteCount = wcount;
		blackCount = bcount;
	}

	void updata(bool iswhite);

	static void reSetBuffer();

	void draw3D();

	void draw2D();

private:

	void changeBackSprite();

	void setParticle();

	//静的メンバ変数
	static PostEffect* post;	//深度バッファクリア用
	static directX* dx;			//描画処理用
	static float nowRatio;		//現在の白黒の割合
	static float clearRatio;	//クリア条件の白黒の割合
	static int whiteCount;		//白の数
	static int blackCount;		//黒の数

	bool isWhite;				//白の手番かどうか
	SingleSprite clearLine;
	SingleSprite nowLine;
	SingleSprite whiteBack;
	SingleSprite blackBack;

	std::list<std::unique_ptr<SingleParticle>> particleList;
};