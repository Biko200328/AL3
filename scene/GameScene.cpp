#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	mario = TextureManager::Load("mario.jpg");
	
	// 3Dモデルの生成
	model_ = Model::Create();

	//大元
	/*worldTransform_[PartId::Root].translation_ = {0, 0, 10.0f};*/
	worldTransform_ [PartId::Root].Initialize();

	//頭
	worldTransform_[PartId::Head].translation_ = {0, 0, 2.0f};
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Head].Initialize();

	//左腕
	worldTransform_[PartId::ArmL].translation_ = {-2.0f, 0, 0};
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::ArmL].Initialize();

	//右腕
	worldTransform_[PartId::ArmR].translation_ = {2.0f, 0, 0};
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::ArmR].Initialize();

	//置物
	worldTransform_[99].translation_ = {10.0f, 0, 0};
	worldTransform_[99].Initialize();

	viewProjection_.eye = {0, 10, -20};
	viewProjection_.target = worldTransform_[PartId::Root].translation_;

	viewProjection_.Initialize();
}

void GameScene::Update() {

	XMFLOAT3 move = {0, 0, 0};

	const float kChestRotSpeed = 0.05f;

	if (input_->PushKey(DIK_LEFT)) {
		move = {0, -kChestRotSpeed, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {0, kChestRotSpeed, 0};
	}

	worldTransform_[PartId::Root].rotation_.y += move.y;

	XMFLOAT3 vec = {0, 0, 0};
	const float kCharacterSpeed = 0.1f;

	if (input_->PushKey(DIK_UP)) {
		vec = {
		sinf(worldTransform_[PartId::Root].rotation_.y) * kCharacterSpeed, 0,
		cosf(worldTransform_[PartId::Root].rotation_.y) * kCharacterSpeed};
	} else if (input_->PushKey(DIK_DOWN)) {
		vec = {
		  sinf(worldTransform_[PartId::Root].rotation_.y) * (-kCharacterSpeed), 0,
		  cosf(worldTransform_[PartId::Root].rotation_.y) * (-kCharacterSpeed)};
	}

	worldTransform_[PartId::Root].translation_.x += vec.x;
	worldTransform_[PartId::Root].translation_.z += vec.z;


	viewProjection_.target = worldTransform_[PartId::Root].translation_;
	viewProjection_.eye.x += vec.x;
	viewProjection_.eye.z += vec.z;

	

	////長さ
	//float vecLen = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	////正規化
	//vec = {vec.x / vecLen, vec.y / vecLen, vec.z / vecLen};

	worldTransform_[PartId::Root].UpdateMatrix();
	worldTransform_[PartId::Head].UpdateMatrix();
	worldTransform_[PartId::ArmL].UpdateMatrix();
	worldTransform_[PartId::ArmR].UpdateMatrix();

	viewProjection_.UpdateMatrix();

	debugText_->SetPos(0, 0);
	debugText_->Printf(
	  "obj	x : %f	y : %f	z : %f", worldTransform_->translation_.x, worldTransform_->translation_.y,
	  worldTransform_->translation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf(
	  "camera	x : %f	y : %f	z : %f", viewProjection_.eye.x, viewProjection_.eye.y,
	  viewProjection_.eye.z);
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model_->Draw(worldTransform_[PartId::Root], viewProjection_, mario);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, mario);
	model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, mario);
	model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, mario);

	model_->Draw(worldTransform_[99], viewProjection_, mario);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
