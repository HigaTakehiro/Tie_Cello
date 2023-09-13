#include "Othello.h"
#include <array>

// ファイルの読み込み
#include <fstream>
#include "../File/LoadCSV.h"

dxinput* Othello::input = nullptr;
int Othello::loadStageNumber = 0;

Othello::Othello() :
	cell{},
	width(8),
	height(8),
	startColor(false),
	blockDrawOffsetX(0),
	blockDrawOffsetZ(0),
	drawOffset(0, 0, 0),
	drawScale(1.0f, 1.0f, 1.0f)
{
	cellList.clear();
	blockList.clear();
}

Othello::~Othello()
{
	cellList.clear();
	blockList.clear();
}

void Othello::Init(const XMFLOAT3& offset, const XMFLOAT3& scale)
{
	cell.reserve(static_cast<size_t>(width * height));
	nowPlayingCell = std::make_unique<Cell>();

	isSkip = false;
	isTie = false;
	isFinish = false;
	drawOffset = offset;
	drawScale = scale;
}

void Othello::updata(const XMFLOAT3& mousepos)
{
	playerInput();

	isNowPlayerPointBlock(mousepos);

	for (std::unique_ptr<Block>& newblock : blockList)
	{
		if (newblock->getIndex() == nowPlayerPointBlockIndex)
		{
			Cell::playerBlockPosUpdata(newblock->getBlockPosition());
		}

		newblock->updata();
	}

	for (std::unique_ptr<Cell>& newcell : cellList)
	{
		newcell->updata();
	}

	if (nowPlayingCell)
	{
		if (nowColor == ColorFlag::WHITE)
		{
			if (nowPlayingCell->getColor() == cellType::black)
			{
				nowPlayingCell->changeColor();
			}
		}
		else if (nowColor == ColorFlag::BLACK)
		{
			if (nowPlayingCell->getColor() == cellType::white)
			{
				nowPlayingCell->changeColor();
			}
		}
		nowPlayingCell->updata();
	}

	blackCellCount = 0;
	whiteCellCount = 0;

	for (size_t i = 0; i < GetSize(); i++)
	{
		if (cell[i].colorFlag == ColorFlag::BLACK ||
			(cell[i].colorFlag == ColorFlag::BIG_B && cell[i].GetBigCell() == BigCell::LT))
		{
			blackCellCount++;
		}
		else if (cell[i].colorFlag == ColorFlag::WHITE ||
				 (cell[i].colorFlag == ColorFlag::BIG_W && cell[i].GetBigCell() == BigCell::LT))
		{
			whiteCellCount++;
		}
	}
}

void Othello::Draw()
{
	for (std::unique_ptr<Block>& newblock : blockList)
	{
		newblock->draw3D();
	}

	for (std::unique_ptr<Cell>& newcell : cellList)
	{
		newcell->draw3D();
	}

	if (nowPlayingCell && nowPlayerPointBlockIndex != -1)
	{
		nowPlayingCell->draw3D();
	}
}

void Othello::Reset()
{
	for (size_t i = 0; i < cell.size(); i++)
	{
		cell[i].Reset();
	}
}

int Othello::Put(ColorFlag color)
{
	if (nowPlayerPointBlockIndex == -1)
	{
		return 0;
	}

	if (!input->Mouse_LeftTriger())
	{
		return 0;
	}

	//置く場所のインデックスをセット
	int index = nowPlayerPointBlockIndex;

	//置く場所に何かしら置いてあったら
	if (cell[index].colorFlag != ColorFlag::EMPTY)
	{
		return 0;
	}

	//ひっくり返った枚数
	int count = 0;

	//相手の色
	ColorFlag other = ColorFlag::EMPTY;

	if (color == ColorFlag::BLACK)
	{
		other = ColorFlag::WHITE;
	}
	else if (color == ColorFlag::WHITE)
	{
		other = ColorFlag::BLACK;
	}

	//置くマスを中心とした３行
	for (int i = -1; i <= 1; i++)
	{
		//置くマスを中心とした３列
		for (int j = -1; j <= 1; j++)
		{
			//中心(置くマス)だったらとばす
			if (i == 0 && j == 0)
			{
				continue;
			}
			//場外だったらとばす
			if (nowPlayerPointBlockX + j < 0 || nowPlayerPointBlockZ + i < 0 ||
				nowPlayerPointBlockX + j >= width || nowPlayerPointBlockZ + i >= height)
			{
				continue;
			}

			//3*3マスの中の1マスのインデックスをセット
			index = (nowPlayerPointBlockZ + i) * width + (nowPlayerPointBlockX + j);

			//1マス周囲が同じ色か空だったらとばす
			if (cell[index].colorFlag != other)
			{
				continue;
			}

			//マップの最大幅
			const int size = 8;

			//2マス先以降を判定
			for (int s = 2; s < size; s++)
			{
				//場外だったらとばす
				if (nowPlayerPointBlockX + (j * s) < 0 || nowPlayerPointBlockZ + (i * s) < 0 ||
					nowPlayerPointBlockX + (j * s) >= width || nowPlayerPointBlockZ + (i * s) >= height)
				{
					break;
				}

				//2マス先以降のインデックスをセット
				index += i * width + j;

				//インデックスがマップの配列サイズ内だったら
				if (index >= 0 && index < cell.size())
				{
					//2マス先以降に石がないなら抜ける
					if (cell[index].colorFlag != ColorFlag::BLACK && cell[index].colorFlag != ColorFlag::WHITE)
					{
						break;
					}

					//隣が違う色で、2マス先以降に同じ色があった場合ひっくり返す
					if (cell[index].colorFlag == color)
					{
						//石を置く
						index = nowPlayerPointBlockZ * width + nowPlayerPointBlockX;
						cell[index].colorFlag = color;

						for (std::unique_ptr<Block>& newblock : blockList)
						{
							if (newblock->getIndex() == nowPlayerPointBlockIndex)
							{
								//石を置く
								if (cell[index].colorFlag == WHITE)
								{
									nowPlayingCell->init(
										{
											newblock->getBlockPosition().x,
											cellPosY,
											newblock->getBlockPosition().z
										},
										drawScale,
										cellType::white, true);
									nowPlayingCell->setIndex(index);
									cellList.push_back(std::move(nowPlayingCell));

									nowPlayingCell = std::make_unique<Cell>();
									nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::black, false);
								}
								else if (cell[index].colorFlag == BLACK)
								{
									nowPlayingCell->init(
										{
											newblock->getBlockPosition().x,
											cellPosY,
											newblock->getBlockPosition().z
										},
										drawScale,
										cellType::black, true);
									nowPlayingCell->setIndex(index);
									cellList.push_back(std::move(nowPlayingCell));

									nowPlayingCell = std::make_unique<Cell>();
									nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::white, false);
								}
							}
						}

						//ひっくり返した枚数
						count += s;

						//間のマスをひっくり返す
						for (int n = 1; n < s; n++)
						{
							//ひっくり返すマスのインデックスをセット
							index += i * width + j;

							//ひっくり返す(色を変える)
							for (std::unique_ptr<Cell>& newcell : cellList)
							{
								if (newcell->getIndex() == index)
								{
									newcell->setReverce();
								}
							}
							cell[index].colorFlag = color;
						}

						//ひっくり返したら別の方向を探索するため抜ける
						break;
					}
				}
			}
		}
	}

	return count;
}

void Othello::isNowPlayerPointBlock(XMFLOAT3 mousepos)
{
	nowPlayerPointBlockIndex = -1;

	for (std::unique_ptr<Block>& newblock : blockList)
	{
		if (newblock->isThisPlayerPoint({ mousepos.x,mousepos.y }))
		{
			nowPlayerPointBlockIndex = newblock->getIndex();

			Cell::playerBlockPosUpdata(newblock->getBlockPosition());

			nowPlayerPointBlockX = nowPlayerPointBlockIndex % width;
			nowPlayerPointBlockZ = nowPlayerPointBlockIndex / width;
		}
	}
}

bool Othello::IsSkip(ColorFlag color)
{
	//結果
	bool result = true;

	//とりあえず全マスを中心とした3*3を全て判定
	// i は一次元配列のインデックス
	for (int i = 0; i < width * height; i++)
	{
		//置けるなら抜ける
		if (result == false)
		{
			break;
		}

		//ブロックに何か置いてあったらとばす
		if (cell[i].colorFlag != ColorFlag::EMPTY)
		{
			continue;
		}

		//置く色に対して相手のタイプ
		ColorFlag other = ColorFlag::EMPTY;

		//引数が白か黒だったら反転したものをセット
		if (color == ColorFlag::BLACK)
		{
			other = ColorFlag::WHITE;
		}
		else if (color == ColorFlag::WHITE)
		{
			other = ColorFlag::BLACK;
		}

		//現在の一次元配列インデックス
		int index = i;

		//横座標：インデックスをマップの横幅で割った余り
		int x = index % width;

		//縦座標：インデックスをマップの横幅で割り小数点以下切り捨て
		int y = index / width;

		//置くマスを中心とした３行
		for (int dirY = -1; dirY <= 1; dirY++)
		{
			//置くマスを中心とした３列
			for (int dirX = -1; dirX <= 1; dirX++)
			{
				//中心(置くマス)だったらとばす
				if (dirY == 0 && dirX == 0)
				{
					continue;
				}

				//場外だったらとばす
				if (x + dirX < 0 || y + dirY < 0 || x + dirX >= width || y + dirY >= height)
				{
					continue;
				}

				//3*3マスの中の1マスのインデックスをセット
				index = (y + dirY) * width + (x + dirX);

				//1マス周囲が同じ色か空だったらとばす
				if (cell[index].colorFlag != other)
				{
					continue;
				}

				//マップの最大幅
				const int size = (width > height) ? width : height;

				//2マス先以降を判定
				for (int s = 2; s < size; s++)
				{
					//場外だったらとばす
					if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
					{
						break;
					}

					//2マス先以降のインデックスをセット
					index += dirY * width + dirX;

					//インデックスがマップの配列サイズ内だったら
					if (index >= 0 && index < cell.size())
					{
						//2マス先以降に石がないなら抜ける
						if (cell[index].colorFlag != ColorFlag::BLACK && cell[index].colorFlag != ColorFlag::WHITE)
						{
							break;
						}

						//隣が違う色で、2マス先以降に同じ色があった場合置けるので抜ける
						if (cell[index].colorFlag == color)
						{
							result = false;
							break;
						}
					}
				}
			}
		}
	}

	return result;
}

void Othello::playerInput()
{
	if (nowPlayerPointBlockIndex == -1)
	{
		return;
	}

	if (IsSkip(nowColor))
	{
		if (isSkip && !isAllCellMoved())
		{
			isFinish = true;
			isTie = blackCellCount == whiteCellCount;
		}
		else
		{
			isSkip = true;

			if (nowColor == ColorFlag::BLACK)
			{
				nowColor = ColorFlag::WHITE;
			}
			else if (nowColor == ColorFlag::WHITE)
			{
				nowColor = ColorFlag::BLACK;
			}
		}
	}
	else
	{
		isSkip = false;

		if (Put(nowColor) != 0)
		{
			if (nowColor == ColorFlag::BLACK)
			{
				nowColor = ColorFlag::WHITE;
			}
			else if (nowColor == ColorFlag::WHITE)
			{
				nowColor = ColorFlag::BLACK;
			}
		}
	}
}

int Othello::Load(const std::string& filePath)
{
	if (filePath.empty())
	{
		return -1;
	}

	cell.clear();
	blockList.clear();
	cellList.clear();

	std::ifstream ifs(filePath);
	std::string str;

	if (ifs.fail())
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return -1;
	}

	getline(ifs, str);
	int num[] = { 0, 0, 0 };
	int i = 0;
	for (auto s : str)
	{
		if (s == ',' || s == '\n')
		{
			i++;
			continue;
		}
		else if (s >= '0' && s <= '9')
		{
			num[i] *= 10;
			num[i] += s - '0';
		}
	}

	//ステージの横幅
	width = num[0];

	//ステージの縦幅
	height = num[1];

	//開始時の色
	startColor = num[2];

	int* cellArray = new int[(width * height)];
	File::LoadMapChip(ifs, cellArray, width * height);

	std::vector<std::array<int, 4>> bigCells;
	BigCell bigCellState = BigCell::NONE;

	//ここでデータを入力
	for (int i = 0; i < width * height; i++)
	{
		bigCellState = BigCell::NONE;

		if (cellArray[i] == ColorFlag::BIG_E || cellArray[i] == ColorFlag::BIG_B || cellArray[i] == ColorFlag::BIG_W)
		{
			bool isHit = false;

			for (auto& itr : bigCells)
			{
				for (int j = 0; j < 4; j++)
				{
					if (itr[j] == i)
					{
						isHit = true;
						bigCellState = static_cast<BigCell>(j);
						break;
					}
				}

				if (isHit)
				{
					break;
				}
			}

			if (isHit == false)
			{
				bigCells.push_back({ i, i + 1,i + width, i + width + 1 });
				bigCellState = BigCell::LT;
			}
		}

		cell.push_back({});
		cell.back().Init(static_cast<ColorFlag>(cellArray[i]), bigCellState);
	}

	blockDrawOffsetX = ((0.0f - ((float)width / 2)) * blockDistance * drawScale.x) + blockDistance * drawScale.x / 2;
	blockDrawOffsetZ = ((0.0f + ((float)height / 2)) * blockDistance * drawScale.z) - blockDistance * drawScale.z / 2;

	for (int i = 0; i < cell.size(); i++)
	{
		int x = i % width;
		int z = i / width;

		if (cell[i].colorFlag == HOLE || cell[i].colorFlag == NONE)
		{
			continue;
		}

		//ブロックを置く
		blockType type = blockType((x + z) % 2);

		std::unique_ptr<Block> newblock = std::make_unique<Block>();
		XMFLOAT3 pos =
		{
			blockDrawOffsetX + ((float)x * blockDistance) * drawScale.x,
			-30.0f,
			blockDrawOffsetZ - ((float)z * blockDistance) * drawScale.z
		};
		pos.x += drawOffset.x;
		pos.y += drawOffset.y;
		pos.z += drawOffset.z;
		newblock->init(type, pos, drawScale, i);

		blockList.push_back(std::move(newblock));

		//石を置く
		if (cell[i].colorFlag == WHITE ||
			(cell[i].colorFlag == ColorFlag::BIG_W && cell[i].GetBigCell() == BigCell::LT))
		{
			//新しい石
			std::unique_ptr<Cell> newcell = std::make_unique<Cell>();
			newcell->init(
				{
					blockList.back()->getBlockPosition().x,
					cellPosY,
					blockList.back()->getBlockPosition().z
				},
				drawScale,
				cellType::white, true);
			newcell->setIndex(i);
			cellList.push_back(std::move(newcell));
		}
		else if (cell[i].colorFlag == BLACK ||
				 (cell[i].colorFlag == ColorFlag::BIG_B && cell[i].GetBigCell() == BigCell::LT))
		{
			//新しい石
			std::unique_ptr<Cell> newcell = std::make_unique<Cell>();
			newcell->init(
				{
					blockList.back()->getBlockPosition().x,
					cellPosY,
					blockList.back()->getBlockPosition().z
				},
				drawScale,
				cellType::black, true);
			newcell->setIndex(i);
			cellList.push_back(std::move(newcell));
		}
	}

	Cell::playerBlockPosUpdata(blockList[0]->getBlockPosition());

	if (GetStartColor() == BLACK)
	{
		nowPlayingCell->init(blockList[0]->getBlockPosition(), drawScale, cellType::black, false);
	}
	else
	{
		nowPlayingCell->init(blockList[0]->getBlockPosition(), drawScale, cellType::white, false);
	}

	delete[] cellArray;
	ifs.close();
	return 0;
}

ColorFlag Othello::GetStartColor() const
{
	if (startColor)
	{
		return ColorFlag::WHITE;
	}
	else
	{
		return ColorFlag::BLACK;
	}
}
