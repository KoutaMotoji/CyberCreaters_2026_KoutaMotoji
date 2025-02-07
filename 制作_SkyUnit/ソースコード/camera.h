//===============================================================================
//
//  3Dゲーム基礎(Block.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//カメラクラス
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();
	void SetCameraDistance(float distance) { m_camDistance = distance; };
	void AddCameraDistance(float dis) { m_camDistance = dis; };

	float GetCameraDistance() { return m_camDistance; };
	D3DXVECTOR3& GetPlayerPos();
	void SetPlayerPos(D3DXVECTOR3 pos);
	float GetRotZ();


	void SetFreeCam(D3DXVECTOR3 destPosV, D3DXVECTOR3 destPosR, int Frame);
	void DefuseFreeCam() { m_bFreeCam = false; }
	void SetRotz(float rot) { m_fRotZ = rot; };
	void AddRotz(float rot) { m_fRotZ += rot; };

	void SetCameraHeigjt(float Height) { m_camHeight = Height; };
	void AddCameraHeigjt(float Height) { m_camHeight += Height; };

	void SetShake(int nFlame, float fShake);
	void SetCamPos(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR) { m_posV = PosV; m_posR = PosR; }	//視点 / 注視点の設定
	void SetCamDefault() {
		m_posV = D3DXVECTOR3(0.0f, 100.0f, -300.0f);
		m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_PlayerPos = { 0.0f,0.0f,0.0f };
		m_camDistance = 600.0f;
		m_fRotZ = 0.0f;
	};

private:
	void UpdateNormalCam();
	void UpdateFreeCam();

	D3DXVECTOR3 m_posV, m_posR, m_posU,m_rot,m_PlayerPos;
	D3DXMATRIX m_mtxProjection, m_mtxView;
	float m_fRotZ;
	float m_camDistance;
	float m_camHeight;
	int m_nShakeFlame;
	float m_fShalePower;


	D3DXVECTOR3 m_DestPosV, m_DestPosR,m_LastPosV,m_LastPosR;
	int m_DestFrame,m_NowFrame;
	bool m_bFreeCam;
};

#endif 
