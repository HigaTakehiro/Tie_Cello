#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
Light* primitiveScene::light = nullptr;
Camera* primitiveScene::camera = nullptr;
std::unique_ptr<SingleSprite> primitiveScene::sample_back = std::make_unique<SingleSprite>();
int primitiveScene::stageNum = 0;
XMFLOAT3 primitiveScene::stageIconOffset = { 340,360,0 };
bool primitiveScene::isTutorial = false;
bool primitiveScene::isClearOrOver = false;
bool primitiveScene::isSelectOrTitle = false;
std::unique_ptr<Othello> primitiveScene::othello = std::make_unique<Othello>();
std::unique_ptr<EvenlyStaging> primitiveScene::evenry = std::make_unique<EvenlyStaging>();

primitiveScene::primitiveScene()
{

}

primitiveScene::~primitiveScene()
{

}

void primitiveScene::setStaticData(directX* Directx, dxinput* Input, Audio* Audio)
{
	directx = Directx;
	input = Input;
	audio = Audio;

	//スプライトクラス初期化
	SingleSprite::setStaticData(directx->dev.Get(), input);

	//パーティクル初期化
	SingleParticle::particleStaticInit(directx, nullptr);

	//背景
	sample_back->size = { 1280,720 };
	sample_back->generateSprite("sample_back.jpg");

	//ライト生成
	Light::Staticinitialize(directx->dev.Get());
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });
	light->SetLightDir({ 0,-1,0,0 });

	camera = new Camera();
	camera->SetEye({ 0.0f,100.0f,-10.0f });
	camera->SetTarget({ 0.0f,-30.0f,0.0f });
	camera->SetUp({ 0,0.01f,0.01f });
	camera->Update();

	//3dオブジェクト生成
	object3dFBX::setLight(light);
	object3dFBX::SetCamera(camera);
	object3dFBX::SetDevice(directx->dev.Get());
	object3dFBX::CreateGraphicsPipeline();
	object3dFBX::CreateGraphicsPipelineSimple();

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(camera);

	//マウスカーソル非表示
	//ShowCursor(false);

	Cell::setStaticData(directx);
	Block::setStaticData(directx);
}

void primitiveScene::finalize()
{

}
