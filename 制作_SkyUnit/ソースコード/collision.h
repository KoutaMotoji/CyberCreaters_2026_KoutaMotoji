//===============================================================================
//
//  当たり判定処理クラス(collision.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
#include "mesh_obstacle.h"
#include "mesh_ground.h"

#define MAX_LIGHT	(3)
class CCollision
{
public:
	CCollision();
	~CCollision();
	
	bool CircleCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious);	//円形当たり判定
	bool SphireCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious);	//円形当たり判定

	bool MeshToIntersectCollision(LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);			//レイとメッシュの交差判定
	bool MeshToIntersectCollision(float* Distance,LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);			//レイとメッシュの交差判定
	bool MeshToIntersectCollision(CMeshObstacle* pObs, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);		//メッシュが移動・回転等した時の処理を含むオーバーロード

};

#endif 
