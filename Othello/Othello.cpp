#include "Othello.h"
#include <array>

// ファイルの読み込み
#include <fstream>
#include "../File/LoadCSV.h"

dxinput* Othello::input = nullptr;
Audio* Othello::audio = nullptr;
int Othello::loadStageNumber = 0;
SoundData Othello::putCellSE = SoundData();

void Othello::setAudioAndLoadSE(Audio* ad)
{
	audio = ad;
	putCellSE = audio->loadSoundWave("Resources/Sound's/PutStone.wav", putCellSE);
}

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
	bool isBigC = false; //デカマスを通るかどうか(自分の色)
	bool isBigO = false; //デカマスを通るかどうか(相手の色)

	//置く場所に何かしら置いてあったら
	if (cell[index].colorFlag != ColorFlag::EMPTY && cell[index].colorFlag != ColorFlag::BIG_E)
	{
		return 0;
	}

	ColorFlag skipColor[] = {
		ColorFlag::EMPTY,
		ColorFlag::HOLE,
		ColorFlag::BIG_E
	};

	//ひっくり返った枚数
	int count = 0;

	ColorFlag bigColor = ColorFlag::EMPTY; //自分の色(デカマス)
	ColorFlag other = ColorFlag::EMPTY; //相手の色
	ColorFlag bigOther = ColorFlag::EMPTY; //相手の色(デカマス)
	// colorがデカマスだったらノーマルに設定し直す
	if (color == ColorFlag::BIG_B)
	{
		color = ColorFlag::BLACK;
	}
	else if (color == ColorFlag::BIG_W)
	{
		color = ColorFlag::WHITE;
	}
	// それぞれに対応した物を設定する
	if (color == ColorFlag::BLACK)
	{
		bigColor = ColorFlag::BIG_B;
		other = ColorFlag::WHITE;
		bigOther = ColorFlag::BIG_W;
	}
	else if (color == ColorFlag::WHITE)
	{
		bigColor = ColorFlag::BIG_W;
		other = ColorFlag::BLACK;
		bigOther = ColorFlag::BIG_B;
	}

	if (cell[index].colorFlag == ColorFlag::EMPTY)
	{
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
				if (nowPlayerPointBlockX + dirX < 0 || nowPlayerPointBlockZ + dirY < 0 ||
					nowPlayerPointBlockX + dirX >= width || nowPlayerPointBlockZ + dirY >= height)
				{
					continue;
				}

				//3*3マスの中の1マスのインデックスをセット
				index = (nowPlayerPointBlockZ + dirY) * width + (nowPlayerPointBlockX + dirX);
				isBigC = false;
				isBigO = false;

				//1マス周囲が同じ色か空だったらとばす
				if (cell[index].colorFlag == bigOther)
				{
					isBigO = true;
				}
				else if (cell[index].colorFlag != other)
				{
					continue;
				}

				//マップの最大幅
				const int size = (width > height) ? width : height;

				//2マス先以降を判定
				for (int s = 2; s < size; s++)
				{
					//場外だったらとばす
					if (nowPlayerPointBlockX + (dirX * s) < 0 || nowPlayerPointBlockZ + (dirY * s) < 0 ||
						nowPlayerPointBlockX + (dirX * s) >= width || nowPlayerPointBlockZ + (dirY * s) >= height)
					{
						break;
					}

					//2マス先以降のインデックスをセット
					index += dirY * width + dirX;

					//インデックスが配列サイズ外だったらとばす
					if (index < 0 || index >= cell.size())
					{
						break;
					}

					bool flag = false;
					for (auto j : skipColor)
					{
						if (cell[index].colorFlag == j)
						{
							flag = true;
							break;
						}
					}
					if (flag)
					{
						break;
					}

					//隣が違う色で、2マス先以降に同じ色があった場合ひっくり返す
					if (cell[index].colorFlag == color)
					{
						if (isBigC || isBigO)
						{
							break;
						}

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

									audio->stopSoundWave(putCellSE);
									audio->playSoundWave_Val(putCellSE, false, 0.35);
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

									audio->stopSoundWave(putCellSE);
									audio->playSoundWave_Val(putCellSE, false, 0.35);
								}
							}
						}

						//間のマスをひっくり返す
						for (int n = 1; n < s; n++)
						{
							//ひっくり返すマスのインデックスをセット
							index += dirY * width + dirX;
							count++;

							if (cell[index].GetBigCell() == NONE)
							{
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
							else
							{
								int indexLT = GetBigIndex(index);

								index += dirY * width + dirX;
								n++;

								//ひっくり返す(色を変える)
								for (std::unique_ptr<Cell>& newcell : cellList)
								{
									if (newcell->getIndex() == indexLT)
									{
										newcell->setReverce();
									}
								}

								BigChange(indexLT, bigColor);
							}
						}

						//ひっくり返したら別の方向を探索するため抜ける
						break;
					}
					else if (cell[index].colorFlag == bigColor)
					{
						if (isBigO)
						{
							break;
						}

						if (isBigC)
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

							//間のマスをひっくり返す
							for (int n = 1; n < s; n++)
							{
								//ひっくり返すマスのインデックスをセット
								index += dirY * width + dirX;
								count++;

								if (cell[index].GetBigCell() == NONE)
								{
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
								else
								{
									int indexLT = GetBigIndex(index);

									index += dirY * width + dirX;
									n++;

									//ひっくり返す(色を変える)
									for (std::unique_ptr<Cell>& newcell : cellList)
									{
										if (newcell->getIndex() == indexLT)
										{
											newcell->setReverce();
										}
									}

									BigChange(indexLT, bigColor);
								}
							}

							//ひっくり返したら別の方向を探索するため抜ける
							break;
						}

						isBigC = !isBigC;
					}
					else if (cell[index].colorFlag == other)
					{
						if (isBigO)
						{
							break;
						}
					}
					else if (cell[index].colorFlag == bigOther)
					{
						isBigO = !isBigO;
					}
				}
			}
		}
	}
	else if (cell[index].colorFlag == ColorFlag::BIG_E)
	{
		int indexLT = index;
		if (cell[index].GetBigCell() % 2 == 1)
		{
			indexLT -= 1;
		}
		if (cell[index].GetBigCell() / 2 == 1)
		{
			indexLT -= width;
		}

		for (int i = 0; i < 4; i++)
		{
			index = indexLT;
			if (i % 2 == 1)
			{
				index += 1;
			}
			if (i / 2 == 1)
			{
				index += width;
			}
			int bigCellX = index % width;
			int bigCellY = index / width;

			for (int dir = 0; dir < 3; dir++)
			{
				int dirX = 0, dirY = 0;

				switch (i)
				{
				case BigCell::LT:
					if (dir % 2 == 0)
					{
						dirX = -1;
					}
					if (dir / 2 == 0)
					{
						dirY = -1;
					}
					break;
				case BigCell::RT:
					if (dir > 0)
					{
						dirX = +1;
					}
					if (dir / 2 == 0)
					{
						dirY = -1;
					}
					break;
				case BigCell::LB:
					if (dir / 2 == 0)
					{
						dirX = -1;
					}
					if (dir > 0)
					{
						dirY = +1;
					}
					break;
				case BigCell::RB:
					if (dir % 2 == 0)
					{
						dirX = +1;
					}
					if (dir > 0)
					{
						dirY = +1;
					}
					break;
				default:
					break;
				}

				if (bigCellX + dirX < 0 || bigCellY + dirY < 0 ||
					bigCellX + dirX >= width || bigCellY + dirY >= height)
				{
					continue;
				}

				index = (bigCellY + dirY) * width + (bigCellX + dirX);
				isBigC = false;
				isBigO = false;

				if (cell[index].colorFlag == bigOther)
				{
					isBigO = true;
				}
				else if (cell[index].colorFlag != other)
				{
					continue;
				}

				const int size = (width > height) ? width : height;
				//2マス先以降を判定
				for (int s = 2; s < size; s++)
				{
					//場外だったらとばす
					if (bigCellX + (dirX * s) < 0 || bigCellY + (dirY * s) < 0 ||
						bigCellX + (dirX * s) >= width || bigCellY + (dirY * s) >= height)
					{
						break;
					}

					//2マス先以降のインデックスをセット
					index += dirY * width + dirX;

					//インデックスが配列サイズ外だったらとばす
					if (index < 0 || index >= cell.size())
					{
						break;
					}

					bool flag = false;
					for (auto j : skipColor)
					{
						if (cell[index].colorFlag == j)
						{
							flag = true;
							break;
						}
					}
					if (flag)
					{
						break;
					}

					XMFLOAT3 bigScale =
					{
						drawScale.x * 2.0f,
						drawScale.y * 1.0f,
						drawScale.z * 2.0f
					};

					//隣が違う色で、2マス先以降に同じ色があった場合ひっくり返す
					if (cell[index].colorFlag == color)
					{
						if (isBigC || isBigO)
						{
							break;
						}

						//石を置く
						index = bigCellY * width + bigCellX;
						BigChange(index, color);

						for (std::unique_ptr<Block>& newblock : blockList)
						{
							if (newblock->getIndex() == nowPlayerPointBlockIndex)
							{
								//石を置く
								if (cell[index].colorFlag == BIG_W)
								{
									nowPlayingCell->init(
										{
											newblock->getBlockPosition().x,
											cellPosY,
											newblock->getBlockPosition().z
										},
										bigScale,
										cellType::white, true);
									nowPlayingCell->setIndex(index);
									cellList.push_back(std::move(nowPlayingCell));

									nowPlayingCell = std::make_unique<Cell>();
									nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::black, false);
								}
								else if (cell[index].colorFlag == BIG_B)
								{
									nowPlayingCell->init(
										{
											newblock->getBlockPosition().x,
											cellPosY,
											newblock->getBlockPosition().z
										},
										bigScale,
										cellType::black, true);
									nowPlayingCell->setIndex(index);
									cellList.push_back(std::move(nowPlayingCell));

									nowPlayingCell = std::make_unique<Cell>();
									nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::white, false);
								}
							}
						}

						//間のマスをひっくり返す
						for (int n = 1; n < s; n++)
						{
							//ひっくり返すマスのインデックスをセット
							index += dirY * width + dirX;
							count++;

							if (cell[index].GetBigCell() == NONE)
							{
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
							else
							{
								int indexLT = GetBigIndex(index);

								index += dirY * width + dirX;
								n++;

								//ひっくり返す(色を変える)
								for (std::unique_ptr<Cell>& newcell : cellList)
								{
									if (newcell->getIndex() == indexLT)
									{
										newcell->setReverce();
									}
								}

								BigChange(indexLT, bigColor);
							}
						}

						//ひっくり返したら別の方向を探索するため抜ける
						break;
					}
					else if (cell[index].colorFlag == bigColor)
					{
						if (isBigO)
						{
							break;
						}

						if (isBigC)
						{
							//石を置く
							index = bigCellY * width + bigCellX;
							BigChange(index, color);

							for (std::unique_ptr<Block>& newblock : blockList)
							{
								if (newblock->getIndex() == nowPlayerPointBlockIndex)
								{
									//石を置く
									if (cell[index].colorFlag == BIG_W)
									{
										nowPlayingCell->init(
											{
												newblock->getBlockPosition().x,
												cellPosY,
												newblock->getBlockPosition().z
											},
											bigScale,
											cellType::white, true);
										nowPlayingCell->setIndex(index);
										cellList.push_back(std::move(nowPlayingCell));

										nowPlayingCell = std::make_unique<Cell>();
										nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::black, false);
									}
									else if (cell[index].colorFlag == BIG_B)
									{
										nowPlayingCell->init(
											{
												newblock->getBlockPosition().x,
												cellPosY,
												newblock->getBlockPosition().z
											},
											bigScale,
											cellType::black, true);
										nowPlayingCell->setIndex(index);
										cellList.push_back(std::move(nowPlayingCell));

										nowPlayingCell = std::make_unique<Cell>();
										nowPlayingCell->init(newblock->getBlockPosition(), drawScale, cellType::white, false);
									}
								}
							}

							//間のマスをひっくり返す
							for (int n = 1; n < s; n++)
							{
								//ひっくり返すマスのインデックスをセット
								index += dirY * width + dirX;
								count++;

								if (cell[index].GetBigCell() == NONE)
								{
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
								else
								{
									int indexLT = GetBigIndex(index);

									index += dirY * width + dirX;
									n++;

									//ひっくり返す(色を変える)
									for (std::unique_ptr<Cell>& newcell : cellList)
									{
										if (newcell->getIndex() == indexLT)
										{
											newcell->setReverce();
										}
									}

									BigChange(indexLT, bigColor);
								}
							}

							//ひっくり返したら別の方向を探索するため抜ける
							break;
						}

						isBigC = !isBigC;
					}
					else if (cell[index].colorFlag == other)
					{
						if (isBigO)
						{
							break;
						}
					}
					else if (cell[index].colorFlag == bigOther)
					{
						isBigO = !isBigO;
					}
				}
			}
		}

		if (count > 0)
		{
			count += 4;
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

	ColorFlag skipColor[] = {
		ColorFlag::EMPTY,
		ColorFlag::HOLE,
		ColorFlag::BIG_E
	};

	ColorFlag bigColor = ColorFlag::EMPTY; //自分の色(デカマス)
	ColorFlag other = ColorFlag::EMPTY; //相手の色
	ColorFlag bigOther = ColorFlag::EMPTY; //相手の色(デカマス)
	// colorがデカマスだったらノーマルに設定し直す
	if (color == ColorFlag::BIG_B)
	{
		color = ColorFlag::BLACK;
	}
	else if (color == ColorFlag::BIG_W)
	{
		color = ColorFlag::WHITE;
	}
	// それぞれに対応した物を設定する
	if (color == ColorFlag::BLACK)
	{
		bigColor = ColorFlag::BIG_B;
		other = ColorFlag::WHITE;
		bigOther = ColorFlag::BIG_W;
	}
	else if (color == ColorFlag::WHITE)
	{
		bigColor = ColorFlag::BIG_W;
		other = ColorFlag::BLACK;
		bigOther = ColorFlag::BIG_B;
	}

	bool isBigC = false; //デカマスを通るかどうか(自分の色)
	bool isBigO = false; //デカマスを通るかどうか(相手の色)

	//とりあえず全マスを中心とした3*3を全て判定
	// i は一次元配列のインデックス
	for (int i = 0; i < width * height; i++)
	{
		//ブロックに何か置いてあったらとばす
		if (cell[i].colorFlag != ColorFlag::EMPTY && cell[i].colorFlag != ColorFlag::BIG_E)
		{
			continue;
		}

		//現在の一次元配列インデックス
		int index = i;

		//横座標：インデックスをマップの横幅で割った余り
		int x = index % width;

		//縦座標：インデックスをマップの横幅で割り小数点以下切り捨て
		int y = index / width;

		if (cell[i].colorFlag == ColorFlag::EMPTY)
		{
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
					isBigC = false;
					isBigO = false;

					if (cell[index].colorFlag == bigOther)
					{
						isBigO = true;
					}
					//1マス周囲が同じ色か空だったらとばす
					else if (cell[index].colorFlag != other)
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

						//インデックスが範囲外だったらとばす
						if (index < 0 || index >= cell.size())
						{
							break;
						}

						//2マス先以降に石がないなら抜ける
						bool flag = false;
						for (auto dirX : skipColor)
						{
							if (cell[index].colorFlag == dirX)
							{
								flag = true;
								break;
							}
						}
						if (flag)
						{
							break;
						}

						//隣が違う色で、2マス先以降に同じ色があった場合置けるので抜ける
						if (cell[index].colorFlag == color)
						{
							if (isBigC || isBigO)
							{
								break;
							}

							result = false;
							break;
						}
						else if (cell[index].colorFlag == bigColor)
						{
							if (isBigO)
							{
								break;
							}

							if (isBigC)
							{
								result = false;
								break;
							}

							isBigC = !isBigC;
						}
						else if (cell[index].colorFlag == other)
						{
							if (isBigO)
							{
								break;
							}
						}
						else if (cell[index].colorFlag == bigOther)
						{
							isBigO = !isBigO;
						}
					}

					//置けるなら抜ける
					if (result == false)
					{
						break;
					}
				}

				//置けるなら抜ける
				if (result == false)
				{
					break;
				}
			}
		}
		else if (cell[i].colorFlag == ColorFlag::BIG_E)
		{
			if (cell[i].GetBigCell() != BigCell::LT)
			{
				continue;
			}

			for (int dirX = 0; dirX < 4; dirX++)
			{
				index = i;
				if (dirX % 2 == 1)
				{
					index += 1;
				}
				if (dirX / 2 == 1)
				{
					index += width;
				}
				x = index % width;
				y = index / width;

				for (int dir = 0; dir < 3; dir++)
				{
					int dirX = 0, dirY = 0;

					switch (dirX)
					{
					case BigCell::LT:
						if (dir % 2 == 0)
						{
							dirX = -1;
						}
						if (dir / 2 == 0)
						{
							dirY = -1;
						}
						break;
					case BigCell::RT:
						if (dir % 2 == 1)
						{
							dirX = +1;
						}
						if (dir / 2 == 0)
						{
							dirY = -1;
						}
						break;
					case BigCell::LB:
						if (dir % 2 == 0)
						{
							dirX = -1;
						}
						if (dir / 2 == 1)
						{
							dirY = +1;
						}
						break;
					case BigCell::RB:
						if (dir % 2 == 1)
						{
							dirX = +1;
						}
						if (dir / 2 == 1)
						{
							dirY = +1;
						}
						break;
					default:
						break;
					}

					if (x + dirX < 0 || y + dirY < 0 || x + dirX >= width || y + dirY >= height)
					{
						continue;
					}

					index = (y + dirY) * width + (x + dirX);
					isBigC = false;
					isBigO = false;

					if (cell[index].colorFlag == bigOther)
					{
						isBigO = true;
					}
					else if (cell[index].colorFlag != other)
					{
						continue;
					}

					const int size = (width > height) ? width : height;
					for (int s = 2; s < size; s++)
					{
						if (x + (dirX * s) < 0 || y + (dirY * s) < 0 || x + (dirX * s) >= width || y + (dirY * s) >= height)
						{
							break;
						}

						index += dirY * width + dirX;
						if (index >= 0 && index < cell.size())
						{
							bool flag = false;
							for (auto k : skipColor)
							{
								if (cell[index].colorFlag == k)
								{
									flag = true;
									break;
								}
							}
							if (flag)
							{
								break;
							}

							if (cell[index].colorFlag == color)
							{
								if (isBigO || isBigC)
								{
									break;
								}

								result = false;
								break;
							}
							else if (cell[index].colorFlag == bigColor)
							{
								if (isBigO)
								{
									break;
								}

								if (isBigC)
								{
									result = false;
									break;
								}

								isBigC = !isBigC;
							}
							else if (cell[index].colorFlag == other)
							{
								if (isBigO)
								{
									break;
								}
							}
							else if (cell[index].colorFlag == bigOther)
							{
								isBigO = !isBigO;
							}
						}
					}

					if (result == false)
					{
						break;
					}
				}

				if (result == false)
				{
					break;
				}
			}
		}

		//置けるなら抜ける
		if (result == false)
		{
			break;
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

int Othello::BigChange(int index, ColorFlag color)
{
	if (index < 0 || index >= cell.size() ||
		cell[index].colorFlag <= ColorFlag::HOLE ||
		cell[index].GetBigCell() == BigCell::NONE ||
		color == ColorFlag::HOLE)
	{
		return -1;
	}

	int bigIndex[4] = {};
	index = GetBigIndex(index);
	bigIndex[0] = index;
	bigIndex[1] = index + 1;
	bigIndex[2] = index + width;
	bigIndex[3] = index + width + 1;

	if (color == ColorFlag::BLACK)
	{
		color = ColorFlag::BIG_B;
	}
	if (color == ColorFlag::WHITE)
	{
		color = ColorFlag::BIG_W;
	}

	for (int i = 0; i < 4; i++)
	{
		cell[bigIndex[i]].colorFlag = color;
	}

	return bigIndex[0];
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
	nowColor = GetStartColor();

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
				for (int dirX = 0; dirX < 4; dirX++)
				{
					if (itr[dirX] == i)
					{
						isHit = true;
						bigCellState = static_cast<BigCell>(dirX);
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
		if (cell[i].GetBigCell() != BigCell::NONE && cell[i].GetBigCell() != BigCell::LT)
		{
			continue;
		}

		int x = i % width;
		int z = i / width;

		if (cell[i].colorFlag == HOLE || cell[i].colorFlag == NONE)
		{
			continue;
		}

		//ブロックを置く
		blockType type = blockType((x + z) % 2);

		std::unique_ptr<Block> newblock = std::make_unique<Block>();

		XMFLOAT3 scale =
		{
			drawScale.x * ((cell[i].GetBigCell() == BigCell::LT) + 1),
			drawScale.y,
			drawScale.z * ((cell[i].GetBigCell() == BigCell::LT) + 1)
		};

		XMFLOAT3 pos =
		{
			blockDrawOffsetX + ((float)x * blockDistance) * drawScale.x,
			-30.0f,
			blockDrawOffsetZ - ((float)z * blockDistance) * drawScale.z
		};
		pos.x += drawOffset.x;
		pos.y += drawOffset.y;
		pos.z += drawOffset.z;

		newblock->init(type, pos, scale, i);

		pos.x += blockDistance * (cell[i].GetBigCell() == BigCell::LT) / 2.0f;
		pos.z -= blockDistance * (cell[i].GetBigCell() == BigCell::LT) / 2.0f;
		newblock->blockObject->SetPosition(pos);
		newblock->startPos = newblock->blockObject->getPosition();

		blockList.push_back(std::move(newblock));

		//石を置く
		if (cell[i].colorFlag == WHITE || cell[i].colorFlag == ColorFlag::BIG_W)
		{
			//新しい石
			std::unique_ptr<Cell> newcell = std::make_unique<Cell>();
			newcell->init(
				{
					blockList.back()->getBlockPosition().x,
					cellPosY,
					blockList.back()->getBlockPosition().z
				},
				scale,
				cellType::white, true);
			newcell->setIndex(i);
			cellList.push_back(std::move(newcell));
		}
		else if (cell[i].colorFlag == BLACK || cell[i].colorFlag == ColorFlag::BIG_B)
		{
			//新しい石
			std::unique_ptr<Cell> newcell = std::make_unique<Cell>();
			newcell->init(
				{
					blockList.back()->getBlockPosition().x,
					cellPosY,
					blockList.back()->getBlockPosition().z
				},
				scale,
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

int Othello::GetBigIndex(int index) const
{
	if (index < 0 || index >= cell.size() ||
		cell[index].GetBigCell() == BigCell::NONE)
	{
		return -1;
	}

	if (cell[index].GetBigCell() % 2 == 1)
	{
		index -= 1;
	}
	if (cell[index].GetBigCell() / 2 == 1)
	{
		index -= width;
	}

	return index;
}
