//===============================================================================
//
//  C++�g����2D(mesh_obstacle.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _TEST_OBSTACLE_H_
#define _TEST_OBSTACLE_H_

#include "main.h"
#include "objectX.h"

static constexpr int MAX_MODELTYPE = 12;
static constexpr float BOSS_EMITTE_DIS = 6000.0f;
static constexpr float ENEMY_EMITTE_DIS = 10000.0f;

static constexpr float OBJ_DESTROY_DIS = 12000.0f;


class CMeshObstacle :public CObjectX
{
public:
	static int GetMaxType()	{	return MAX_MODELTYPE;	}

	CMeshObstacle(int nPriority = 1);				//�R���X�g���N�^
	~CMeshObstacle()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	static CMeshObstacle* Create(D3DXVECTOR3 pos,int Type);
	static CMeshObstacle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int Type);
protected:

private:
	bool PlayerDistanceCheck();
};

class CScoreItemEmitter : public CMeshObstacle
{
public:
	CScoreItemEmitter() : m_bSceneCheck(false) {};
	~CScoreItemEmitter()override = default;
	void Init()override;		//������
	void Update()override;		//�X�V
private:
	bool m_bSceneCheck;
};

class CBossEmitter : public CMeshObstacle
{
public:
	CBossEmitter() : m_bSceneCheck(false) {};
	~CBossEmitter()override = default;
	void Init()override;		//������
	void Update()override;		//�X�V
private:
	bool m_bSceneCheck;
	bool PlayerLength();
};

class CEnemyEmitter : public CMeshObstacle
{
public:
	CEnemyEmitter() : m_bSceneCheck(false) {};
	~CEnemyEmitter()override = default;
	void Init()override;		//������
	void Update()override;		//�X�V
private:
	bool m_bSceneCheck;
	bool PlayerLength();
};

#endif