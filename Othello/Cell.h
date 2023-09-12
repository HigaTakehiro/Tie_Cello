#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../staging/Easing.h"
#include"../Base/DirectX_Base.h"

enum class cellType
{
	white = 0,
	black = 1
};

class Cell
{
private:
	/// <summary>
	/// リソース読み込み
	/// </summary>
	static void loadResources();

	/// <summary>
	/// ひっくり返す
	/// </summary>
	void reverseCell();

public:
	Cell();
	~Cell();

	/// <summary>
	/// 静的データセット
	/// </summary>
	/// <param name="d">directxクラス</param>
	static void setStaticData(directX* d);

	/// <summary>
	/// プレイヤーがさしているブロックの位置を更新
	/// </summary>
	/// <param name="playerPos">ブロックの座標</param>
	static void playerBlockPosUpdata(XMFLOAT3 blockPos);

	/// <summary>
	/// 初期化
	/// </summary>
	void init(XMFLOAT3 pos, cellType type, bool put);

	/// <summary>
	/// 更新
	/// </summary>
	void updata();

	/// <summary>
	/// ひっくり返す準備
	/// </summary>
	void setReverce();

	/// <summary>
	/// 色変更
	/// </summary>
	void changeColor();

	/// <summary>
	/// 描画
	/// </summary>
	void draw3D();

	bool getIsPut() { return isPut; }

	bool getIsReverse() { return isReverse; }

	void setIndex(int id) { index = id; }

	int getIndex() { return index; }

	cellType getColor() { return myType; }

private:
	static directX* dx;
	//モデルデータ
	static std::unique_ptr<Model> cellModel;
	//プレイヤーが指しているブロックの位置
	static XMFLOAT3 playerPointBlockPos;
	//石が移動できるかどうか
	static bool isMove;

	//オブジェクト
	object3dFBX* cellObject = nullptr;
	//イージング
	easingManager rotEasing;
	//ウラ・オモテ
	cellType myType = cellType::white;
	//置いてある石かどうか
	bool isPut = false;
	//ひっくり返している途中かどうか
	bool isReverse = false;
	//マップ全体で何番目のインデックスかどうか(初期値-1)
	int index = -1;
};