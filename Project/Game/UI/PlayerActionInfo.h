/*
現在のプレイヤーの行動状態を表すUI
作成日：2021/10/15
*/
#pragma once


//列挙型の使用
#include"../PlayScene/PlaySceneCommon/Player/PlayerActionState.h"
#include"../PlayScene/PlaySceneCommon/Weapon/WeaponeType.h"

//所持
#include"../Common/SpriteNumber/SpriteNumber2D.h"
#include"../Common/ObjectSpriteTexture.h"


class PlayerActionInfo
{
private:
	//下地画像の分割位置
	static const float GROUNDWORK_TEX_HALF_CUT_POSITIONY;

	//下地画像の拡大率
	static const DirectX::SimpleMath::Vector2 GROUNDWORK_TEX_SCALE;
	//下地画像の座標
	static const DirectX::SimpleMath::Vector2 GROUNDWORK_TEX_POSITION;

	//壁建状態の切り取り位置
	static const RECT BLOCK_CREATE_RECT;
	//攻撃状態の切り取り位置
	static const RECT ATTACK_ENEMY_RECT;

	//武器画像の座標
	static const DirectX::SimpleMath::Vector2 WEAPON_TEX_SCALE;
	//武器画像の拡大率
	static const DirectX::SimpleMath::Vector2 WEAPON_TEX_POSITION;

	//弱攻撃画像のずらす幅
	static const DirectX::SimpleMath::Vector2 WEAPON_TEX_SHIFT;

	//武器画像の分割サイズ
	static const int WEAPON_SIZE;

	//武器画像の初期位置
	static const RECT WEAPON_INIT_RECT;

	//弾数の描画位置
	static const DirectX::SimpleMath::Vector2 BULLET_NUM_POSITION;



	//下地の画像
	std::unique_ptr<ObjectSpriteTexture> mpGroundworkTexture;

	//各種武器画像
	std::unique_ptr<ObjectSpriteTexture> mpWeaponSpriteTexture;

	std::unique_ptr<SpriteNumber2D> mpSpriteWeaponBulletNum;

	//プレイヤーの状態
	ePlayerActionState mActionState;

	//プレイヤーの武器
	eWeaponType mPlayerWeapon;

	//現在の弾数
	int mStrongWeaponBulletNum;

public:

	//コンストラクタ
	PlayerActionInfo();
	//デストラクタ
	~PlayerActionInfo();
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Draw();


	//プレイヤーの状態のアクセサ
	void SetPlayerActionState(ePlayerActionState const& state) { mActionState = state; this->ChangeTexture(); }
	//所持している武器の変更
	void SetPlayerWeaponType(eWeaponType const& type) { mPlayerWeapon = type; this->ChangeTexture(); }

	//切り取り位置を切り替える
	void ChangeTexture();

	//現在の弾数のアクセサ
	void StrongWeaponBulletNum(int const& num) { mStrongWeaponBulletNum = num; };
};
