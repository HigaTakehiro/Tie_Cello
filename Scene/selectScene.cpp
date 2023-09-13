#include "selectScene.h"
#include "../Othello/Othello.h"

namespace
{
const int winW = 1280, winH = 720; //ウィンドウサイズ
}

selectScene::selectScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::select;
}

selectScene::~selectScene()
{

}

void selectScene::loadResources()
{
	//背景
	selectBack.size = { 1280,720 };
	selectBack.generateSprite("Tiethello_background_tentative.png");
	//数字
	for (int i = 0; i < number.size(); i++)
	{
		number[i].size = { 192.0f, 192.0f };
		number[i].position = { winW / 2.0f + ((i % 2) - 1) * number[i].size.x, winH / 2.0f - number[i].size.y / 2.0f, 0 };
		number[i].texSize = { 192.0f, 192.0f };
		number[i].generateSprite("Number_pattern01.png");
	}
	//←
	arrowL.size = { 128.0f, 128.0f };
	arrowL.position = { arrowR.size.x, winH / 2.0f, 0.0f };
	arrowL.rotation = 180.0f;
	arrowL.anchorpoint = { 0.5f, 0.5f };
	arrowL.generateSprite("Arrow.png");
	arrowL.spriteUpdata();
	//→
	arrowR.size = arrowL.size;
	arrowR.position = { winW - arrowR.size.x, winH / 2.0f, 0.0f };
	arrowR.anchorpoint = { 0.5f, 0.5f };
	arrowR.generateSprite("Arrow.png");
	arrowR.spriteUpdata();
}

void selectScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void selectScene::setParameter()
{
	isNextScene = false;
}

void selectScene::updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y, 0.0f };

	bool isLeft = IsTex2Mouse(arrowL) && nowStageNumber > 1;
	bool isRight = IsTex2Mouse(arrowR) && nowStageNumber < stageCount;
	bool isLoad = IsTex2Mouse(number[0]) || IsTex2Mouse(number[1]);

	if (input->Mouse_LeftTriger())
	{
		// ステージ切り替え
		if (isLeft)
		{
			nowStageNumber--;
		}
		if (isRight)
		{
			nowStageNumber++;
		}
		// ステージ読み込み
		if (isLoad)
		{
			isLoadStage = loadStage();
		}
	}
#ifdef _DEBUG
	// ステージ切り替え
	if (input->Triger(DIK_LEFT) || input->Triger(DIK_A))
	{
		if (nowStageNumber > 1)
		{
			nowStageNumber--;
		}
	}
	if (input->Triger(DIK_RIGHT) || input->Triger(DIK_D))
	{
		if (nowStageNumber < stageCount)
		{
			nowStageNumber++;
		}
	}
	// ステージ読み込み
	if (input->Triger(DIK_SPACE))
	{
		isLoadStage = loadStage();
	}
#endif // _DEBUG

	//次のシーンへの移行条件
	if (isLoadStage)
	{
		isNextScene = true;
	}
}

void selectScene::drawBack()
{
	selectBack.drawSprite(directx->cmdList.Get());
}

void selectScene::draw3D()
{
}

void selectScene::draw2D()
{
	if (nowStageNumber > 1)
	{
		arrowL.drawSprite(directx->cmdList.Get());
	}
	if (nowStageNumber < stageCount)
	{
		arrowR.drawSprite(directx->cmdList.Get());
	}

	char numberStr[10];
	sprintf_s(numberStr, "%02d", nowStageNumber);
	for (int i = 0; i < 2; i++)
	{
		int num = numberStr[i] - '0'; //描画する数字
		int texX = num % 5;
		int texY = num / 5;

		number[i].texLeftTop = { texX * number[i].texSize.x, texY * number[i].texSize.y };
		number[i].spriteUpdata(true);
		number[i].drawSprite(directx->cmdList.Get());
	}
}

bool selectScene::loadStage()
{
	Othello::SetLoadStageNumber(nowStageNumber);
	return true;
}

bool selectScene::IsTex2Mouse(const SingleSprite& sprite)
{
	float left = (0.0f - sprite.anchorpoint.x) * sprite.baceSize.x + sprite.position.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.baceSize.x + sprite.position.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.baceSize.y + sprite.position.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.baceSize.y + sprite.position.y;

	if (sprite.isFlipX == true)
	{
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY == true)
	{
		top = -top;
		bottom = -bottom;
	}

	if (left <= input->mousePoint.x && input->mousePoint.x <= right &&
		top <= input->mousePoint.y && input->mousePoint.y <= bottom)
	{
		return true;
	}

	return false;
}
