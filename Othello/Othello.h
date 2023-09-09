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
public:
	static const int circleSize = 90;

	static dxinput* input;

private: //メンバ変数
	std::vector<Color> cell;
	std::vector<Color> initCell;
	int width;  //幅
	int height; //高さ
	bool startColor; //開始時の色

	//石とブロックのリスト
	std::vector<std::unique_ptr<Cell>> cellList;
	std::vector<std::unique_ptr<Block>> blockList;

	//現在マウスで指している位置インデックス(場外＆穴の場合は-1)
	int nowPlayerPointBlockIndex = -1;

	//描画オフセット
	float drawOffsetX;
	float drawOffsetZ;

	//1ブロックの幅
	const float blockDistance = 10.0f;

public: //メンバ関数
	static void setInput(dxinput* in) { input = in; }

	Othello();
	~Othello() {}

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
	/// <param name="offsetX">全体のオフセット</param>
	/// <param name="offsetY">全体のオフセット</param>
	void Draw(int offsetX = 0, int offsetY = 0);

	// リセット
	void Reset();

	// 石を置く
	int Put(Color color);

	//どのブロックをマウスで指しているか判定
	void isNowPlayerPointBlock(XMFLOAT3 mousepos);

	
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
};