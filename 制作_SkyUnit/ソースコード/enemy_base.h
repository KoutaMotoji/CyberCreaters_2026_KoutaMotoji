//===============================================================================
//
//  �G���N���X(enemy_base.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _ENEMY_TERRA_H_
#define _ENEMY_TERRA_H_

#include "main.h"
#include "objectX.h"
#include "lockon_enemy.h"
#include "shadow.h"

class CEnemyBase :public CObjectX
{
public:
	CEnemyBase();				//�R���X�g���N�^
	~CEnemyBase()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CEnemyBase* Create(D3DXVECTOR3 pos);
	static CEnemyBase* Create(D3DXVECTOR3 pos,float size);

	void Damaged();
	void LockOned() 
	{
		if (m_bLockOned != true)
		{
			m_bLockOned = true;
			lockon = CLockonEnemy::Create(CObjectX::GetPos());
		}
	};
protected:
	bool m_bMove;
	D3DXVECTOR3 m_DefPos;
	bool m_bLockOned;
private:
	CLockonEnemy* lockon;
	CShadow* m_pShadow;
};



#endif