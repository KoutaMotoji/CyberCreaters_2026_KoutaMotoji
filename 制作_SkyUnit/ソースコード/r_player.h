//===============================================================================
//
//  リザルトシーン用プレイヤー処理(t_player.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _R_PLAYERX_H_
#define _R_PLAYERX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"

class CResultPlayer :public CObject
{
public:
	CResultPlayer(int nPriority = 0);					//コンストラクタ
	~CResultPlayer()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	inline void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CResultPlayer* Create(D3DXVECTOR3 pos);

	inline D3DXVECTOR3 GetPos() { return m_pos; };
private:
	static constexpr int MAX_MODELPARTS = 20;

	static constexpr int MAX_PARTS = 20;
	static constexpr int MAX_KEYSET = 20;
	static constexpr int MAX_MOTION = 7;

	D3DXVECTOR3 m_pos, m_rot, m_size;	//座標・回転・大きさ
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_OldPos;			//過去の位置
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	static const float MOVE_SPEED;			//移動スピードの設定

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//モデルパーツ数
	int m_CurKey;		//現在のキー番号
	int m_CurMotion;	//現在のモーション番号
	int m_NextMotion;	//現在のモーション番号
	int m_KeySetNum;	//キーセット数
	int m_MotionNum;	//モーション最大数
	int m_NowFrame;		//現在のフレーム
	bool m_bBlend;

	struct Key
	{
		//位置
		D3DXVECTOR3 pos;
		//向き
		D3DXVECTOR3 rot;
	};
	struct KeySet
	{

		int nFrame;
		Key aKey[MAX_PARTS];
	};
	struct Motion
	{
		int bLoop;
		int nKeyNum;
		KeySet aKetSet[MAX_KEYSET];
	};
	Motion m_aMotion[MAX_MOTION];

	void MotionInit();
	void MotionDataLoad();
	void SetNextMotion(int nNextMotionNum);
	void SetNextKey();
	bool MotionBlending();

	//=====================			クオータニオン用		====================================
	D3DXMATRIX m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;		//クオータニオン
	D3DXVECTOR3 m_vecAxis;		//回転軸のベクトル
	float m_fValueRot;			//回転量
};

#endif