//===============================================================================
//
//  �����蔻�菈���N���X(collision.h)
//								����F���n�O��
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
	
	bool CircleCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious);	//�~�`�����蔻��
	bool SphireCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious);	//�~�`�����蔻��

	bool MeshToIntersectCollision(LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);			//���C�ƃ��b�V���̌�������
	bool MeshToIntersectCollision(float* Distance,LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);			//���C�ƃ��b�V���̌�������
	bool MeshToIntersectCollision(CMeshObstacle* pObs, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance);		//���b�V�����ړ��E��]���������̏������܂ރI�[�o�[���[�h

};

#endif 
