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
	/// <summary>
	/// 静的データセット
	/// </summary>
	/// <param name="d">directxクラス</param>
	static void setStaticData(directX* d);

	/// <summary>
	/// 初期化
	/// </summary>
	void init(blockType type, XMFLOAT3 position);

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

private:
	static directX* dx;
	//モデルデータ
	static std::unique_ptr<Model> blockModel_1;
	static std::unique_ptr<Model> blockModel_2;

	//オブジェクト
	std::unique_ptr<object3dFBX> blockObject;
	//イージング
	easingManager startEasing;
	//初期位置
	XMFLOAT3 startPos;
	//終端位置
	XMFLOAT3 endPos;
};