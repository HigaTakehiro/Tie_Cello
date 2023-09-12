#include "Block.h"

directX* Block::dx = nullptr;
std::unique_ptr<Model> Block::blockModel_1 = std::make_unique<Model>();
std::unique_ptr<Model> Block::blockModel_2 = std::make_unique<Model>();

void Block::loadResources()
{
	blockModel_1.reset(FbxLoader::GetInstance()->LoadmodelFromFile("Board_1"));
	blockModel_2.reset(FbxLoader::GetInstance()->LoadmodelFromFile("Board_2"));
}

Block::Block()
{
}

Block::~Block()
{
	delete(blockObject);
}

void Block::setStaticData(directX* d)
{
	dx = d;
	loadResources();
}

void Block::init(blockType type, XMFLOAT3 position, const XMFLOAT3& scale, int id)
{
	if (blockObject == nullptr)
	{
		//オブジェクト生成
		blockObject = new object3dFBX;
	}

	blockObject->initialize();

	if (type == blockType::light)
	{
		blockObject->SetModel(blockModel_1.get());
	}
	else
	{
		blockObject->SetModel(blockModel_2.get());
	}

	startPos = position;
	blockObject->SetPosition(position);
	const XMFLOAT3 baseScale = { 0.024f,0.024f,0.024f };
	blockObject->SetScale({ baseScale.x * scale.x, baseScale.y * scale.y, baseScale.z * scale.z });

	index = id;
}

void Block::updata()
{
	blockObject->updata();
}

void Block::draw3D()
{
	blockObject->Draw(dx->cmdList.Get());
}

bool Block::isThisPlayerPoint(XMFLOAT2 mousepos)
{
	XMFLOAT2 thispos = blockObject->worldToScleen();
	thispos =
	{
		thispos.x,
		thispos.y - 10
	};

	float dis = sqrtf(powf(thispos.x - mousepos.x, 2) + powf(thispos.y - mousepos.y, 2));

	if (dis <= 30.0f)
	{
		return true;
	}

	return false;
}
