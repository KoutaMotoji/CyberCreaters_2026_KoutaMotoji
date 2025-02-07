//===============================================================================
//
//  C++使った2D(mesh_ground.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _MESH_GROUND_H_
#define _MESH_GROUND_H_

#include "main.h"
#include "objectX.h"

class CMeshGround :public CObjectX
{
public:
	CMeshGround(int nPriority = 0);				//コンストラクタ
	~CMeshGround()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CMeshGround* Create(D3DXVECTOR3 pos);
	static CMeshGround* Create(D3DXVECTOR3 pos,int i);

protected:

private:
	int m_mode;
};

#endif