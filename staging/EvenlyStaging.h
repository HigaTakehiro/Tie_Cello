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
		oldWhiteCount = whiteCount;
		oldBlackCount = blackCount;

		whiteCount = wcount;
		blackCount = bcount;
	}

	void updata();

	void setIsWhite(bool iswhite)
	{
		oldIsWhite = isWhite;
		isWhite = iswhite;
	}

	static void reSetBuffer();

	void draw3D();

	void draw2D();

	bool getIsClear()
	{
		if (nowRatio == clearRatio)
		{
			isClear = true;
		}
		else
		{
			isClear = false;
		}

		return isClear;
	}

private:

	void changeBackSprite();

	void setParticle();

	void countWhiteBlack();

	//静的メンバ変数
	static PostEffect* post;	//深度バッファクリア用
	static directX* dx;			//描画処理用
	static float nowRatio;		//現在の白黒の割合
	static float clearRatio;	//クリア条件の白黒の割合

	//現在フレームのカウント
	static int whiteCount;		//白の数
	static int blackCount;		//黒の数

	static int oldWhiteCount;		//白の数
	static int oldBlackCount;		//黒の数

	bool isClear = false;
	bool isWhite = false;		//白の手番かどうか
	bool oldIsWhite = false;
	SingleSprite clearLine;
	SingleSprite nowLine;
	SingleSprite whiteBack;
	SingleSprite blackBack;

	std::list<std::unique_ptr<SingleSprite>> whiteCountNum;
	std::list<std::unique_ptr<SingleSprite>> blackCountNum;

	std::list<std::unique_ptr<SingleParticle>> particleList;

	int FontWid = 9;
	int FontHei = 18;
};