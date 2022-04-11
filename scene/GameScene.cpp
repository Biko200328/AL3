#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	mario = TextureManager::Load("mario.jpg");
	sound_ = audio_->LoadWave("se_sad03.wav");

	//スプライトの生成
	sprite_ = Sprite::Create(mario, {50, 50});

	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	/*sound_ = audio_->PlayWave(sound_, true);*/
}

void GameScene::Update() {
	//スプライトの今の座標を取得
	XMFLOAT2 potision = sprite_->GetPosition();
	//座標を{2,0}移動
	potision.x += 2.0f;
	potision.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(potision);

	if (input_->TriggerKey(DIK_SPACE))
	{
		audio_->StopWave(sound_);
	}

	value_++;
	std::string strDebug = std::string("Value:") + std::to_string(value_);

	debugText_->Print(strDebug, 50, 90, 1.0f);

	/*debugText_->Print("Kaizokuou ni oreha naru.", 50, 50, 1.0f);
	debugText_->SetPos(50, 70);
	debugText_->Printf("year:%d", 2022);*/
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
	//2Dモデルの描画
	sprite_->Draw();

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

	//3Dモデルの描画
	model_->Draw(worldTransform_,viewProjection_,mario);

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
