/*
弾管理クラス
作成日：2021/09/17
*/
#include"pch.h"

#include"BulletManager.h"

//前方宣言
#include"../ObjectModel/BoxModel.h"
#include"../Enemy/Enemy.h"
#include"../Block/BlockManager.h"
#include"BulletStateBoomerang.h"
#include"../Enemy/EnemyManager.h"

//エフェクトの使用
#include"../../Effect/EffectManager.h"
#include"../../Effect/Effect.h"

//関数の使用
#include"Libraries/MyLibraries/Camera.h"
#include"../Player/Player.h"
#include"../Enemy/EStateKnockBack.h"
#include"../ObjectModel/SphereModel.h"


//最大範囲
const float BulletManager::BULLET_MAX_POSITION_Z = -50.f;

//インターバル計測終了用初期化用定数
const int BulletManager::BULLET_INTERVAL_FINISH_NUM = -1;

/*===================================
コンストラクタ
===================================*/
BulletManager::BulletManager()
	:
	mpBullet{ nullptr },
	mpBulletType(nullptr),
	mBulletShotIntervalFlag(false),
	mIntervalTimer(Utility::EXCEPTION_NUM),
	mWeaponType(),
	mpCamera(nullptr),
	mpBoomerangBullet(nullptr),
	mpEnemyManager(nullptr)
{
	//弾の初期準備
	for (int i = 0; i < BULLET_MAX_NUM ; i++)
	{
		mpBullet[i] = std::make_unique<Bullet>();
	}

	//弾情報管理クラス
	mpBulletType = std::make_unique<BulletType>();

	//ブーメランの処理クラス
	mpBoomerangBullet = std::make_unique<BulletStateBoomerang>();
}

/*===================================
デストラクタ
===================================*/
BulletManager::~BulletManager()
{
}

/*=============================================================================================
初期化処理
引数：カメラのポインタ、プレイヤーのポインタ、敵管理クラスのポインタ
===============================================================================================*/
void BulletManager::Initialize(CameraBase* pCamera, Player* pPlayer, EnemyManager* pEnemyManager)
{
	//全ての弾の初期化処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		mpBullet[i]->Initialize();
	}

	//ポインタ保存
	mpCamera = pCamera;
	mpEnemyManager = pEnemyManager;

	//ファイル読み込み
	mpBulletType->SetCSV("Resources/CSV/WeaponInfomation.csv");

	//座標をあらかじめ保存しておく
	mpBoomerangBullet->SavePlayerPointer(pPlayer);

}
/*==========================================
弾生成処理
引数：弾の出現座標、弾の種類
返り値：弾の生成の成功か失敗か(true：成功)
==========================================*/
bool BulletManager::Create(DirectX::SimpleMath::Vector3 const& pos)
{
	//もし発射できる状態ではないなら処理を終える
	if (!mBulletShotIntervalFlag)
	{
		return false;
	}

	//全ての弾を回す
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//もし弾が使用されていたらスキップする
		if (mpBullet[i]->GetUseFlag())
		{
			continue;
		}

		//もし対象の弾ならポインタを渡す
		if (mWeaponType == eWeaponType::BOOMERANG)
		{
			mpBoomerangBullet->SetBuletPointer(mpBullet[i].get());
			mpBullet[i]->SetBoomerangPointer(mpBoomerangBullet.get());
		}


		//弾を作成
		mpBullet[i]->Create(pos, mWeaponType);
		//対応した弾の種類を入れ、その弾の情報を弾単体に保存保存
		mpBullet[i]->SetBulletInfomation(*mpBulletType.get()->GetBulletInfomation(mWeaponType));

		//インターバル計測開始
		this->ResetIntervalTimer();

		//作成できたら抜ける
		return true;
	}

	return false;
}

/*=================================
更新処理
===================================*/
void BulletManager::Update()
{
	//全ての弾の更新処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態なら処理をしない
		if (!mpBullet[i]->GetUseFlag())
		{
			//未使用状態で


			continue;
		}

		mpBullet[i]->Update();
	}

	//インターバル計測処理
	mBulletShotIntervalFlag = this->MeasureInterval();

	//範囲外処理
	this->OutOfRange();
}

/*==================================
描画処理
===================================*/
void BulletManager::Draw()
{
	//全ての弾の描画処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態なら処理をしない
		if (!mpBullet[i]->GetUseFlag())
		{
			continue;
		}
		mpBullet[i]->Draw();
	}
}

/*==================================
弾と敵の衝突判定
引数：敵単体のポインタ
返り値：敵の追加撃破数
===================================*/
int BulletManager::ChackHitBulletEnemy(Enemy* pEnemy)
{
	//当たり判定関数群の取得
	Collider& pC = Collider::GetColliderInstance();

	int breakNum = 0;
	bool hitflag = false;
	//弾の数だけ回す
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態の弾は処理を通さない
		if (!mpBullet[i]->GetUseFlag())
		{
			continue;
		}

		//もし当たっていたら
		if (pC.Intersect(
			mpBullet[i]->GetBulletModelCollider(), 
			pEnemy->GetBoxModel()->GetAABBCollider()))
		{
			
			bool breakflag = false;
			hitflag = true;

			//ダメージ値を先に出しておく
			int damageNum(mpBullet[i]->GetBulletAttackNum());


			//弾の種類に応じて処理を変える
			switch (mpBullet[i]->GetBulletWeaponType())
			{
				case eWeaponType::MACHINE_GUN:
				case eWeaponType::PISTOL:			//NoBreak
				{
					//もしまだ当たったことのない弾だったら
					if (pEnemy->GetSaveBulletNumber()!=i)
					{
						breakflag = pEnemy->Damage(damageNum);

						//エフェクトを設定する
						this->SetEffect(eEffectType::SLASH, pEnemy->GetBoxModel()->GetPosition());

						//接触した弾番号を保存する
						pEnemy->SetSaveBulletNumber(i);
					}
					
					break;
				}		
				case eWeaponType::BOMB:
				case eWeaponType::ROCKET_LAUNCHER://NoBreak
				{
					//敵にダメージを与える
					breakflag = pEnemy->Damage(mpBullet[i]->GetBulletAttackNum());
					//爆発フラグを建てる
					mpBullet[i]->SetExplosionFlag(true);
					mpBullet[i]->SetDoubleCollider();
					//エフェクトを設定する
					this->SetEffect(eEffectType::BOMB, pEnemy->GetBoxModel()->GetPosition());

					//対象のブーメラン接触フラグを立てる
					pEnemy->SetBoomerangHitFlag(true);

					//接触した弾番号を保存する
					pEnemy->SetSaveBulletNumber(i);

					//ノックバックの方向を算出する
					DirectX::SimpleMath::Vector3 knockBackVelocity = pEnemy->GetBoxModel()->GetPosition() -
						mpBullet[i]->GetBulletModel()->GetPosition();
					knockBackVelocity.y = 0.f;
					pEnemy->GetStateKnockBack()->SetMoveVector(knockBackVelocity);
					pEnemy->ChangeStateKnockBack();
					
					break;
				}
				case eWeaponType::BOOMERANG:
				{
					//ブーメランに当たっていなかったら
					if (!pEnemy->GetBoomerangHitFlag())
					{
						//敵にダメージを与える
						breakflag = pEnemy->Damage(mpBullet[i]->GetBulletAttackNum());
						//エフェクトを設定する
						this->SetEffect(eEffectType::SLASH, pEnemy->GetBoxModel()->GetPosition());
						//対象のブーメラン接触フラグを立てる
						pEnemy->SetBoomerangHitFlag(true);
					}
					
					break;
				}
			}

			//ダメージエフェクトをリセットして
			pEnemy->ResetNumberEffect();
			//作成する
			pEnemy->CreateNumberEffect(pEnemy->GetEnemyDamage());

			//倒せていたら増加させる
			if (breakflag)
			{
				breakNum++;
			}

			//ブーメラン、爆発系弾以外なら
			if (mpBullet[i]->GetBulletWeaponType() != eWeaponType::BOOMERANG&&
				mpBullet[i]->GetBulletWeaponType() != eWeaponType::BOMB&&
				mpBullet[i]->GetBulletWeaponType() != eWeaponType::ROCKET_LAUNCHER)
			{
				//弾を消滅させる
				mpBullet[i]->SelfDestroy();

				//番号が消滅した番号と同じならば
				if (pEnemy->GetSaveBulletNumber() == i)
				{
					//番号を初期化する
					pEnemy->SetSaveBulletNumber(Utility::EXCEPTION_NUM);
				}
			}
		}
		//ブーメランに当たっていなかったら
		else if(pEnemy->GetBoomerangHitFlag())
		{
			//対象のブーメラン接触フラグを下す
			pEnemy->SetBoomerangHitFlag(false);
		}
		
	}
	return breakNum;
}

/*=================================
プレイヤーと弾の衝突判定
引数：プレイヤーのポインタ
===================================*/
void BulletManager::ChackHitBulletPlayer(Player* pPlayer)
{
	//当たり判定関数群の取得
	Collider& pC = Collider::GetColliderInstance();

	//全ての弾の描画処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態なら処理をしない
		if (!mpBullet[i]->GetUseFlag())
		{
			continue;
		}
		//ブーメラン以外でも処理を通さない
		if (mpBullet[i]->GetBulletWeaponType() != eWeaponType::BOOMERANG)
		{
			continue;
		}
		if (mpBoomerangBullet->GetBoomerangState() == eBoomerangState::SHOT||
			mpBoomerangBullet->GetBoomerangState() == eBoomerangState::NONE)
		{
			continue;
		}

		//当たり判定を取る
		if (pC.Intersect(
			mpBullet[i]->GetBulletModelCollider(),
			pPlayer->GetPlayerModel()->GetAABBCollider()))
		{
			//当たっていたら
			mpBoomerangBullet->ChangeBoomerangState(eBoomerangState::CATCH);
		}
	}
}

/*===================================
ブロックとの衝突判定を回す関数
引数：ブロック管理クラスのポインタ
===================================*/
void BulletManager::RoolChackHitBlock(BlockManager* pBlockManager)
{
	//全ての弾の描画処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態なら処理をしない
		if (!mpBullet[i]->GetUseFlag())
		{
			continue;
		}

		//当たり判定処理
		pBlockManager->ChackBulletBlock(mpBullet[i].get());
	}
}

/*===================================
範囲外の弾の処理:private
===================================*/
void BulletManager::OutOfRange()
{
	//全ての弾の描画処理
	for (int i = 0; i < BULLET_MAX_NUM; i++)
	{
		//未使用状態なら処理をしない
		if (!mpBullet[i]->GetUseFlag())
		{
			continue;
		}
		//ブーメランでも処理をしない
		if (mpBullet[i]->GetBulletWeaponType() == eWeaponType::BOOMERANG)
		{
			continue;
		}

		//もしZ座標が範囲外になったら
		if (mpBullet[i]->GetBulletPositionZ() <= BULLET_MAX_POSITION_Z)
		{
			//未使用状態とする
			mpBullet[i]->SelfDestroy();

			//弾番号をリセットする
			mpEnemyManager->ResetBulletNum(i);
		}
	}
}

/*=============================================
インターバル計測:private
返り値：弾発射可能か不可能か(true：可能)
=============================================*/
bool BulletManager::MeasureInterval()
{
	if (mIntervalTimer == BULLET_INTERVAL_FINISH_NUM)
	{
		return true;
	}

	//タイマーを更新
	mIntervalTimer++;

	//武器種に応じたインターバル処理を行う
	//簡略化変数の宣言
	int intervalNum = mpBulletType.get()->GetBulletInfomation(mWeaponType)
													->mBulletIntervalNum;
	//インターバルが終わったら
	if (mIntervalTimer == intervalNum)
	{
		mIntervalTimer = BULLET_INTERVAL_FINISH_NUM;

		return true;
	}

	return false;
}

/*====================================================
エフェクトを設定する:private
引数：エフェクトの種類、座標
====================================================*/
void BulletManager::SetEffect(eEffectType const& type, DirectX::SimpleMath::Vector3 const& pos)
{
	//エフェクトマネージャーの取得
	auto pEM = EffectManager::GetInstance();

	auto effect = pEM->GetEffect(type);
	effect->SetPosition(pos + DirectX::SimpleMath::Vector3::UnitY);
	effect->SetSpriteFlag(true);
	effect->SetLoopFlag(false);
}
