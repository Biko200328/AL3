#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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
	for (int i = 0; i < 18; i++) {
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};
		if (i < 9)worldTransform_[i].translation_ = {- 36.0f + (i * 10), 20.0f, 0};
		else if (i >= 9 && i < 18)worldTransform_[i].translation_ = {-36.0f + ((i - 9) * 10), -20.0f, 0};
		worldTransform_[i].Initialize();
	}
	
	viewProjection_.Initialize();
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
	
	for (int i = 0; i < 18; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, mario);
	}

	/*debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "%f,%f,%f", worldTransform_[11].translation_.x, worldTransform_[11].translation_.y,
	  worldTransform_[11].translation_.z);*/

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
