//===============================================================================
//
//  C++使った2D(mesh_Boss_Terra.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "mesh_Boss_Terra.h"
#include "eff_explosion.h"
#include "playerX.h"
#include "player_observer.h"
#include "boss_bullet.h"
#include "enemy_base.h"
#include "particle3D.h"
#include "eff_bomb.h"

#include "mesh_ground.h"
#include "manager.h"
#include "game.h"

namespace BulletOption {
	//各弾の発射地点の位置
	int WeaponsIdx[B_MUZZLE_CUR] = {
		15,		//左腰キャノン
		16,		//右腰キャノン
		18,		//背面キャノン
		18,		//背面キャノン
		19,		//左手ライフル
		20		//右手ライフル
	};

	D3DXVECTOR3 WeaponsMuzzlePos[B_MUZZLE_CUR] = {
		{150.0f,-400.0f,0.0f},
		{-150.0f,-400.0f,0.0f},
		{ 200.0f, -400.0f,150.0f},
		{-200.0f,-400.0f,150.0f},
		{500.0f,0.0f,-50.0f},
		{-500.0f,0.0f,-50.0f}
	};

	int Life = 60;
	float Radius = 150;
	float EffectSize = 100;
	D3DXCOLOR color[2] = {
		{ 0.2f,0.2f,0.8f,1.0f },
		{ 0.8f,0.1f,0.1f,1.0f }
	};
	D3DXCOLOR DamagingColor = { 1.0f,0.1f,0.2,0.8f };
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CBossTerra::CBossTerra():m_bMove(false), m_nLife(400), m_bDead(false), m_bDamaging(false), m_nDamageFrame(0),m_nDeadFrame(0),m_nAttackFrame(0), m_move({0.0f,0.0f,0.0f}), m_bBlend(false), m_bMotion(false), m_bTransformed(false)
{
	m_Reticle[0] = nullptr;
	m_Reticle[1] = nullptr;
	for (int i = 0; i < B_MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i] = nullptr;
	}
	for (int j = 0; j < B_MUZZLE_CUR; ++j)
	{
		m_mtxWeapon[j] = {};
	}
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CBossTerra::~CBossTerra()
{
}

//==========================================================================================
//初期化処理
//==========================================================================================
void CBossTerra::Init()
{
	MotionInit();

	CObject::SetType(TYPE_3D_BOSSTERRA);

	m_Gauge = CGaugeBoss::Create(m_pos, { 750,50 }, m_nLife);
	m_pShadow = CShadow::Create(m_pos,270);
}

//==========================================================================================
//終了処理
//==========================================================================================
void CBossTerra::Uninit()
{
	if (m_Reticle[0] != nullptr)
	{
		m_Reticle[0]->Release();
		m_Reticle[0] = nullptr;
	}
	if (m_Reticle[1] != nullptr)
	{
		m_Reticle[1]->Release();
		m_Reticle[1] = nullptr;
	}
	if (m_Gauge != nullptr)
	{
		m_Gauge->Uninit();
	}
	for (int i = 0; i < B_MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Uninit();
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CBossTerra::Update()
{
	D3DXVECTOR3 Playerpos = CPlayerObserver::GetInstance()->GetPlayerPos();
	m_pShadow->SetShadowGround(m_pos);

	if (!MotionBlending())
	{
		SetNextKey();
	}
	if (m_pos.x > 500.0f ||
		m_pos.x < -500.0f)
	{
		m_bMove = !m_bMove;
	}
	if (!m_bDead)
	{
		if (m_bMove)
		{
			m_pos += { 5.0f,0.0f,0.0f };
		}
		else
		{
			m_pos += { -5.0f,0.0f,0.0f };
		}
		Attack(Playerpos);
	}
	
	if (m_bDamaging)
	{
		++m_nDamageFrame;
		if (m_nDamageFrame > 20)
		{
			m_bDamaging = false;
		}
	}
	if (!m_bTransformed)
	{
		m_Gauge->SetPos({ m_pos.x,m_pos.y + 700.0f ,m_pos.z });
	}
	else
	{
		m_Gauge->SetPos({ m_pos.x,m_pos.y + 1100.0f ,m_pos.z });
		m_rot.y = atan2f((m_pos.x - Playerpos.x), (m_pos.z - Playerpos.z));
	}
	DeathCheck();
	if(m_bDead)
	{
		DeathAnim();
	}


	m_move.z = CPlayerObserver::GetInstance()->GetPlayerMove().z * 1.15f;
	float disPos = m_pos.z - Playerpos.z;
	if (disPos < 1500)
	{
		m_move.z += 2500;
	}

	m_pos += m_move;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.17f;
	m_move.y += (0.0f - m_move.y) * 0.17f;
	m_move.z += (0.0f - m_move.z) * 0.17f;
}


//==========================================================================================
//描画処理
//==========================================================================================
void CBossTerra::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxSize);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	for (int i = 0; i < B_MAX_MODELPARTS; ++i)
	{
		if (m_bDamaging || m_bDead)
		{
			m_apModelParts[i]->Draw(BulletOption::DamagingColor);
		}
		else
		{
			m_apModelParts[i]->Draw();
		}
	}
}


//==========================================================================================
//生成処理
//==========================================================================================
CBossTerra* CBossTerra::Create(D3DXVECTOR3 pos)
{
	CBossTerra* enemy = new CBossTerra;

	enemy->Init();

	enemy->m_pos = pos;
	enemy->m_move = { 0.0f,0.0f,0.0f };
	enemy->m_rot = { 0.0f,0.0f,0.0f };
	enemy->m_size = { 1.0f,1.0f,1.0f };

	return enemy;
}

//==========================================================================================
//死亡演出処理
//==========================================================================================
void CBossTerra::DeathAnim() {
	if (m_nDeadFrame % 15 == 0)
	{
		if (m_nDeadFrame == 180)
		{
			m_Gauge->Release();
			m_Gauge = nullptr;
			if (m_pShadow != nullptr)
			{
				m_pShadow->Release();
				m_pShadow = nullptr;
			}
			CObject::Release();
			CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_RESULT);
			return;
		}
		else
		{
			//乱数生成
			std::random_device rnd;				// 非決定的な乱数生成器でシード生成機を生成
			std::mt19937 mt(rnd());				//  メルセンヌツイスターの32ビット版、引数は初期シード
			std::uniform_int_distribution<> rand_x(-800, 800);    // [-1200, 1200] 範囲の一様乱数
			std::uniform_int_distribution<> rand_z(-600, 600);		// [-700, 700] 範囲の一様乱数
			std::uniform_int_distribution<> rand_y(-400, 700);		// [-400, 700] 範囲の一様乱数
			std::uniform_int_distribution<> rand_Radius(200, 450);	// [100, 250] 範囲の一様乱数
			D3DXVECTOR3 pos = {
							m_pos.x + (float)(rand_x(mt)),
							m_pos.y + (float)(rand_y(mt)),
							m_pos.z + (float)(rand_z(mt))
			};
			CEffExplosion::Create(pos, rand_Radius(mt));
		}
	}
	m_Bomb->SetPos(m_pos);
	++m_nDeadFrame;
	m_pos += {0.0f, -2.0f, 0.0f};
	
}

//==========================================================================================
//攻撃のレート管理処理
//==========================================================================================
bool CBossTerra::AttackRateCheck()
{
	if (m_nAttackFrame >= 180) {
		m_nAttackFrame = 0;
		return true;
	}
	++m_nAttackFrame;
	return false;
}

//==========================================================================================
//攻撃の管理
//==========================================================================================
void CBossTerra::Attack(D3DXVECTOR3& Playerpos)
{
	if (AttackRateCheck())
	{
		if (!m_bTransformed)
		{
			std::random_device rnd;				// 非決定的な乱数生成器でシード生成機を生成
			std::mt19937 mt(rnd());				//  メルセンヌツイスターの32ビット版、引数は初期シード
			std::uniform_int_distribution<> rand_num(0, 2);     // [0, 1] 範囲の一様乱数
			switch (rand_num(mt))
			{
			case 0:
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BOSSLOCKON);

				m_Reticle[0] = CBossReticle::Create(Playerpos, 150, 50, 0.08f);
				m_Reticle[1] = CBossReticle::Create(Playerpos, 100, 50, -0.06f);
				break;
			case 1:
				SetStatue();
				break;
			case 2:
				for (int i = 0; i < 5; ++i)
				{
					CBossEnemySpawner::Create(m_pos, i);
				}
				break;
			}
		}
		else
		{
			std::random_device rnd;				// 非決定的な乱数生成器でシード生成機を生成
			std::mt19937 mt(rnd());				//  メルセンヌツイスターの32ビット版、引数は初期シード
			std::uniform_int_distribution<> rand_num(0, 2);     // [0, 1] 範囲の一様乱数
			switch (rand_num(mt))
			{
			case 0:
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BOSSLOCKON);
				SetNextMotion(MOTION_ROBO_BURST);
				m_Reticle[0] = CBossReticle::Create(Playerpos, 120, 80, 0.08f);
				m_Reticle[1] = CBossReticle::Create(Playerpos, 90, 80, -0.06f);
				break;
			case 1:
				SetNextMotion(MOTION_ROBO_SHOOT);
				break;
			case 2:
				SetNextMotion(MOTION_ROBO_FUNNEL);
				for (int i = 0; i < 5; ++i)
				{
					CBossFunnel::Create(m_pos, i);
				}
				break;
			}
		}
	}


	if (m_Reticle[0] != nullptr &&
		m_Reticle[1] != nullptr)
	{
		m_Reticle[0]->SetPos(Playerpos);
		m_Reticle[1]->SetPos(Playerpos);
		if (m_Reticle[0]->GetLifeState() ||
			m_Reticle[1]->GetLifeState())
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BOSSSHOT);
			SetBullet(m_pos, Playerpos);

			m_Reticle[0]->Release();
			m_Reticle[1]->Release();
			m_Reticle[0] = nullptr;
			m_Reticle[1] = nullptr;
		}
	}
}

void CBossTerra::DeathCheck() 
{
	if (m_nLife <= 0 && !m_bDead)
	{
		if (!m_bTransformed)
		{
			if (m_Gauge != nullptr)
			{
				m_Gauge->Release();
				m_Gauge = nullptr;
			}
			SetNextMotion(MOTION_TRANSFORM);
			m_nLife = 400;
			m_Gauge = CGaugeBoss::Create(m_pos, { 750,50 }, m_nLife);
			m_bTransformed = true;
			return;
		}
		m_pos += { 0.4f,0.0f,0.6f };
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BIGBOMB);
		m_Bomb = CBossBomb::Create(m_pos);
		m_bDead = true;
		SetNextMotion(MOTION_ROBO_DIE);

		CScene* pScene = CManager::GetInstance()->GetScene();
		if (pScene->GetSceneMode() == CScene::MODE_GAME)
		{
			CGame* pGame = dynamic_cast<CGame*>(pScene);
			pGame->GetScore()->AddScore(5000);
		}
	}
};

//==========================================================================================
//すべての武器の銃口オフセットを設定
//==========================================================================================
void CBossTerra::SetWeaponMtx()
{
	using namespace BulletOption;
	for (int i = 0; i < B_MUZZLE_CUR; ++i)
	{
		int T = WeaponsIdx[i];
		D3DXMATRIX RifleMtx = m_apModelParts[T]->GetWorldMatrix();
		D3DXMATRIX UseMtx;
		D3DXVECTOR3 Addpos = WeaponsMuzzlePos[i];
		D3DXVECTOR3 Addrot = { 0.0f,0.0f,0.0f };

		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&UseMtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			Addrot.y,
			Addrot.x,
			Addrot.z);
		D3DXMatrixMultiply(&UseMtx,
			&UseMtx,
			&mtxRot);
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			Addpos.x,
			Addpos.y,
			Addpos.z);
		D3DXMatrixMultiply(&UseMtx,
			&UseMtx,
			&mtxTrans);

		//ワールド行列を親の行列に掛ける
		D3DXMatrixMultiply(&UseMtx,
			&UseMtx,
			&RifleMtx);

		m_mtxWeapon[i] = UseMtx;
	}
}

//==========================================================================================
//弾の発射を設定
//==========================================================================================
void CBossTerra::SetBullet(D3DXVECTOR3& pos, D3DXVECTOR3& Playerpos)
{
	using namespace BulletOption;
	D3DXVECTOR3 SetMtxpos[B_MUZZLE_CUR];
	D3DXVECTOR3 value[B_MUZZLE_CUR];
	SetWeaponMtx();

	for (int i = 0; i < B_MUZZLE_CUR; ++i)
	{
		SetMtxpos[i] = { m_mtxWeapon[i]._41, m_mtxWeapon[i]._42, m_mtxWeapon[i]._43 };
		value[i] = Playerpos - SetMtxpos[i];
		D3DXVec3Normalize(&value[i], &value[i]);
		CBossBullet::Create(SetMtxpos[i], value[i], color[i % 2], Life, Radius, EffectSize);
	}
	/*
	D3DXVECTOR3 pos1 = setpos1 + pos;
	D3DXVECTOR3 pos2 = setpos2 + pos;
	D3DXVECTOR3 pos3 = setpos3 + pos;
	D3DXVECTOR3 pos4 = setpos4 + pos;
	D3DXVECTOR3 pos5 = setpos5 + pos;
	D3DXVECTOR3 pos6 = setpos6 + pos;

	D3DXVECTOR3 value1 = Playerpos - pos1;
	D3DXVECTOR3 value2 = Playerpos - pos2;
	D3DXVECTOR3 value3 = Playerpos - pos3;
	D3DXVECTOR3 value4 = Playerpos - pos4;
	D3DXVECTOR3 value5 = Playerpos - pos5;
	D3DXVECTOR3 value6 = Playerpos - pos6;

	D3DXVec3Normalize(&value1, &value1);
	D3DXVec3Normalize(&value2, &value2);
	D3DXVec3Normalize(&value3, &value3);
	D3DXVec3Normalize(&value4, &value4);
	D3DXVec3Normalize(&value5, &value5);
	D3DXVec3Normalize(&value6, &value6);

	CBossBullet::Create(pos2, value2, color2, Life, Radius, EffectSize);
	CBossBullet::Create(pos3, value3, color1, Life, Radius, EffectSize);
	CBossBullet::Create(pos4, value4, color2, Life, Radius, EffectSize);
	CBossBullet::Create(pos5, value5, color1, Life, Radius, EffectSize);
	CBossBullet::Create(pos6, value6, color2, Life, Radius, EffectSize);
	*/
}

//==========================================================================================
//ボスが出す石柱の出現
//==========================================================================================
void CBossTerra::SetStatue()
{
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_SPAWN);

	std::random_device rnd;			// 非決定的な乱数生成器でシード生成機を生成
	std::mt19937 mt(rnd());			//  メルセンヌツイスターの32ビット版、引数は初期シード
	std::uniform_int_distribution<> rand_x(-2500, 2500);	 // [-3000, 3000] 範囲の一様乱数
	std::uniform_int_distribution<> rand_y(2000, 5000);		 // [2000, 5000] 範囲の一様乱数

	for (int i = 0; i < 4; ++i)
	{
		CBossStatue::Create(
			{ (float)(rand_x(mt)),
			(float)(rand_y(mt)),
			m_pos.z + (i + 1) * 2500 }
		);
	}
}

//==========================================================================================
//ボスが出す石柱の生成処理
//==========================================================================================
CBossStatue* CBossStatue::Create(D3DXVECTOR3 pos)
{
	CBossStatue* statue = new CBossStatue;

	statue->BindModel("data\\MODEL\\boss_statue.x");
	statue->SetModelParam(pos);
	statue->Init();
	statue->AddPos({ 0.0f,3000.0f,0.0f });
	return statue;
}

//==========================================================================================
//空中に出した石柱を地面に突き刺す
//==========================================================================================
void CBossStatue::SetYPos()
{
	CBossStatue::AddPos({ 0.0f,-250.0f,0.0f });
	// 地形判定
	BOOL  bIsHit = false;
	float fLandDistance;
	DWORD dwHitIndex = 0U;
	float fHitU;
	float fHitV;
	LPD3DXMESH pMesh = nullptr;
	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_MESHOBJECT) {
					CMeshGround* pTest = dynamic_cast<CMeshGround*>(pObj);
					if (pTest != nullptr) {
						pMesh = pTest->GetMesh();
						if (pTest != nullptr) {
							// 地形判定
							LPD3DXMESH pMesh = nullptr;

							pMesh = pTest->GetMesh();
							D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							D3DXVECTOR3 objpos = CBossStatue::GetPos() - pTest->GetPos();
							D3DXIntersect(pMesh, &objpos, &dir, &bIsHit, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, nullptr, nullptr);

							// ----- 接地時処理 -----
							if (bIsHit)
							{
								if (fLandDistance > 100)
								{
									m_bSeted = true;
									CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_FALL);
								}
							}
						}
					}
				}
			}
		}
	}
}

//
//==========================================================================================================================
//敵を放出するクラスの処理群

//==========================================================================================
//ボスが出す敵の生成処理
//==========================================================================================
CBossEnemySpawner* CBossEnemySpawner::Create(D3DXVECTOR3 pos,int num)
{
	CBossEnemySpawner* statue = new CBossEnemySpawner;

	statue->BindModel("data\\MODEL\\enemy_base.x");
	statue->SetModelParam(pos);
	statue->Init(num);
	return statue;
}

//==========================================================================================
//初期化処理
//==========================================================================================
void CBossEnemySpawner::Init(int num)
{
	//乱数生成
	std::random_device rnd;			// 非決定的な乱数生成器でシード生成機を生成
	std::mt19937 mt(rnd());			//  メルセンヌツイスターの32ビット版、引数は初期シード
	std::uniform_int_distribution<> rand_x(-1200, 1200);	// [-1200, 1200] 範囲の一様乱数
	std::uniform_int_distribution<> rand_y(-1000, 1000);	// [-1000, 1000] 範囲の一様乱数
	std::uniform_int_distribution<> rand_z(1500, 2500);		// [1000, 2000] 範囲の一様乱数
	std::uniform_int_distribution<> rand_time(30, 60);		// [30, 60] 範囲の一様乱数

	m_Lifetime = rand_time(mt);

	m_moveVec = {
		(float)(rand_x(mt) / m_Lifetime),
		(float)(rand_y(mt) / m_Lifetime),
		(float)((rand_z(mt) * (num + 1)) / m_Lifetime)
	};

	CObjectX::Init();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CBossEnemySpawner::Update()
{
	if (m_CntTime >= m_Lifetime)
	{
		SetEnemy();
	}
	else
	{
		++m_CntTime;
		Move();
	}
	CObjectX::Update();
}

//==========================================================================================
//動かす処理
//==========================================================================================
void CBossEnemySpawner::Move()
{
	D3DXVECTOR3 pos = CObjectX::GetPos();

	pos += m_moveVec;

	if (pos.x >= WORLD_WALL_X)
	{
		pos.x = WORLD_WALL_X;
	}
	else if (pos.x <= -WORLD_WALL_X)
	{
		pos.x = -WORLD_WALL_X;
	}

	if (pos.y >= WORLD_WALL_Y)
	{
		pos.y = WORLD_WALL_Y;
	}
	else if (pos.y <= -WORLD_WALL_Y)
	{
		pos.y = -WORLD_WALL_Y;
	}
	CParticle3D::Create(pos, {0.2f,0.0f,1.0f,0.7f}, 90, 120, true);

	CObjectX::SetPos(pos);
}

//==========================================================================================
//本物の敵に置き換える処理
//==========================================================================================
void CBossEnemySpawner::SetEnemy()
{
	CObjectX::SetSize({ 0.8f,0.8f ,0.8f });

	CEnemyBase::Create(CObjectX::GetPos());
	CObject::Release();
	return;
}

//==========================================================================================
//ボスが出す敵の生成処理
//==========================================================================================
CBossFunnel* CBossFunnel::Create(D3DXVECTOR3 pos, int num)
{
	CBossFunnel* statue = new CBossFunnel;

	statue->BindModel("data\\MODEL\\Boss_Funnel.x");
	statue->SetModelParam(pos);
	statue->Init(num);
	return statue;
}

//==========================================================================================================================

//==========================================================================================
//敵投げナイフの初期化
//==========================================================================================
void CBossKnife::Init()
{
	m_Sec = 0;
	m_vecAxis = { 1.0f,0.0f,0.0f };
	m_RotValue = -B_KNIFE_ROTSPEED;
	D3DXVec3Normalize(&m_vecAxis, &m_vecAxis);
	
	CObject::SetType(TYPE_3D_BOSSWEAPONS);
	
	lockon = CLockonEnemy::Create(CObjectX::GetPos());
	
	CObjectX::Init();
}

//==========================================================================================
//敵投げナイフの更新
//==========================================================================================
void CBossKnife::Update()
{
	//回転量
	float m_fValueRot  = m_Sec * m_RotValue;	

	//クオータニオンを作成
	D3DXQuaternionRotationAxis(
		&m_quat,
		&m_vecAxis,
		m_fValueRot);

	//クオータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(
		&m_mtxRot,
		&m_quat);

	D3DXVECTOR3 Playerpos = CPlayerObserver::GetInstance()->GetPlayerPos();

	m_Target1 = { m_StartPos.x - 2300,m_StartPos.y - 1700,m_StartPos.z - 320 };
	m_Target2 = { Playerpos.x - 2300,Playerpos.y - 1700,Playerpos.z + 280 };
	D3DXVECTOR3 DigitPos = {0.0f,0.0f,0.0f};
	float rate = (float)((1.0 / m_Reach) * m_Sec);
	
	D3DXVec3CatmullRom(&DigitPos, &m_Target1,& m_StartPos, &Playerpos, &m_Target2,rate);

	CObjectX::SetPos(DigitPos);

	++m_Sec;
	lockon->SetThisPos(CObjectX::GetPos());

	if (m_Reach < m_Sec)
	{
		if (lockon != nullptr)
		{
			lockon->Release();
			lockon = nullptr;
		}
		CObject::Release();
		return;
	}
	CObjectX::Update();
}

void CBossKnife::Braking()
{
	CEffExplosion::Create(CObjectX::GetPos(),240.0f);
	CObject::Release();
	return;
}

//==========================================================================================
//敵投げナイフの生成
//==========================================================================================
CBossKnife* CBossKnife::Create(D3DXVECTOR3 startPos, int Reach, bool Side)
{
	CBossKnife* knife = new CBossKnife();

	knife->BindModel("data\\MODEL\\Boss_Knife.x");
	knife->SetModelParam(startPos);
	knife->m_StartPos = startPos;
	knife->m_Reach = Reach;
	if (Side)
	{
		knife->AddRot({ 0.0f,0.0f,D3DX_PI * 0.25 });
	}
	else
	{
		knife->AddRot({ 0.0f,0.0f,-D3DX_PI * 0.25 });
	}
	knife->Init();
	return knife;
}

//==========================================================================================
//敵投げナイフの初期化
//==========================================================================================
void CBossBomb::Init()
{
	CObject::SetType(TYPE_BILLBOARD);

	CObjectX::Init();
}

//==========================================================================================
//敵投げナイフの更新
//==========================================================================================
void CBossBomb::Update()
{
	CObjectX::AddRot({ 0.0f,0.2f,0.0f });
	CObjectX::AddSize({ 0.8f,0.8f,0.8f });
	CObjectX::Update();
}

//==========================================================================================
//敵投げナイフの更新
//==========================================================================================
void CBossBomb::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 法線の自動正規化を有効に
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングを両面に

	CObjectX::Draw();

	// 法線の自動正規化を無効に
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//カリングをもどすに
}


//==========================================================================================
//敵投げナイフの生成
//==========================================================================================
CBossBomb* CBossBomb::Create(D3DXVECTOR3 pos)
{
	CBossBomb* knife = new CBossBomb();

	knife->BindModel("data\\MODEL\\Bomb.x");
	knife->SetModelParam(pos);
	knife->Init();
	return knife;
}


//======================================================================================================================================
//モーション関連処理

//==========================================================================================
//モーションの初期化処理
//==========================================================================================
void CBossTerra::MotionInit()
{
	MotionDataLoad();

	m_CurMotion = 0;
	m_CurKey = 0;
	m_NowFrame = 0;
}

//==========================================================================================
//次のモーションに移行する処理
//==========================================================================================
void CBossTerra::SetNextMotion(int nNextMotionNum)
{
	m_NextMotion = nNextMotionNum;
	m_NowFrame = 0;
	m_bBlend = true;
	MotionBlending();
}

//==========================================================================================
//次のキーのモーション処理
//==========================================================================================
void CBossTerra::SetNextKey()
{
	//現在の位置・角度
	D3DXVECTOR3 NowPos = {};
	D3DXVECTOR3 NowRot = {};

	//次の位置・角度
	D3DXVECTOR3 NextPos = {};
	D3DXVECTOR3 NextRot = {};

	//動きの差分を求める用
	D3DXVECTOR3 DifPos = {};
	D3DXVECTOR3 DifRot = {};

	//計算用
	D3DXVECTOR3 DigitPos = {};
	D3DXVECTOR3 DigitRot = {};

	int nNowKey = m_CurKey;
	int nNowMotion = m_CurMotion;
	int nNextKey = (m_CurKey + 1) % m_aMotion[nNowMotion].nKeyNum;
	float fRatioFrame = (float)m_NowFrame / (float)m_aMotion[nNowMotion].aKetSet[nNowKey].nFrame;

	for (int nCntParts = 0; nCntParts < MAX_PARTS; ++nCntParts)
	{
		//現在の向きと位置の情報
		NowPos = m_aMotion[nNowMotion].aKetSet[nNowKey].aKey[nCntParts].pos;
		NowRot = m_aMotion[nNowMotion].aKetSet[nNowKey].aKey[nCntParts].rot;

		//次のキーの情報
		NextPos = m_aMotion[nNowMotion].aKetSet[nNextKey].aKey[nCntParts].pos;
		NextRot = m_aMotion[nNowMotion].aKetSet[nNextKey].aKey[nCntParts].rot;

		//差分を求める
		DifPos = NextPos - NowPos;
		DifRot = NextRot - NowRot;

		//-3.14～3.14の間を超える場合の修正
		if (DifRot.x >= D3DX_PI)
		{
			DifRot.x -= D3DX_PI * 2;
		}
		if (DifRot.y >= D3DX_PI)
		{
			DifRot.y -= D3DX_PI * 2;
		}
		if (DifRot.z >= D3DX_PI)
		{
			DifRot.z -= D3DX_PI * 2;
		}
		if (DifRot.x <= -D3DX_PI)
		{
			DifRot.x += D3DX_PI * 2;
		}
		if (DifRot.y <= -D3DX_PI)
		{
			DifRot.y += D3DX_PI * 2;
		}
		if (DifRot.z <= -D3DX_PI)
		{
			DifRot.z += D3DX_PI * 2;
		}

		DigitPos = DifPos * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultPos() + NowPos;
		DigitRot = DifRot * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultRot() + NowRot;

		m_apModelParts[nCntParts]->SetPos(DigitPos);
		m_apModelParts[nCntParts]->SetRot(DigitRot);
	}

	++m_NowFrame;

	if (m_NowFrame >= m_aMotion[nNowMotion].aKetSet[nNowKey].nFrame)
	{
		++m_CurKey;
		m_NowFrame = 0;
		if (m_CurKey >= m_aMotion[nNowMotion].nKeyNum)
		{
			if (!m_aMotion[nNowMotion].bLoop)
			{
				if (m_CurMotion == MOTION_TRANSFORM ||
					m_CurMotion == MOTION_ROBO_BURST ||
					m_CurMotion == MOTION_ROBO_FUNNEL ||
					m_CurMotion == MOTION_ROBO_SHOOT )
				{
					if (m_CurMotion == MOTION_ROBO_SHOOT)
					{
						D3DXVECTOR3 Handpos = {
							m_apModelParts[5]->GetWorldMatrix()._41,
							m_apModelParts[5]->GetWorldMatrix()._42,
							m_apModelParts[5]->GetWorldMatrix()._43,
						};
						CBossKnife::Create(Handpos, 120, false);
					}
					else if (MOTION_ROBO_SHOOT)
					{
						SetWeaponMtx();
						D3DXVECTOR3 Weaponpos = {
							m_mtxWeapon[5]._41,
							m_mtxWeapon[5]._42,
							m_mtxWeapon[5]._43,
						};
						CBossBullet::Create(Weaponpos, {0.0f,0.0f,-1.0f}, BulletOption::color[0], BulletOption::Life, BulletOption::Radius, BulletOption::EffectSize);

					}
					else if (MOTION_ROBO_SHOT1)
					{
						SetWeaponMtx();
						D3DXVECTOR3 Weaponpos = {
							m_mtxWeapon[5]._41,
							m_mtxWeapon[5]._42,
							m_mtxWeapon[5]._43,
						};
						CBossBullet::Create(Weaponpos, { 0.0f,0.0f,-1.0f }, BulletOption::color[0], BulletOption::Life, BulletOption::Radius, BulletOption::EffectSize);
						--m_CurKey;
						m_bMotion = false;
						SetNextMotion(MOTION_ROBO_SHOT2);
						return;
					}
					else if (MOTION_ROBO_SHOT2)
					{
						SetWeaponMtx();
						D3DXVECTOR3 Weaponpos1 = {
							m_mtxWeapon[4]._41,
							m_mtxWeapon[4]._42,
							m_mtxWeapon[4]._43,
						};
						D3DXVECTOR3 Weaponpos2 = {
							m_mtxWeapon[5]._41,
							m_mtxWeapon[5]._42,
							m_mtxWeapon[5]._43,
						};
						CBossBullet::Create(Weaponpos1, { 0.0f,0.0f,-1.0f }, BulletOption::color[1], BulletOption::Life, BulletOption::Radius, BulletOption::EffectSize);
						CBossBullet::Create(Weaponpos2, { 0.0f,0.0f,-1.0f }, BulletOption::color[0], BulletOption::Life, BulletOption::Radius, BulletOption::EffectSize);

					}
					--m_CurKey;
					m_bMotion = false;
					SetNextMotion(MOTION_ROBO_NUTO);

				}
				else if (m_CurMotion == MOTION_ROBO_DIE)
				{
					--m_CurKey;
					m_bMotion = false;
				}
			}
			else
			{
				m_CurKey = 0;
			}
		}
	}
}
//==========================================================================================
//モーション切り替え時のブレンド処理
//==========================================================================================
bool CBossTerra::MotionBlending()
{
	if (!m_bBlend) {
		return false;
	}

	//現在の位置・角度
	D3DXVECTOR3 NowPos = {};
	D3DXVECTOR3 NowRot = {};

	//次の位置・角度
	D3DXVECTOR3 NextPos = {};
	D3DXVECTOR3 NextRot = {};

	//動きの差分を求める用
	D3DXVECTOR3 DifPos = {};
	D3DXVECTOR3 DifRot = {};

	//計算用
	D3DXVECTOR3 DigitPos = {};
	D3DXVECTOR3 DigitRot = {};

	int nLastKey = m_CurKey;
	int nNowMotion = m_CurMotion;

	float fRatioFrame = ((float)m_NowFrame / (float)m_aMotion[m_NextMotion].aKetSet[0].nFrame);

	for (int nCntParts = 0; nCntParts < MAX_PARTS; ++nCntParts)
	{
		//現在の向きと位置の情報
		NowPos = m_aMotion[nNowMotion].aKetSet[nLastKey].aKey[nCntParts].pos;
		NowRot = m_aMotion[nNowMotion].aKetSet[nLastKey].aKey[nCntParts].rot;

		//次のキーの情報
		NextPos = m_aMotion[m_NextMotion].aKetSet[0].aKey[nCntParts].pos;
		NextRot = m_aMotion[m_NextMotion].aKetSet[0].aKey[nCntParts].rot;

		//差分を求める
		DifPos = NextPos - NowPos;
		DifRot = NextRot - NowRot;

		//-3.14～3.14の間を超える場合の修正
		if (DifRot.x >= D3DX_PI)
		{
			DifRot.x -= D3DX_PI * 2;
		}
		if (DifRot.y >= D3DX_PI)
		{
			DifRot.y -= D3DX_PI * 2;
		}
		if (DifRot.z >= D3DX_PI)
		{
			DifRot.z -= D3DX_PI * 2;
		}
		if (DifRot.x <= -D3DX_PI)
		{
			DifRot.x += D3DX_PI * 2;
		}
		if (DifRot.y <= -D3DX_PI)
		{
			DifRot.y += D3DX_PI * 2;
		}
		if (DifRot.z <= -D3DX_PI)
		{
			DifRot.z += D3DX_PI * 2;
		}

		DigitPos = DifPos * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultPos() + NowPos;
		DigitRot = DifRot * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultRot() + NowRot;

		m_apModelParts[nCntParts]->SetPos(DigitPos);
		m_apModelParts[nCntParts]->SetRot(DigitRot);
	}

	++m_NowFrame;

	if (m_NowFrame >= m_aMotion[m_NextMotion].aKetSet[0].nFrame)
	{
		m_CurKey = 0;
		m_NowFrame = 0;
		m_CurMotion = m_NextMotion;
		m_bBlend = false;
		SetNextKey();
	}

	return m_bBlend;
}

//==========================================================================================
//モーションをファイルから読み込み
//==========================================================================================
void CBossTerra::MotionDataLoad()
{
	char LoadData[128];
	char ModelPath[128];

	int nNumModel = 0;
	FILE* pFile;
	int nCnt = 0;

	int nMotionCnt = 0;
	int nKeySet = 0;
	int nKey = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	int nFilenameCnt = 0;
	int nParent = 0;
	int nIndex = 0;
	int nModelCnt = 0;

	pFile = fopen("data\\TEXT\\motion_Boss_Terra.txt", "r");

	if (pFile != nullptr)
	{
		while (1)
		{
			fscanf(pFile, "%s", LoadData);

			if (!strcmp(LoadData, "END_SCRIPT"))	// ファイルの最後
			{
				fclose(pFile);
				break;
			}

			if (LoadData[0] == '#')		// 文字飛ばし
			{
				continue;
			}

			//モデルの読み込み
			if (!strcmp(LoadData, "NUM_MODEL"))
			{
				fscanf(pFile, "%s", LoadData);
				fscanf(pFile, "%d", &m_ModelParts);
			}
			//モデルのファイル名読み込み
			if (!strcmp(LoadData, "MODEL_FILENAME"))
			{
				fscanf(pFile, "%s", LoadData);

				fscanf(pFile, "%s", ModelPath);

				m_pModelFileName[nFilenameCnt] = ModelPath;

				m_apModelParts[nFilenameCnt] = CModelParts::Create(pos, m_pModelFileName[nFilenameCnt]);

				++nFilenameCnt;
			}
			//キャラクターの設定の読み込み開始
			if (!strcmp(LoadData, "CHARACTERSET"))
			{
				while (1)
				{
					fscanf(pFile, "%s", LoadData);

					if (!strcmp(LoadData, "END_CHARACTERSET"))//読み込みを終了
					{
						break;
					}
					else if (!strcmp(LoadData, "PARTSSET"))
					{
						while (1)
						{
							fscanf(pFile, "%s", LoadData);

							if (LoadData[0] == '#')
							{//文字飛ばし
								fgets(LoadData, 100, pFile);
								continue;
							}

							if (!strcmp(LoadData, "END_PARTSSET"))
							{
								//読み込みを終了
								break;
							}
							//各パーツのモーションpos値
							else if (!strcmp(LoadData, "INDEX"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nIndex);
							}

							//各パーツのモーションrot値
							else if (!strcmp(LoadData, "PARENT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nParent);
							}
							//各パーツのモーションpos値
							else if (!strcmp(LoadData, "POS"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}

							//各パーツのモーションrot値
							else if (!strcmp(LoadData, "ROT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
						}
						m_apModelParts[nModelCnt]->SetPos(pos);
						m_apModelParts[nModelCnt]->SetRot(rot);

						m_apModelParts[nModelCnt]->SetDefault();
						m_apModelParts[nModelCnt]->SetIndex(nIndex);
						if (nParent != -1)
						{
							m_apModelParts[nModelCnt]->SetParent(m_apModelParts[nParent]);
						}
						++nModelCnt;
					}
				}
			}

			//モーションの読み込み開始
			if (!strcmp(LoadData, "MOTIONSET"))
			{
				while (1)
				{
					fscanf(pFile, "%s", LoadData);

					if (!strcmp(LoadData, "END_MOTIONSET"))//読み込みを終了
					{
						break;
					}
					//ループの判断
					else if (!strcmp(LoadData, "LOOP"))
					{
						fscanf(pFile, "%s", LoadData);
						fscanf(pFile, "%d", &m_aMotion[nMotionCnt].bLoop);
					}

					//全体のキー数の読み込み
					else  if (!strcmp(LoadData, "NUM_KEY"))
					{
						fscanf(pFile, "%s", LoadData);
						fscanf(pFile, "%d", &m_aMotion[nMotionCnt].nKeyNum);
					}

					//各キーを読み込み
					if (!strcmp(LoadData, "KEYSET"))
					{
						while (1)
						{
							fscanf(pFile, "%s", LoadData);

							if (LoadData[0] == '#')
							{//文字飛ばし
								fgets(LoadData, 100, pFile);
								continue;
							}

							if (!strcmp(LoadData, "END_KEYSET"))
							{
								//読み込みを終了
								break;
							}

							//現在のキーのフレーム数を読み込み
							else if (!strcmp(LoadData, "FRAME"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &m_aMotion[nMotionCnt].aKetSet[nKeySet].nFrame);
							}

							//現在のキーの読み込み
							if (!strcmp(LoadData, "KEY"))
							{
								while (1)
								{
									fscanf(pFile, "%s", LoadData);

									if (!strcmp(LoadData, "END_KEY"))
									{
										// 読み込みを終了
										break;
									}

									//各パーツのモーションpos値
									else if (!strcmp(LoadData, "POS"))
									{
										fscanf(pFile, "%s", LoadData);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.x);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.y);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.z);
									}

									//各パーツのモーションrot値
									else if (!strcmp(LoadData, "ROT"))
									{
										fscanf(pFile, "%s", LoadData);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.x);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.y);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.z);
									}
								}
								//キー番号を進める
								++nKey;
							}
						}
						//キー番号を初期化、キーセット番号を進める
						nKey = 0;
						++nKeySet;
					}
				}
				//キーセット番号を初期化、モーション番号を進める
				nKeySet = 0;
				++nMotionCnt;
			}
		}
	}
	else
	{
		assert(pFile == nullptr);
	}
}