//===============================================================================
//
//  C++使った2D(object2D.h)
//								制作：元地弘汰
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
	CObject2D(int nPriority = 3);						//コンストラクタ
	~CObject2D()override;				//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	D3DXVECTOR3 GetPos();				//座標取得
	void AddPos(D3DXVECTOR3 pos);		//座標移動
	void SetPos(D3DXVECTOR3 pos);		//座標変更
	D3DXVECTOR3 GetRot();				//角度取得
	void AddRot(D3DXVECTOR3 rot);		//角度移動
	D3DXVECTOR2 GetAnim();				//アニメーション取得
	void SetAnim(D3DXVECTOR2 Anim);		//アニメーション移動
	float GetSize();					//大きさ取得
	void SetZoom(float Zoom);			//大きさ変更
	D3DXCOLOR GetColor() { return m_col; };			//カラーを取得
	void SetColor(D3DXCOLOR col) { m_col = col; };	//カラーを設定
	void SetGauge(int value);			//ゲージのサイズ変更
	void SetGaugeAnim(D3DXVECTOR2 value);
	static CObject2D* Create(D3DXVECTOR3 pos);
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth);					//初期設定
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth,D3DXCOLOR col);	//初期設定(オーバーロード)
	void SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, int nMaxGauge,bool Reverse);	//初期設定(ゲージ用オーバーロード)
	void BindTexture(LPDIRECT3DTEXTURE9 pTex, int AnimU, int AnimV);	//作ったテクスチャ /縦分割 /横分割
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