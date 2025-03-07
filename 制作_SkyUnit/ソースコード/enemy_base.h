//===============================================================================
//
//  敵基底クラス(enemy_base.h)
//								制作：元地弘汰
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
	CEnemyBase();				//コンストラクタ
	~CEnemyBase()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
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