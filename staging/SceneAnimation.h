#pragma once

class SceneAnimation
{
private: //メンバ変数
	bool isAnimation; //シーン遷移のアニメーションが行われているかどうか
	bool isChange;    //シーン遷移するかどうか

	int radius; //半径

public: //メンバ関数
	SceneAnimation();
	~SceneAnimation();

};
