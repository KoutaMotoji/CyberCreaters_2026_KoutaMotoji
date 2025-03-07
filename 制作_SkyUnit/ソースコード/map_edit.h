//===============================================================================
//
//  マップオブジェクト配置シーン(map_edit.h)
//								制作：元地弘汰
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
	void LoadFile();	//ファイルから読み込んで配置(一括)

	inline D3DXVECTOR3 GetThisPos() { return m_thisPos; }
	static void SetLoadMap();		//外部からマップを読み込む用関数
private:
	void SaveFile();		//配置情報を保存
	void EditObj();			//オブジェクトを配置
	void SelectObjType();	//設置オブジェクトの種類の選択
	void SelectObject();	//選択されたオブジェクトの種類に変える処理
	void SetObject();		//Enterを押した時のオブジェクト生成

	void SetEditPos();		//設置オブジェクトの座標を変える処理
	void SetEditRot();		//設置オブジェクトの回転を変える処理
	void SetEditScale();	//設置オブジェクトのスケールを変える処理

	CObjectX* m_SelectObject;
	const float MOVE_SCALE;
	float m_CamDis,m_SaveDis;

	D3DXVECTOR3 m_thisPos,m_thisRot,m_thisScale;	//設置プレビューオブジェクト用要素
	int m_thisType;									//設置オブジェクトの種類用
	int m_MaxObj;
	SetInfo ObjInfo[MAX_EDITOBJ];

	//デバッグ表示用フォント
	LPD3DXFONT m_pFont;		//フォントのポインタ
	void InitFont();		//フォント初期化用関数
	void UninitFont();		//フォント破棄
	void DrawFont();		//フォント描画

	CGizmo* m_Gizmo;
	std::string LoadFilename;
};

//回転後の移動方向を分かりやすくするためのギズモ
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