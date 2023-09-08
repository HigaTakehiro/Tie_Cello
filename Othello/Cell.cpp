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
	if (!isReverse)
	{
		return;
	}

	cellObject->setRotMatrix(rotEasing.easing(), 0.0f, 0.0f);
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

void Cell::init(cellType type)
{
	cellObject = std::make_unique<object3dFBX>();
	cellObject->initialize();
	cellObject->SetModel(cellModel.get());

	if (type == cellType::black)
	{
		cellObject->setRotMatrix(180.0f, 0.0f, 0.0f);
	}

	cellObject->SetScale({ 1,1,1 });
}

void Cell::updata()
{
	reverseCell();
}

void Cell::draw3D()
{
	cellObject->Draw(dx->cmdList.Get());
}
