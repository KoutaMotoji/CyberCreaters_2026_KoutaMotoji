//===============================================================================
//
//  C++使った2D(t_starter.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _T_STARTER_H_
#define _T_STARTER_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"


static constexpr int MAX_MODELPARTS = 11;

static constexpr int MAX_PARTS = 11;
static constexpr int MAX_KEYSET =11;
static constexpr int MAX_MOTION = 3;

class CStarter :public CObject
{
public:
	CStarter();					//コンストラクタ
	~CStarter()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CStarter* Create(D3DXVECTOR3 pos);

	void SetStart();
private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//座標・回転・大きさ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//モデルパーツ数
	int m_CurKey;		//現在のキー番号
	int m_CurMotion;	//現在のモーション番号
	int m_NextMotion;	//現在のモーション番号
	int m_KeySetNum;	//キーセット数
	int m_MotionNum;	//モーション最大数
	int m_NowFrame;		//現在のフレーム

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

	enum
	{
		MOTION_NORMAL = 0,
		MOTION_START,
		MOTION_USED
	};
};

#endif