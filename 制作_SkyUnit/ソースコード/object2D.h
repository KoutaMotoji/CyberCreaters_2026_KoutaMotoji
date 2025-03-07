//===============================================================================
//
//  2Dオブジェクト基底クラス(object2D.h)
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
	inline D3DXVECTOR3 GetPos() { return m_pos; }				//座標取得
	inline void AddPos(D3DXVECTOR3 pos) { m_pos += pos; }		//座標移動
	inline void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//座標変更
	inline D3DXVECTOR3 GetRot() { return m_rot; }				//角度取得
	inline void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }		//角度移動
	inline D3DXVECTOR2 GetAnim() { return { (float)m_AnimU,(float)m_AnimV }; }					//アニメーション取得
	inline void SetAnim(D3DXVECTOR2 Anim) {m_AnimU = (int)Anim.x; m_AnimV = (int)Anim.y;}		//アニメーション移動
	inline float GetSize() {return m_fLength * m_fZoom;}					//大きさ取得
	inline void SetZoom(float Zoom) { m_fZoom = Zoom; }						//大きさ変更
	inline D3DXCOLOR GetColor() { return m_col; }							//カラーを取得
	inline void SetColor(D3DXCOLOR col) { m_col = col; }					//カラーを設定
	inline void SetGauge(int value) { m_nCharge = value; }					//ゲージのサイズ変更
	inline void SetGaugeAnim(D3DXVECTOR2 value) { m_GaugeAnim += value; }	//ゲージのテクスチャアニメーション
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