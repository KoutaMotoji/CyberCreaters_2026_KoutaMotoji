//===============================================================================
//
//  C++�g����2D(object3D.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

#include "main.h"
#include "object.h"

class CMeshCylinder :public CObject
{
public:
	CMeshCylinder(int nPliority = 0);				//�R���X�g���N�^
	~CMeshCylinder()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CMeshCylinder* Create(D3DXVECTOR3 pos);

	LPD3DXMESH GetMesh() { return m_pMesh; };
	D3DXVECTOR3 GetPos() { return m_pos; }
private:
	
	D3DXVECTOR3 m_pos, m_rot;
	D3DXMATRIX m_mtx;

	LPD3DXMESH m_pMesh;

	//�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_apTexMeshCylinder;

	static const int MAX_VTX;
	static const int MAX_CORNER;
	static const int MAX_INDEX;

	static const float MAX_WIDTH;
	static const float MAX_HEIGHT;

};

#endif