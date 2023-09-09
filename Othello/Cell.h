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
	void init(XMFLOAT3 pos, cellType type);

	/// <summary>
	/// 更新
	/// </summary>
	void updata();

	/// <summary>
	/// ひっくり返す準備
	/// </summary>
	void setReverce();

	/// <summary>
	/// 描画
	/// </summary>
	void draw3D();

	bool getIsPut() { return isPut; }

private:
	static directX* dx;
	//モデルデータ
	static std::unique_ptr<Model> cellModel;
	//プレイヤーが指しているブロックの位置
	static XMFLOAT3 playerPointBlockPos;
	//石が移動できるかどうか
	static bool isMove;

	//オブジェクト
	std::unique_ptr<object3dFBX> cellObject;
	//イージング
	easingManager rotEasing;
	//ウラ・オモテ
	cellType myType = cellType::white;
	//置いてある石かどうか
	bool isPut = false;
	//ひっくり返している途中かどうか
	bool isReverse = false;
};