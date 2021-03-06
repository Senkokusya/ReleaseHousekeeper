/*
ステージ選択シーン
*/
#pragma once


#include "../IScene.h"
#include "Game/GameMain.h"

//選択した情報をプレイシーンへ(構造体を使用)
#include"Game/Interface/SceneInterface/StoPSceneInterface.h"

//列挙型の使用
#include"StageNumer.h"

//所持
#include"../Common/Cursor.h"
#include"StageInfomation.h"
#include"StageNumberTag.h"
#include"ConfirmationScreen.h"
#include"../Common/Blinking.h"
#include"../Common/Cloud.h"
#include"../Common/ObjectTexture.h"

class StageSelectScene : public IScene
{
private:
	//番号の縦移動
	static const int SELECTNUM_VERTICAL_SHIFT;

	//1画面当たりの最大ステージ数
	static const int SCREEN_STEGE_MAX_NUM = 6;

	//カーソルの拡大率
	static const DirectX::SimpleMath::Vector2 CURSOR_SCALE;
	//初期座標
	static const DirectX::SimpleMath::Vector2 CURSOR_INIT_POSITION;
	//移動量
	static const DirectX::SimpleMath::Vector2 CURSOR_VELOCITY;

	//タグの初期座標
	static const DirectX::SimpleMath::Vector2 TAG_INIT_POSITION;
	//幅
	static const float TAG_SHIFT;

	//確認画面用カーソルの基本情報
	//拡大率
	static const DirectX::SimpleMath::Vector2 SELECT_CURSOR_SCALE;
	//初期座標
	static const DirectX::SimpleMath::Vector2 SELECT_CURSOR_INIT_POSITION;
	//移動幅
	static const float SELECT_CURSOR_SHIFT;

	//題の位置
	static const DirectX::SimpleMath::Vector2 NAME_POSITION;
	static const DirectX::SimpleMath::Vector2 NAME_SCALE;



	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mpTexture;

	//カーソル
	std::unique_ptr<Cursor> mpCursor;

	//ステージ各種情報
	std::unique_ptr<StageInfomation> mpStageInfo;

	//タグ
	std::unique_ptr<StageNumberTag> mpTag[SCREEN_STEGE_MAX_NUM];

	//確認画面
	std::unique_ptr<ConfirmationScreen> mpConfirScreen;

	//点滅
	std::unique_ptr<Blinking> mpTextureBlinking;

	//題
	std::unique_ptr<ObjectTexture> mpStageSelectNameTexture;

	//雲
	std::unique_ptr<Cloud> mpCloud;

	//現在選択中の番号
	int mSelectNum;

	//選択フラグ
	bool mSelectFlag;

	//BGMのID
	int mBGMID;

public:

	// コンストラクタ
	StageSelectScene();

	// デストラクタ
	~StageSelectScene();

	// 初期化
	void Initialize() override;

	// 更新
	eGameScene Update() override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

private:
	//モード選択
	void StageSelect();

	//ファイル名算出
	std::string SetFileName();

	//カーソルをあげる
	void CursorUp();

	//カーソルをあげる
	void CursorDown();


	//番号を設定する
	void SetNumberTag();

private:
	//BGMを流す
	void PlayBGM()override;

	//BGMを止める
	void StopBGM()override;

};