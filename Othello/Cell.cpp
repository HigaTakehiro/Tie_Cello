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

	cellObject->setRotMatrix(0.0f, 0.0f, rotEasing.easing());

	if (rotEasing.getIsActive())
	{
		if (myType == cellType::white)
		{
			myType = cellType::black;
		}
		else if (myType == cellType::black)
		{
			myType = cellType::white;
		}
		isReverse = false;
	}
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
	cellObject = std::make_unique<object3dFBX>();
	cellObject->initialize();
	cellObject->SetModel(cellModel.get());

	myType = type;

	if (type == cellType::black)
	{
		cellObject->setRotMatrix(180.0f, 0.0f, 0.0f);
	}

	cellObject->SetScale({ 1,1,1 });

	isPut = put;
}

void Cell::updata()
{
	if (!isPut)
	{
		cellObject->SetPosition(
			{
				playerPointBlockPos.x,
				playerPointBlockPos.y + 10,
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
	rotEasing.set(easingType::easeOut, easingPattern::Quadratic, 40,
		cellObject->getRotation().z, cellObject->getRotation().z + 180);

	isReverse = true;
}

void Cell::draw3D()
{
	cellObject->Draw(dx->cmdList.Get());
}
