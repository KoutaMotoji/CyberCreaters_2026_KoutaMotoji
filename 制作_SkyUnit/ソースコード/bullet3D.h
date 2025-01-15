//===============================================================================
//
//  C++使った2D(object3D.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _BULLET3D_H_
#define _BULLET3D_H_

#include "main.h"
#include "billboard.h"

class CBullet3D :public CBillboard
{
public:
	CBullet3D(int nPriority = 4);				//コンストラクタ
	~CBullet3D()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CBullet3D* Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col,int nLife,float Radius,float EffectSize);
protected:
	virtual bool MeshCollision();
	float Poly_Size;
	float m_EffectSize;
	D3DXVECTOR3 m_move;
	D3DXCOLOR m_col;
	int m_nLife;

private:

};

#endif