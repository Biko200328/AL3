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

	worldTransform_[0].translation_ = {0.0f, 5.0f, 0.0f};
	worldTransform_[1].translation_ = {-5.0f, 0.0f, 0.0f};
	worldTransform_[2].translation_ = {5.0f, 0.0f, 0.0f};

	for (int i = 0; i < 3; i++) {
		worldTransform_[i].Initialize();
	}

	viewProjection_.target = {0.0, 0.0, 0.0};
	viewProjection_.Initialize();
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		x++;
		if (x >= 3)
			x = 0;
	}

	if (x == 0)viewProjection_.target = {0.0, 5.0, 0.0};
	else if (x == 1)viewProjection_.target = {-5.0, 0.0, 0.0};
	else if (x == 2)viewProjection_.target = {5.0, 0.0, 0.0};

	viewProjection_.UpdateMatrix();

	debugText_->Printf("%d", x);
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
	for (int i = 0; i < 3; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, mario);
	}

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
