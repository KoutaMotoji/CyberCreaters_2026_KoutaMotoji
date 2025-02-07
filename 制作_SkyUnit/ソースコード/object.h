//===============================================================================
//
//  C++使った2D(object.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

#define MAX_OBJECT	(1024)
#define SET_PRIORITY	(10)

class CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BG,
		TYPE_PLAYER,
		TYPE_BULLET,
		TYPE_EXPLOSON,
		TYPE_ENEMY,
		TYPE_PARTICLE,
		TYPE_BLOCK,
		TYPE_2D_UI,
		TYPE_3D_FIELD,
		TYPE_3D_PLAYER,
		TYPE_3D_BLOCK,
		TYPE_3D_STAGE_BG,
		TYPE_3D_S_MARKER,
		TYPE_3D_G_MARKER,
		TYPE_3D_WEAPON,
		TYPE_3D_ENEMY,
		TYPE_3D_ENEMY_WEAPON,
		TYPE_BILLBOARD,
		TYPE_3D_ITEMS,
		TYPE_3D_MESHOBJECT,
		TYPE_3D_OBSTACLE,
		TYPE_3D_BOSSTERRA,
		TYPE_3D_MADEMESH,
		TYPE_3D_TITLEOBJ,
		TYPE_3D_TUTORIALOBJ,
		TYPE_3D_RESULTOBJ,
		TYPE_3D_GAMEOVEROBJ,

		TYPE_3D_BOSSWEAPONS,
		TYPE_3D_BOSSBILLET
	}TYPE;

	CObject(int nPriority = 3);		//コンストラクタ
	virtual ~CObject();				//デストラクタ
	virtual void Init() = 0;		//初期化
	virtual void Uninit() = 0;		//終了
	virtual void Update() = 0;		//更新
	virtual void Draw() = 0;		//描画

	static void ReleaseAll();		//すべてリリース
	static void UpdateAll();		//すべて更新
	static void DrawAll();			//すべて描画
	
	void Release();					//リリース

	static CObject* GetObjects(int nPriority,int nIdx);
	
	TYPE GetType();
	void SetType(TYPE type);

protected:							
private:
	static CObject* m_apObject[SET_PRIORITY][MAX_OBJECT];
	int m_nID;
	int m_nPriority;
	static int m_nNumAll;
	TYPE m_type;
	//bool m_bDeath;					//trueであれば破棄
};

#endif