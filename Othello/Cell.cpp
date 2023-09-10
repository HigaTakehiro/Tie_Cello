#include "Cell.h"

directX* Cell::dx = nullptr;
std::unique_ptr<Model> Cell::cellModel = std::make_unique<Model>();
XMFLOAT3 Cell::playerPointBlockPos = { 0,0,0 };
bool Cell::isMove = false;

void Cell::loadResources()
{
	cellModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("Stone"));
}

void Cell::reverseCell()
{
	if (!isReverse || !isPut)
	{
		return;
	}

	cellObject->setRotMatrix(rotEasing.easing(), 0.0f, 0.0f);

	if (!rotEasing.getIsActive())
	{
		rotEasing.reSet();
		if (myType == cellType::white)
		{
			myType = cellType::black;
			cellObject->setRotMatrix(135.0f, 0.0f, 0.0f);
		}
		else if (myType == cellType::black)
		{
			myType = cellType::white;
			cellObject->setRotMatrix(0.0f, 0.0f, 0.0f);
		}
		isReverse = false;
	}
}

Cell::Cell()
{

}

Cell::~Cell()
{
	delete(cellObject);
}

void Cell::setStaticData(directX* d)
{
	dx = d;
	loadResources();
}

void Cell::playerBlockPosUpdata(XMFLOAT3 blockPos)
{
	playerPointBlockPos = blockPos;
}

void Cell::init(XMFLOAT3 pos, cellType type, bool put)
{
	cellObject = new object3dFBX;
	cellObject->initialize();
	cellObject->SetModel(cellModel.get());

	myType = type;

	if (type == cellType::black)
	{
		cellObject->setRotMatrix(135.0f, 0.0f, 0.0f);
	}

	cellObject->SetPosition(pos);
	cellObject->SetScale({ 0.015f,0.015f,0.015f });

	isPut = put;
}

void Cell::updata()
{
	if (!isPut)
	{
		cellObject->SetPosition(
			{
				playerPointBlockPos.x,
				playerPointBlockPos.y + 5,
				playerPointBlockPos.z
			}
		);
	}

	reverseCell();

	cellObject->updata();
}

void Cell::setReverce()
{
	if (!isPut && isReverse)
	{
		return;
	}

	if (myType == cellType::white)
	{
		rotEasing.set(easingType::easeOut, easingPattern::Quadratic, 30,
			0.0f, 180.0f);
	}
	else if (myType == cellType::black)
	{
		rotEasing.set(easingType::easeOut, easingPattern::Quadratic, 30,
			180.0f, 0.0f);
	}

	isReverse = true;
}

void Cell::draw3D()
{
	cellObject->Draw(dx->cmdList.Get());
}
