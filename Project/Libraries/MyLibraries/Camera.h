/*
メインカメラ
作成日：2021/10/24
*/
#pragma once



//基底クラス
#include"MoveCamera.h"


class Camera :public MoveCamera
{
private:
	//定数

	//カメラ座標
	static const DirectX::SimpleMath::Vector3 EYE_TO_VEC;
	//最初のターゲット座標
	static const DirectX::SimpleMath::Vector3 TARGET_INIT_TO_VEC;
	//ターゲット座標
	static const DirectX::SimpleMath::Vector3 TARGET_TO_VEC;	

	//移動制限
	static const float MOVE_RESTRICTION_MAX_Z;

	// 参照視点
	DirectX::SimpleMath::Vector3 mRefEye;

	// 参照注視点
	DirectX::SimpleMath::Vector3 mRefTarget;

	//使用カメラ視点
	DirectX::SimpleMath::Vector3 mUseEye;


public:
	// コンストラクタ
	Camera();

	//常時処理
	void Update(DirectX::SimpleMath::Vector3 modelpos);
	void Update();

	// デストラクタ
	~Camera();

	// カメラ座標のアクセサ
	void SetRefEyePosition(DirectX::SimpleMath::Vector3 eye) { mRefEye = eye; }
	DirectX::SimpleMath::Vector3 GetRefEyePosition() const { return mRefEye; }
		//さらに細かいアクセサ
		//X
		void SetRefEyePositionX(float eyex) { mRefEye.x = eyex; }
		float GetRefEyePositionX() const { return mRefEye.x; }
		//Y
		void SetRefEyePositionY(float eyey) { mRefEye.y = eyey; }
		float GetRefEyePositionY() const { return mRefEye.y; }
		//Z
		void SetRefEyePositionZ(float eyez) { mRefEye.z = eyez; }
		float GetRefEyePositionZ() const { return mRefEye.z; }

	// 参照注視点座標のアクセサ
	void SetRefTargetPosition(DirectX::SimpleMath::Vector3 target) { mRefTarget = target; }
	DirectX::SimpleMath::Vector3 GetRefTargetPosition() const { return mRefTarget; }
		//さらに細かいアクセサ
		//X
		void SetRefTargetPositionX(float targetx) { mRefTarget.x = targetx; }
		float GetRefTargetPositionX() const { return mRefTarget.x; }
		//Y
		void SetRefTargetPositionY(float targety) { mRefTarget.y = targety; }
		float GetRefTargetPositionY() const { return mRefTarget.y; }
		//Z
		void SetRefTargetPositionZ(float targetz) { mRefTarget.z = targetz; }
		float GetRefTargetPositionZ() const { return mRefTarget.z; }
};
