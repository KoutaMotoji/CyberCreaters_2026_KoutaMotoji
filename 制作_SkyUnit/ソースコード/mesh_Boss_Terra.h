//===============================================================================
//
//  ステージボス処理(mesh_Boss_Terra.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _BOSS_TERRA_H_
#define _BOSS_TERRA_H_

#include "main.h"
#include "object.h"
#include "gauge_Boss_Life.h"
#include "mesh_obstacle.h"
#include "boss_reticle.h"
#include "modelparts.h"
#include "lockon_enemy.h"

#include "shadow.h"



class CBossBomb;

class CBossTerra :public CObject
{
public:
	CBossTerra();				//コンストラクタ
	~CBossTerra()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Attack(D3DXVECTOR3& Playerpos);
	void SetStatue();
	void SetBullet(D3DXVECTOR3& pos, D3DXVECTOR3& Playerpos);
	void Draw()override;		//描画
	static CBossTerra* Create(D3DXVECTOR3 pos);

	void Damaged(int nDmg) { m_nLife -= nDmg; m_Gauge->SetGauge(m_nLife); m_bDamaging = true; m_nDamageFrame = 0; };
	void DeathAnim();
	void DeathCheck();
	bool GetDamageState() { return m_bDamaging; }
	bool GetDeadState() { return m_bDead; }
	bool AttackRateCheck();
	D3DXMATRIX GetBossMtx(int idx) { return m_apModelParts[idx]->GetWorldMatrix(); }
	LPD3DXMESH GetBossMesh(int idx) { return m_apModelParts[idx]->GetMesh(); }
protected:

private:
	static constexpr float WORLD_WALL_X = 1000;
	static constexpr float WORLD_WALL_Y = 600;

	//モーション用定数
	static constexpr int MAX_MODELPARTS = 21;
	static constexpr int MAX_PARTS = 21;
	static constexpr int MAX_KEYSET = 21;
	static constexpr int MAX_MOTION = 9;
	static constexpr int MUZZLE_CUR = 6;


	D3DXVECTOR3 m_pos, m_rot, m_size;		//座標・回転・大きさ
	D3DXVECTOR3 m_move;						//移動量
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	D3DXMATRIX m_mtxWeapon[MUZZLE_CUR];		//武器のワールドマトリックス
	void SetWeaponMtx();

	CBossReticle* m_Reticle[2];
	CGaugeBoss* m_Gauge;
	int m_nLife;
	bool m_bMove;
	bool m_bDead;
	int m_nDamageFrame;
	int m_nDeadFrame;
	int m_nAttackFrame;
	bool m_bTransformed;
	bool m_bDamaging;

	CBossBomb* m_Bomb;

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//モデルパーツ数
	int m_CurKey;		//現在のキー番号
	int m_CurMotion;	//現在のモーション番号
	int m_NextMotion;	//現在のモーション番号
	int m_KeySetNum;	//キーセット数
	int m_MotionNum;	//モーション最大数
	int m_NowFrame;		//現在のフレーム
	float m_SecZrot;
	struct Key
	{
		//位置
		D3DXVECTOR3 pos;
		//向き
		D3DXVECTOR3 rot;
	};
	struct KeySet
	{

		int nFrame;
		Key aKey[MAX_PARTS];
	};
	struct Motion
	{
		int bLoop;
		int nKeyNum;
		KeySet aKetSet[MAX_KEYSET];
	};
	Motion m_aMotion[MAX_MOTION];

	void MotionInit();
	void MotionDataLoad();
	void SetNextMotion(int nNextMotionNum);
	void SetNextKey();
	bool MotionBlending();

	bool m_bBlend;
	bool m_bMotion;

	enum
	{
		MOTION_SHIP_NUTO = 0,
		MOTION_TRANSFORM,
		MOTION_ROBO_NUTO,
		MOTION_ROBO_BURST,
		MOTION_ROBO_FUNNEL,
		MOTION_ROBO_SHOOT,
		MOTION_ROBO_DIE,
		MOTION_ROBO_SHOT1,
		MOTION_ROBO_SHOT2,
	};
	CShadow* m_pShadow;
};

class CBossStatue : public CMeshObstacle
{
public:
	CBossStatue() :m_bSeted(false) {};
	~CBossStatue()override = default;
	void Update()override
	{
		if (!m_bSeted)
		{
			SetYPos();
		}
	}
	static CBossStatue* Create(D3DXVECTOR3 pos);
private:
	void SetYPos();
	bool m_bSeted;
};

class CBossEnemySpawner : public CObjectX
{
public:
	CBossEnemySpawner() : m_moveVec({0.0f,0.0f,0.0f}), m_Lifetime(), m_CntTime(0){};
	~CBossEnemySpawner()override = default;

	void Init(int num);
	void Update()override;
	static CBossEnemySpawner* Create(D3DXVECTOR3 pos,int num);
protected:
	static constexpr float WORLD_WALL_X = 1000;
	static constexpr float WORLD_WALL_Y = 600;

	void Move();
	void SetEnemy();
	D3DXVECTOR3 m_moveVec;
	int m_Lifetime;
	int m_CntTime;
};

class CBossFunnel : public CBossEnemySpawner
{
public:
	CBossFunnel() : m_moveVec({ 0.0f,0.0f,0.0f }), m_Lifetime(), m_CntTime(0) {};
	~CBossFunnel()override = default;

	static CBossFunnel* Create(D3DXVECTOR3 pos, int num);
protected:
	//void Move()override;
	//void SetEnemy()override;
	D3DXVECTOR3 m_moveVec;
	int m_Lifetime;
	int m_CntTime;
};

class CBossKnife : public CObjectX
{
public:
	CBossKnife() :m_Reach(0), m_Sec(0), m_StartPos({ 0.0f,0.0f,0.0f }), m_Target1({ 0.0f,0.0f,0.0f }), m_Target2({ 0.0f,0.0f,0.0f }), m_RotValue(0) { lockon = nullptr; };
	~CBossKnife()override = default;

	void Init()override;
	void Update()override;
	void Draw()override { CObjectX::Draw(m_mtxRot); lockon->Draw(); }
	static CBossKnife* Create(D3DXVECTOR3 startPos,int Reach,bool Side);
	void Braking();
private:
	static constexpr float KNIFE_ROTSPEED = 0.75f;

	D3DXVECTOR3 m_StartPos,m_Target1,m_Target2;
	int m_Reach;
	int m_Sec;
	float m_RotValue;
	CLockonEnemy* lockon;

	D3DXMATRIX m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;		//クオータニオン
	D3DXVECTOR3 m_vecAxis;		//回転軸のベクトル
};

class CBossBomb : public CObjectX
{
public:
	CBossBomb(){};
	~CBossBomb()override = default;

	void Init()override;
	void Update()override;
	void Draw()override;
	static CBossBomb* Create(D3DXVECTOR3 pos);
private:
};


#endif