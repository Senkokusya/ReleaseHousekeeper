/*
プレイフェーズの状態遷移の列挙を管理するヘッダーファイル
*/
#pragma once


enum class ePlayScenePhase
{
	ROTATION_CAMERA,//カメラ周り状態
	COUNTDOWN,		//カウントダウン状態
	DEFENCE,		//防衛状態
	WEAPON_SHOP,	//武器買い状態
	END_ANIMATION,	//終了演出
	RESULT,			//終了

	PHASE_MAX_NUM	//フェーズ数
};

