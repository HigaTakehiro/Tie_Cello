#include "titleScene.h"

titleScene::titleScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::title;
}

titleScene::~titleScene()
{

}

void titleScene::loadResources()
{
	//背景
	titleBack.size = { 1280,720 };
	titleBack.generateSprite("Tiethello_background_tentative.png");

	const size_t charCount = titleChars.size() / 2;
	const XMFLOAT3 offset = { 100.0f, 100.0f, 0 };
	for (int i = 0; i < titleChars.size(); i++)
	{
		titleChars[i].size = { 192.0f, 192.0f };
		titleChars[i].position = { (i / 2) * 128.0f, 0, 0 };
		titleChars[i].position = {
			titleChars[i].position.x + offset.x,
			titleChars[i].position.y + offset.y,
			titleChars[i].position.z + offset.z
		};

		titleChars[i].texSize = { 256.0f, 256.0f };
		//Tiethllo
		int texY = 0;
		switch (i / 2)
		{
		case 3: //t
			texY = 6;
			break;
		case 4: //h
			texY = 5;
			break;
		case 5: //l
		case 6:
			texY = 4;
			break;
		case 7: //o
			texY = 3;
			break;
		default: // T, i, e
			texY = i / 2;
			break;
		}
		titleChars[i].texLeftTop = { (i % 2) * titleChars[i].texSize.x, texY * titleChars[i].texSize.y };
		titleChars[i].generateSprite("TitleChars.png", false, false, false, true);
	}
	//タイトル

	//Move
	//Number_pattern01
	//Number_pattern02
}

void titleScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void titleScene::setParameter()
{
	//タイトルアニメーション準備
	isPushStart = false;
	isNextScene = false;
}

void titleScene::updata()
{
	//タイトル画面更新処理
	
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	titleBack.spriteUpdata();
	for (auto& i : titleChars)
	{
		i.spriteUpdata(true);
	}

	//次のシーンへの移行条件
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
}

void titleScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void titleScene::draw3D()
{
}

void titleScene::draw2D()
{
	titleBack.drawSprite(directx->cmdList.Get());
	for (auto& i : titleChars)
	{
		i.drawSprite(directx->cmdList.Get());
	}
}
