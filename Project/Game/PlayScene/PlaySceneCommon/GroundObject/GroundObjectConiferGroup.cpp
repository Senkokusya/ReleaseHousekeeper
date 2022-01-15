/*
針葉樹のモデルを管理するクラス
作成日：2021/08/19
*/

#include"pch.h"

#include "GroundObjectConiferGroup.h"

//モデルの拡大率
const DirectX::SimpleMath::Vector3 GroundObjectConiferGroup::CONIFER_SCALE(2.f,2.f,2.f);
//針葉樹グループモデルのY座標
const float GroundObjectConiferGroup::CONIFER_MODEL_POSITION_Y = 1.f;


/*======================
コンストラクタ
======================*/
GroundObjectConiferGroup::GroundObjectConiferGroup()
	:
	IGroundObject(),
	mpConiferModel(nullptr)
{
	mpConiferModel = std::make_unique<ObjectModel>();
}

/*======================
デストラクタ
======================*/
GroundObjectConiferGroup::~GroundObjectConiferGroup()
{

}

/*======================
初期化
======================*/
void GroundObjectConiferGroup::Initialize()
{
	//リソースマネージャの準備
	auto pRM = ResourceManager::GetInstance();
	auto model = pRM->GetCmoModel("ConiferGroup");

	//モデルの設定
	mpConiferModel->SetModel(model);
	//拡大率の設定
	mpConiferModel->SetScale(CONIFER_SCALE);
}

/*======================
描画処理
======================*/
void GroundObjectConiferGroup::Draw()
{
	mpConiferModel->Draw();
}

/*====================================
モデルの座標を設定する
引数：モデルの座標
====================================*/
void GroundObjectConiferGroup::SetConiferModelPosition(DirectX::SimpleMath::Vector3 const& pos)
{
	mpConiferModel->SetPosition(pos);
}
