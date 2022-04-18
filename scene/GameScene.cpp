#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	mario = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.rotation_ = {XMConvertToRadians(45.0f), XMConvertToRadians(45.0f), 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.Initialize();

	// 乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> eyeDist(-100.0f, 100.0f);

	x = 0;

	for (size_t i = 0; i < _countof(viewProjection_); i++) {
		viewProjection_[i].eye = {eyeDist(engine), eyeDist(engine), eyeDist(engine)};
		viewProjection_[i].Initialize();
	}
}

void GameScene::Update() { }

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
	if (input_->TriggerKey(DIK_SPACE)) {
		x++;
		if (x >= 3)
			x = 0;
	}
		model_->Draw(worldTransform_, viewProjection_[x], mario);

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
