/*
モデル描画クラス
作成日：2021/04/18
*/
#include<pch.h>
#include"ObjectModel.h"


//初期化時の定数
const DirectX::SimpleMath::Vector3 ObjectModel::INIT_POSITION= DirectX::SimpleMath::Vector3::Zero;
//未使用時の定数
const DirectX::SimpleMath::Vector3 ObjectModel::NOT_USE_POSITION(-20.f, -20.f, -20.f);

/*==============
コンストラクタ
==============*/

ObjectModel::ObjectModel()
	:
	IModel(),
	mpCommonState(nullptr),
	mpSpriteBatch(nullptr),
	mWorld(),
	mView(),
	mProj(),
	mFxFactory(nullptr),
	mModel(nullptr),
	mPosition(),
	mScale(),
	mRotX(),
	mRotY(),
	mRotZ(),
	mPosition3(),
	mScale3(),
	mRotAngleX(),
	mRotAngleY(),
	mRotAngleZ(),
	mDrawFlag(true),
	mModelUseFlag(false)
{
	DirectXtkIF& pDxIF = DirectXtkIF::GetDirectXtkIFInstance();
	mpCommonState = pDxIF.GetCommonStates();
	mpSpriteBatch = pDxIF.GetSpriteBatch();
	//インスタンスの取得
		//ビュー行列、プロジェクション行列の取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* pDevice = pDR->GetD3DDevice();
	CameraInfoIF& pCiIF = CameraInfoIF::GetCameraInfoIFInstance();
	mView = *pCiIF.GetView();
	mProj = *pCiIF.GetProjection();

	// モデルデータの入っているフォルダの指定
	std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(pDevice);
	factory->SetDirectory(L"Resources/Models/");


	mFxFactory = std::make_unique<DirectX::EffectFactory>(pDevice);
	mWorld = DirectX::SimpleMath::Matrix::Identity;



	//未代入の回避
	mScale3 = DirectX::SimpleMath::Vector3::Zero;
	mPosition3 = INIT_POSITION;	//初期化
	mPosition3 = NOT_USE_POSITION;//未使用用座標の指定
	mScale = DirectX::SimpleMath::Matrix::CreateScale(mScale3);
	mPosition = DirectX::SimpleMath::Matrix::CreateTranslation(mPosition3);
}

/*===================================
デストラクタ
===================================*/
ObjectModel::~ObjectModel()
{
}

/*========================
モデルの指定
引数：モデル情報
============================*/
void ObjectModel::SetModel(DirectX::Model* model)
{
	mModel = model;
}

/*============
座標の指定
引数：座標
==============*/
void ObjectModel::SetPosition(const DirectX::SimpleMath::Vector3& pos)
{
	mPosition = DirectX::SimpleMath::Matrix::CreateTranslation(pos);

	mPosition3 = pos;

}

/*============
拡大率の指定
引数：拡大率
==============*/
void ObjectModel::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	mScale = DirectX::SimpleMath::Matrix::CreateScale(scale);
	mScale3 = scale;
}

/*============
回転の指定
引数：角度
==============*/
void ObjectModel::SetRotationX(const float& rot)
{
	mRotAngleX = rot;
	mRotX = DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(rot));
}
void ObjectModel::SetRotationY(const float& rot)
{
	mRotAngleY = rot;
	mRotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rot));
}
void ObjectModel::SetRotationZ(const float& rot)
{
	mRotAngleZ = rot;
	mRotZ = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rot));
}


/*========================
座標の取得
返り値：座標(X,Y,Z)
============================*/
DirectX::SimpleMath::Vector3 ObjectModel::GetPosition()const
{
	return mPosition3;
}

/*========================
拡大率の取得
返り値：拡大率(X,Y,Z)
============================*/
DirectX::SimpleMath::Vector3 ObjectModel::GetScale()const
{
	return mScale3;
}



/*========================
移動
引数：移動ベクトル
============================*/
void ObjectModel::Move(DirectX::SimpleMath::Vector3 const& vel)
{
	DirectX::SimpleMath::Vector3 vel3 = vel;

	mPosition3 += vel3;
	
	mPosition = DirectX::SimpleMath::Matrix::CreateTranslation(mPosition3);
}

/*============
描画
==============*/
void ObjectModel::Draw(DirectX::SimpleMath::Matrix* world)
{
	if (mDrawFlag)
	{
		//インスタンスの取得
		DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
		DirectXtkIF& pDI = DirectXtkIF::GetDirectXtkIFInstance();
		CameraInfoIF& pCiIF = CameraInfoIF::GetCameraInfoIFInstance();


		mView = *pCiIF.GetView();

		mProj = *pCiIF.GetProjection();

		if (world == nullptr)
		{
			//いろいろ反映
			mWorld = mScale * mRotX * mRotY * mRotZ * mPosition;
		}
		else
		{
			mWorld = *world;
		}

		ID3D11DeviceContext1* pContext = pDR->GetD3DDeviceContext();

		mModel->Draw(pContext, *pDI.GetCommonStates(), mWorld, mView, mProj);
	}
}

