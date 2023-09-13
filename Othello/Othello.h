#pragma once
#include <vector>
#include <string>
#include"../FbxLoder/Object3d_FBX.h"
#include "ColorFlag.h"
#include"Cell.h"
#include"Block.h"
#include"../Input/dxInput.h"
#include"../Audio/Audio.h"

class Othello
{
public: //定数
	static const int circleSize = 90;

private: //静的メンバ変数
	static dxinput* input;
	static Audio* audio;
	static int loadStageNumber;

	//石を置いた音
	static SoundData putCellSE;

public: //静的メンバ関数
	static void setInput(dxinput* in) { input = in; }
	static void SetLoadStageNumber(int num) { loadStageNumber = num; }

	static int GetLoadStageNumber() { return loadStageNumber; }
	static void setAudioAndLoadSE(Audio* ad);

private: //メンバ変数

	//マップデータ
	std::vector<CellData> cell;

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
	XMFLOAT3 drawOffset;
	// 描画スケール
	XMFLOAT3 drawScale;

	//1ブロックの幅
	const float blockDistance = 15.0f;

	//スキップフラグ
	bool isSkip = false;
	//終了フラグ
	bool isFinish = false;
	//引き分けフラグ
	bool isTie = false;

	//現在選択しているインデックスの横幅・縦幅
	int nowPlayerPointBlockX = 0;
	int nowPlayerPointBlockZ = 0;

	//現在の色
	ColorFlag nowColor = ColorFlag::BLACK;

	int blackCellCount = 0;
	int whiteCellCount = 0;

	float cellPosY = -27.0f;

public: //メンバ関数
	Othello();
	~Othello();

	// 初期化
	void Init(const XMFLOAT3& offset = {}, const XMFLOAT3& scale = {1.0f, 1.0f, 1.0f});

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mousepos">マウス座標</param>
	void updata(const XMFLOAT3& mousepos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// リセット
	void Reset();

	// 石を置く
	int Put(ColorFlag color);

	//どのブロックをマウスで指しているか判定
	void isNowPlayerPointBlock(XMFLOAT3 mousepos);

	//プレイヤー入力
	void playerInput();

	// デカマスの置き換え
	int BigChange(int index, ColorFlag color);
	// ステージ読み込み
	int Load(const std::string& filePath);

	// スキップ判定
	//置ける所がなければスキップ
	bool IsSkip(ColorFlag color);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	size_t GetSize() const { return cell.size(); }
	CellData GetCell(const size_t& index) const { return cell[index]; }
	ColorFlag GetStartColor() const;

	ColorFlag getNowColor() { return nowColor; }
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

	ColorFlag GetNowColor() { return nowColor; }
	int GetBlackCount() { return blackCellCount; }
	int GetWhiteCount() { return whiteCellCount; }
	bool GetSkipFlag() const { return isSkip; }
	bool GetTieFlag() const { return isTie; }
	bool GetFinishFlag() const { return isFinish; }

	// デカマスの左上座標
	int GetBigIndex(int index) const;
};