//===============================================================================
//
//  C++�g����3D(map_edit.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _MAP_EDIT_H_
#define _MAP_EDIT_H_

#include "scene.h"
#include "objectX.h"
#include "mesh_obstacle.h"

#define MAX_EDITOBJ	(1024)

static constexpr int MAX_OBJNUM = 12;


class CGizmo;

class CMapEdit : public CScene
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 scale;
		int SelType;
	}SetInfo;

	CMapEdit();
	~CMapEdit()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void LoadFile();	//�t�@�C������ǂݍ���Ŕz�u(�ꊇ)

	D3DXVECTOR3 GetThisPos() { return m_thisPos; };
	static void SetLoadMap();
private:
	void SaveFile();	//�z�u����ۑ�
	void EditObj();		//�I�u�W�F�N�g��z�u
	void SelectObjType();
	void SelectObject();
	void SetObject();	//Enter�����������̃I�u�W�F�N�g����

	void SetEditPos();
	void SetEditRot();
	void SetEditScale();

	CObjectX* m_SelectObject;
	CObjectX* m_LastObj;
	const float MOVE_SCALE;
	float m_CamDis,m_SaveDis;

	D3DXVECTOR3 m_thisPos,m_thisRot,m_thisScale;
	int m_thisType;
	int m_MaxObj;
	SetInfo ObjInfo[MAX_EDITOBJ];

	//�f�o�b�O�\���p�t�H���g
	LPD3DXFONT m_pFont;
	void InitFont();
	void UninitFont();
	void DrawFont();

	CGizmo* m_Gizmo;
};

class CGizmo : public CObjectX
{
public:
	CGizmo() :m_bdraw(true) {};
	void Init()override;
	void Draw()override;
	static CGizmo* Create(D3DXVECTOR3 pos);
	bool m_bdraw;

};

#endif