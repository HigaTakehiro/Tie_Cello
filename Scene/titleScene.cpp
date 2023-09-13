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
	const XMFLOAT3 offset = { 32.0f, 100.0f, 0 };
	for (int i = 0; i < titleChars.size(); i++)
	{
		titleChars[i].size = { 192.0f, 192.0f };
		titleChars[i].position = { (i / 2) * 128.0f, -16.0f, 0 };
		titleChars[i].position = {
			titleChars[i].position.x + offset.x,
			titleChars[i].position.y + offset.y,
			titleChars[i].position.z + offset.z
		};

		titleChars[i].texSize = { 256.0f, 256.0f };
		//Tiethello
		int texY = 0;
		switch (i / 2)
		{
		// T
		case 0:
			texY = 0;
			break;
		// i
		case 1:
			texY = 1;
			break;
		// e
		case 2:
		case 5:
			texY = 2;
			break;
		// o
		case 8:
			texY = 3;
			break;
		// l
		case 6:
		case 7:
			texY = 4;
			break;
		// h
		case 4:
			texY = 5;
			break;
		// t
		case 3:
			texY = 6;
			break;
		default:
			break;
		}
		titleChars[i].texLeftTop = { (i % 2) * titleChars[i].texSize.x, texY * titleChars[i].texSize.y };
		titleChars[i].generateSprite("TitleChars.png", false, false, false, true);
		titleChars[i].spriteUpdata(true);
	}
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
	// 盤面の初期化
	Cell::setStaticData(directx);
	Block::setStaticData(directx);
	Othello::setInput(input);
	othello = std::make_unique<Othello>();
	othello->Init({ 0.0f, 0.0f, -15.0f }, { 1.5f, 1.0f, 1.5f });
	othello->Load(("Resources/StageData/tutorial.csv"));

	audio->playSoundWave_Val(titleBGM, true, 0.2);

	//タイトルアニメーション準備
	isPushStart = false;
	isNextScene = false;
}

void titleScene::updata()
{
	//ライト更新
	light->Update();

	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y, 0.0f };

	othello->updata(input->mousePosition);

	static bool oldCellMoved = false;
	static bool nowCellMoved = false;
	oldCellMoved = nowCellMoved;
	nowCellMoved = othello->isAllCellMoved();

	//次のシーンへの移行条件
	if (othello->GetSkipFlag() &&
		(nowCellMoved == false && oldCellMoved == false))
	{
		isNextScene = true;
	}
#ifdef _DEBUG
	if (input->Triger(DIK_SPACE))
	{
		isNextScene = true;
	}
#endif // _DEBUG
}

void titleScene::drawBack()
{
	titleBack.drawSprite(directx->cmdList.Get());
	for (auto& i : titleChars)
	{
		i.drawSprite(directx->cmdList.Get());
	}
}

void titleScene::draw3D()
{
	othello->Draw();
}

void titleScene::draw2D()
{
}
