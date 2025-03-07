//===============================================================================
//
//  �}�b�v�I�u�W�F�N�g�z�u�V�[��(map_edit.h)
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

	inline D3DXVECTOR3 GetThisPos() { return m_thisPos; }
	static void SetLoadMap();		//�O������}�b�v��ǂݍ��ޗp�֐�
private:
	void SaveFile();		//�z�u����ۑ�
	void EditObj();			//�I�u�W�F�N�g��z�u
	void SelectObjType();	//�ݒu�I�u�W�F�N�g�̎�ނ̑I��
	void SelectObject();	//�I�����ꂽ�I�u�W�F�N�g�̎�ނɕς��鏈��
	void SetObject();		//Enter�����������̃I�u�W�F�N�g����

	void SetEditPos();		//�ݒu�I�u�W�F�N�g�̍��W��ς��鏈��
	void SetEditRot();		//�ݒu�I�u�W�F�N�g�̉�]��ς��鏈��
	void SetEditScale();	//�ݒu�I�u�W�F�N�g�̃X�P�[����ς��鏈��

	CObjectX* m_SelectObject;
	const float MOVE_SCALE;
	float m_CamDis,m_SaveDis;

	D3DXVECTOR3 m_thisPos,m_thisRot,m_thisScale;	//�ݒu�v���r���[�I�u�W�F�N�g�p�v�f
	int m_thisType;									//�ݒu�I�u�W�F�N�g�̎�ޗp
	int m_MaxObj;
	SetInfo ObjInfo[MAX_EDITOBJ];

	//�f�o�b�O�\���p�t�H���g
	LPD3DXFONT m_pFont;		//�t�H���g�̃|�C���^
	void InitFont();		//�t�H���g�������p�֐�
	void UninitFont();		//�t�H���g�j��
	void DrawFont();		//�t�H���g�`��

	CGizmo* m_Gizmo;
	std::string LoadFilename;
};

//��]��̈ړ������𕪂���₷�����邽�߂̃M�Y��
class CGizmo : public CObjectX
{
public:
	CGizmo() :m_bdraw(true) {};
	void Init()override;
	void Draw()override;
	static CGizmo* Create(D3DXVECTOR3 pos);
	bool m_bdraw;
private:
};

#endif