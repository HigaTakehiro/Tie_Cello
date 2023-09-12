#pragma once
#include"../FbxLoder/Object3d_FBX.h"
#include"../staging/Easing.h"
#include"../Base/DirectX_Base.h"

enum class blockType
{
	light = 0,
	dark = 1
};

class Block
{
private:
	/// <summary>
	/// リソース読み込み
	/// </summary>
	static void loadResources();

public:
	Block();
	~Block();

	/// <summary>
	/// 静的データセット
	/// </summary>
	/// <param name="d">directxクラス</param>
	static void setStaticData(directX* d);

	/// <summary>
	/// 初期化
	/// </summary>
	void init(blockType type, XMFLOAT3 position, const XMFLOAT3& scale, int id);

	/// <summary>
	/// 更新
	/// </summary>
	void updata();

	/// <summary>
	/// 描画
	/// </summary>
	void draw3D();

	/// <summary>
	///	マウスが指しているブロックかどうか
	/// </summary>
	/// <param name="mousepos">マウス座標</param>
	/// <returns>判定結果</returns>
	bool isThisPlayerPoint(XMFLOAT2 mousepos);

	/// <summary>
	/// インデックスを取得
	/// </summary>
	/// <returns>インデックス</returns>
	int getIndex() { return index; }

	/// <summary>
	/// ブロックの座標を取得
	/// </summary>
	XMFLOAT3 getBlockPosition() { return blockObject->getPosition(); }

private:
	//dierctx
	static directX* dx;

	//モデルデータ
	static std::unique_ptr<Model> blockModel_1;
	static std::unique_ptr<Model> blockModel_2;

public:
	//オブジェクト
	object3dFBX* blockObject = nullptr;

	//イージング
	easingManager startEasing;

	//初期位置
	XMFLOAT3 startPos;

	//終端位置
	XMFLOAT3 endPos;

	//マップ全体で何番目のインデックスかどうか(初期値-1)
	int index = -1;
};