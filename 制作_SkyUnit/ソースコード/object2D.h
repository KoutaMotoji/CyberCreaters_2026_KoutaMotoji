//===============================================================================
//
//  C++�g����2D(object2D.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"

class CObject2D:public CObject
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_GAUGE,
		TYPE_MAX,
	}TYPE_2D;
	CObject2D(int nPriority = 3);						//�R���X�g���N�^
	~CObject2D()override;				//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	D3DXVECTOR3 GetPos();				//���W�擾
	void AddPos(D3DXVECTOR3 pos);		//���W�ړ�
	void SetPos(D3DXVECTOR3 pos);		//���W�ύX
	D3DXVECTOR3 GetRot();				//�p�x�擾
	void AddRot(D3DXVECTOR3 rot);		//�p�x�ړ�
	D3DXVECTOR2 GetAnim();				//�A�j���[�V�����擾
	void SetAnim(D3DXVECTOR2 Anim);		//�A�j���[�V�����ړ�
	float GetSize();					//�傫���擾
	void SetZoom(float Zoom);			//�傫���ύX
	D3DXCOLOR GetColor() { return m_col; };			//�J���[���擾
	void SetColor(D3DXCOLOR col) { m_col = col; };	//�J���[��ݒ�
	void SetGauge(int value);			//�Q�[�W�̃T�C�Y�ύX
	void SetGaugeAnim(D3DXVECTOR2 value);
	static CObject2D* Create(D3DXVECTOR3 pos);
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth);					//�����ݒ�
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth,D3DXCOLOR col);	//�����ݒ�(�I�[�o�[���[�h)
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, int nMaxGauge,bool Reverse);	//�����ݒ�(�Q�[�W�p�I�[�o�[���[�h)
	void BindTexture(LPDIRECT3DTEXTURE9 pTex, int AnimU, int AnimV);	//������e�N�X�`�� /�c���� /������
private:
	float CalcWidth();
	void UpdateNormal();
	void UpdateGaugeLeft();
	void UpdateGaugeRight();

	LPDIRECT3DTEXTURE9 m_pTextureObject2D;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffObject2D;
	D3DXVECTOR2 m_GaugeAnim;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXCOLOR m_col;
	static const int MAX_VERTEX;
	static const int MAX_POLYGON;
	float m_fAngle;
	float m_fLength;
	float m_fHeight;
	float m_fWidth;
	float m_fZoom;
	int m_AnimU, m_AnimV;
	int m_TexSliceU, m_TexSliceV;
	int m_nCharge,m_nMaxGauge;
	bool m_bGaugeReverse;
	TYPE_2D m_type;
};

#endif