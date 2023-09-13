#include "sceneManager.h"

sceneManager::sceneManager()
{
	nowscene = new titleScene();

	nowscene->setParameter();
}

sceneManager::~sceneManager()
{
	delete(nowscene);
}

void sceneManager::replacementScene()
{
	if (!nowscene->isNextScene)
	{
		return;
	}

	switch (nowscene->thisType)
	{
	case gameSceneType::title:
		delete(nowscene);
		nowscene = new selectScene();
		nowscene->setParameter();
		break;
	case gameSceneType::select:
		delete(nowscene);
		if (primitiveScene::stageNum == -1)
		{
			nowscene = new titleScene();
			nowscene->setParameter();
		}
		else
		{
			nowscene = new playScene();
			nowscene->setParameter();
		}
		break;
	case gameSceneType::play:
		delete(nowscene);
		if (primitiveScene::isTitle)
		{
			nowscene = new titleScene();
			nowscene->setParameter();
		}
		else if (primitiveScene::isSelect)
		{
			nowscene = new selectScene();
			nowscene->setParameter();
		}
		else if (primitiveScene::isResult)
		{
			nowscene = new resultScene();
			nowscene->setParameter();
		}
		break;
	case gameSceneType::result:
		delete(nowscene);
		if (primitiveScene::isPlay)
		{
			nowscene = new playScene();
			nowscene->setParameter();
		}
		else if (!primitiveScene::isSelectOrTitle)
		{
			nowscene = new titleScene();
			nowscene->setParameter();
		}
		else if (primitiveScene::isSelectOrTitle)
		{
			nowscene = new selectScene();
			nowscene->setParameter();
		}
		break;
	default:
		break;
	}
}
