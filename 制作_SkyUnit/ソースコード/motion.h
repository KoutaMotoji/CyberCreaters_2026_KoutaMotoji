//===============================================================================
//
//  C++使った3D(motion.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "main.h"

#define MAX_PARTS	(16)
#define MAX_KEYSET	(16)
#define MAX_MOTION	(8)
class CMotion
{
public:
	CMotion();
	~CMotion();

	CMotion* Create(int MaxModelParts, int KeySetNum,int MotionNum);
private:
	int m_ModelParts;	//モデルパーツ数
	int m_CurKey;		//現在のキー番号
	int m_CntMotion;	//モーションカウンター
	int m_CurMotion;	//現在のモーション番号
	int m_KeySetNum;	//キーセット数
	int m_MotionNum;	//モーション最大数
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
		bool bLoop;
		int nKeyNum;
		KeySet aKetSet[MAX_KEYSET];
	};
	Motion aMotion[MAX_MOTION];

};

#endif