/*!
@file FixedBox.cpp
@brief 足場ブロックなど
担当：逸見
*/

#include "stdafx.h"
#include "Project.h"
#include "FixedBox.h"

namespace basecross {

	FixedBox::FixedBox(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotation),
		m_scal(scale)
	{}

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetRotation(m_rot);
		ptrTransform->SetScale(m_scal);
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//OBB衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);
		//タグをつける
		AddTag(L"FixedBox");

	}

	Vec3 FixedBox::GetPositoin() {
		return m_pos;
	}
	Vec3 FixedBox::GetScale() {
		return m_scal;
	}

	BoxColl::BoxColl(const shared_ptr<Stage>& stage,
		const Vec3& position
	) :
		GameObject(stage),
		m_position(position)

	{}

	void BoxColl::OnCreate()
	{
		auto trans = AddComponent<Transform>();
		trans->SetPosition(m_position);
		trans->SetRotation(Vec3(0,0,0));
		trans->SetScale(Vec3(1,1,1));

		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		Coll->SetDrawActive(true);
		//Coll->SetUpdateActive(false);
		AddTag(L"FixedBox");
	}

	vector<Mat4x4> TilingFixedBox::m_Mat4x4;
	vector<weak_ptr<Transform>> TilingFixedBox::m_moveObject;


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<PNTStaticInstanceDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		wstring Datadir;
		App::GetApp()->GetDataDirectory(Datadir);
		CsvFile m_GameStage1;
		m_GameStage1.SetFileName(Datadir + L"Stage1.csv");
		auto& LineVec = m_GameStage1.GetCsvVec();
		m_GameStage1.ReadCsv();

		for (size_t i = 0; i < LineVec.size(); i++) {
			//トークン（カラム）の配列
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, LineVec[i], L',');
			for (size_t j = 0; j < Tokens.size(); j++) {
				if (j < 50)
				{
					if (Tokens[j] == L"1" )
					{
						float x = (float)j - 25.0f;
						float y = -(float)i + 15.0f;
						// インスタンス用の行列を作成する
						
						Mat4x4 matrix;
						matrix.translation(Vec3(x, y, 0.0f));
						m_Mat4x4.push_back(matrix);
						PtrDraw->AddMatrix(matrix); // ブロックを表示したい数だけ行列を追加します。この行列が示す位置・向き・大きさで指定したメッシュが描画されます	
						//Collsionが現在一つのみ生成、
						//このmatrixを使ってOBBの生成するか、CollsionOBBのみ生成するクラスを作るか
						auto Coll = GetStage()->AddGameObject<BoxColl>(Vec3(x, y, 0));
						//AddTag(L"FixedBox");

					}
				}
			}
		}
	}
	void TilingFixedBox::OnUpdate()
	{
		
	}
	//--------------------------------------------------------------------------------------
	//	class BreakWall : public GameObject;
	//--------------------------------------------------------------------------------------
	BreakWall::BreakWall(const shared_ptr<Stage>& stage,
		const Vec3& position,
		const Vec3& rotation,
		const Vec3& scale
	) :
		GameObject(stage),
		m_pos(position),
		m_rot(rotation),
		m_scal(scale)

	{}
	void BreakWall::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetRotation(m_rot);
		ptrTransform->SetScale(m_scal);
		//�`��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�Փ˔���
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		AddTag(L"BreakWall");


	}
	void BreakWall::OnUpdate() {

	}
	void BreakWall::ThisDestory() {
		GetStage()->RemoveGameObject<BreakWall>(GetThis<BreakWall>());
	}
}