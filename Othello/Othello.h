#pragma once
#include <vector>
#include <string>
#include"../FbxLoder/Object3d_FBX.h"
#include"Cell.h"
#include"Block.h"
#include"../Input/dxInput.h"

enum Color
{
	EMPTY,
	BLACK,
	WHITE,
	HOLE,
	NONE = -1
};

class Othello
{
public: //定数
	static const int circleSize = 90;

private: //静的メンバ変数
	static dxinput* input;
	static int loadStageNumber;

public: //静的メンバ関数
	static void setInput(dxinput* in) { input = in; }
	static void SetLoadStageNumber(int num) { loadStageNumber = num; }

	static int GetLoadStageNumber() { return loadStageNumber; }

private: //メンバ変数

	//マップデータ
	std::vector<Color> cell;
	std::vector<Color> initCell;

	int width;  //幅
	int height; //高さ

	bool startColor; //開始時の色

	//石とブロックのリスト
	std::vector<std::unique_ptr<Cell>> cellList;
	std::vector<std::unique_ptr<Block>> blockList;

	//今の石
	std::unique_ptr<Cell> nowPlayingCell;

	//現在マウスで指している位置インデックス(場外＆穴の場合は-1)
	int nowPlayerPointBlockIndex = -1;

	//描画オフセット
	float blockDrawOffsetX;
	float blockDrawOffsetZ;

	//1ブロックの幅
	const float blockDistance = 15.0f;

	//終了フラグ
	bool isFinish = false;

	//スキップフラグ
	bool isSkip = false;

	//現在選択しているインデックスの横幅・縦幅
	int nowPlayerPointBlockX = 0;
	int nowPlayerPointBlockZ = 0;

	//現在の色
	Color nowColor = Color::BLACK;

	int blackCellCount = 0;
	int whiteCellCount = 0;

	float cellPosY = -27.0f;

public: //メンバ関数
	Othello();
	~Othello();

	// 初期化
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// /// <param name="mousepos">マウス座標</param>
	void updata(XMFLOAT3 mousepos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// リセット
	void Reset();

	// 石を置く
	int Put(Color color);

	//どのブロックをマウスで指しているか判定
	void isNowPlayerPointBlock(XMFLOAT3 mousepos);

	//プレイヤー入力
	void playerInput();

	// ステージ読み込み
	int Load(const std::string& filePath);

	// スキップ判定
	//置ける所がなければスキップ
	bool IsSkip(Color color);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	size_t GetSize() const { return cell.size(); }
	Color GetCell(const size_t& index) const;
	Color GetStartColor() const;
	Color getNowColor() { return nowColor; }
	int getBlackCount() { return blackCellCount; }
	int getWhiteCount() { return whiteCellCount; }
	bool isAllCellMoved()
	{
		bool ismove = false;
		for (std::unique_ptr<Cell>& newcell : cellList)
		{
			if (newcell->getIsReverse())
			{
				ismove = true;
			}
		}
		return ismove;
	}
	bool getIsFinish()
	{
		return isFinish;
	}
};