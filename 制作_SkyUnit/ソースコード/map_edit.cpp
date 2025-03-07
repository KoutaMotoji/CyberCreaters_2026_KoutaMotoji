//===============================================================================
//
//  マップオブジェクト配置シーン(map_edit.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "inicpp.h"

#include "manager.h"
#include "fade.h"
#include "object.h"
#include "map_edit.h"
#include "playerX.h"

#include "mesh_obstacle.h"
#include "mesh_ground.h"

namespace
{
	const char* typeName[MAX_OBJNUM] = {
		"ゲート",
		"デカ柱",
		"ビル１",
		"ビル２",
		"電波塔",
		"橋",
		"それっぽいもの",
		"ボス出現ポイント",
		"アイテム出現ポイント",
		"通常敵出現ポイント",
		"看板＿左右",
		"看板＿上下",
	};

	const char* Gizmodraw[2] = {
		"描画しない",
		"描画する",
	};

	int SizeDownScale = 150.0f;
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CMapEdit::CMapEdit() :m_MaxObj(0), MOVE_SCALE(20.0f), m_CamDis(3000.0f), m_SaveDis(3000.0f),LoadFilename({})
{
	//マップオブジェクトの初期化
	for (int i = 0; i < MAX_EDITOBJ; ++i)
	{
		ObjInfo[i].pos = { 0.0f,0.0f,0.0f };
		ObjInfo[i].rot = { 0.0f,0.0f,0.0f };
		ObjInfo[i].scale = { 1.0f,1.0f,1.0f };
		ObjInfo[i].SelType = 0;
	}
	m_SelectObject = nullptr;

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CMapEdit::~CMapEdit()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CMapEdit::Init()
{
	CScene::Init();
	InitFont();
	ini::IniFile myIni;

	std::string MapData;
	std::string filename = "data\\TEXT\\Config.ini";

	//INIファイルを読み込む
	myIni.load(filename);

	std::string temp3 = myIni["LoadMap"]["MapTextName"].as<std::string>();
	if (!temp3.empty())
	{
		MapData = temp3;
	}
	else
	{
		assert(temp3.empty());
	}
	LoadFilename = MapData;
	LoadFile();
	CManager::GetInstance()->GetCamera()->SetCameraDistance(m_CamDis);

	m_thisPos = { 0.0f,0.0f,0.0f };
	m_thisRot = { 0.0f,0.0f,0.0f };
	m_thisScale = { 1.0f,1.0f,1.0f };
	m_thisType = 0;
	m_Gizmo = CGizmo::Create(m_thisPos);
	SelectObject();

	for (int i = 0; i < 16; ++i)
	{
		CMeshGround::Create({ 0.0f,-1000.0f,(float)(5940 * 2 * i )});
	}


	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CMapEdit::Uninit()
{
	if (m_SelectObject != nullptr)
	{
		m_SelectObject->Release();
	}
	if (m_Gizmo != nullptr)
	{
		m_Gizmo->Uninit();
	}
	UninitFont();
	CScene::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CMapEdit::Update()
{
	D3DXVECTOR3 CameraPos;
	if (CManager::GetInstance()->GetCamera()->GetPlayerPos() != nullptr)
	{
		CameraPos = CManager::GetInstance()->GetCamera()->GetPlayerPos();
	}
	else
	{
		assert(!(CameraPos = CManager::GetInstance()->GetCamera()->GetPlayerPos()));
	}
	if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_UPARROW) == true)
	{
		m_CamDis -= 50.0f;
		m_SaveDis = m_CamDis;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_DOWNARROW) == true)
	{
		m_CamDis += 50.0f;
		m_SaveDis = m_CamDis;
	}
	CameraPos = m_thisPos;
	CameraPos.y = (m_SelectObject->GetModelMin().y + m_SelectObject->GetModelMax().y) * 0.5f + m_thisPos.y;
	EditObj();
	CManager::GetInstance()->GetCamera()->SetPlayerPos(CameraPos);
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_R) == true)
	{
		m_Gizmo->m_bdraw = !m_Gizmo->m_bdraw;
	}
	m_Gizmo->SetPos(m_thisPos);
	m_Gizmo->SetSize(m_thisScale);

	DrawFont();
	CScene::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CMapEdit::Draw()
{
	CScene::Draw();
	DrawFont();
}

//==========================================================================================
//オブジェクト配置処理
//==========================================================================================
void CMapEdit::EditObj()
{
	SetEditPos();
	SetEditRot();
	SetEditScale();
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		SetObject();
	}
	SelectObjType();
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_F1) == true)
	{
		SaveFile();
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_F2) == true)
	{
		LoadFile();
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_F3) == true)
	{
		SaveFile();

		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAME);
	}
}

//==========================================================================================
//外部ファイルに書き出す処理
//==========================================================================================
void CMapEdit::SaveFile()
{
	FILE* pFile;
	pFile = fopen(LoadFilename.c_str(), "w");
	if (pFile != nullptr)
	{
		fprintf(pFile, "%d\n", m_MaxObj);

		for (int i = 0; i < m_MaxObj; ++i)
		{
			fprintf(pFile, "%d\n", ObjInfo[i].SelType);
			fprintf(pFile, "%.2f,", ObjInfo[i].pos.x);
			fprintf(pFile, "%.2f,", ObjInfo[i].pos.y);
			fprintf(pFile, "%.2f\n", ObjInfo[i].pos.z);

			fprintf(pFile, "%.2f,", ObjInfo[i].rot.x);
			fprintf(pFile, "%.2f,", ObjInfo[i].rot.y);
			fprintf(pFile, "%.2f\n", ObjInfo[i].rot.z);

			fprintf(pFile, "%.2f,", ObjInfo[i].scale.x);
			fprintf(pFile, "%.2f,", ObjInfo[i].scale.y);
			fprintf(pFile, "%.2f\n", ObjInfo[i].scale.z);
		}
		fclose(pFile);
	}
}

//==========================================================================================
//外部ファイルから読み込む処理処理
//==========================================================================================
void CMapEdit::LoadFile()
{
	FILE* pFile;
	pFile = fopen(LoadFilename.c_str(), "r");
	if (pFile != nullptr)
	{
		int nGetCnt = 0;
		fscanf(pFile, "%d\n", &nGetCnt);
		m_MaxObj = nGetCnt;
		D3DXVECTOR3 GetPos;
		D3DXVECTOR3 GetRot;
		D3DXVECTOR3 GetScale;

		int nGetType;
		for (int i = 0; i < nGetCnt; ++i)
		{
			fscanf(pFile, "%d\n", &nGetType);
			fscanf(pFile, "%f,", &GetPos.x);
			fscanf(pFile, "%f,", &GetPos.y);
			fscanf(pFile, "%f\n", &GetPos.z);

			fscanf(pFile, "%f,", &GetRot.x);
			fscanf(pFile, "%f,", &GetRot.y);
			fscanf(pFile, "%f\n", &GetRot.z);

			fscanf(pFile, "%f,", &GetScale.x);
			fscanf(pFile, "%f,", &GetScale.y);
			fscanf(pFile, "%f\n", &GetScale.z);

			ObjInfo[i].pos = GetPos;
			ObjInfo[i].rot = GetRot;
			ObjInfo[i].scale = GetScale;

			ObjInfo[i].SelType = nGetType;
		}
		for (int j = 0; j < m_MaxObj; ++j)
		{
			CMeshObstacle::Create(ObjInfo[j].pos, ObjInfo[j].rot, ObjInfo[j].scale,ObjInfo[j].SelType);
		}
		fclose(pFile);
	}
}
//==========================================================================================
//オブジェクトの種類を選択する処理
//==========================================================================================
void CMapEdit::SelectObject()
{
	if (m_SelectObject != nullptr)
	{
		m_SelectObject->Release();
	}

	m_SelectObject = CMeshObstacle::Create(m_thisPos,m_thisRot,m_thisScale, m_thisType);
}
//==========================================================================================
//オブジェクトを設置する処理
//==========================================================================================
void CMapEdit::SetObject()
{
	CMeshObstacle::Create(m_thisPos, m_thisRot, m_thisScale, m_thisType);
	ObjInfo[m_MaxObj].pos = m_thisPos;
	ObjInfo[m_MaxObj].rot = m_thisRot;
	ObjInfo[m_MaxObj].scale = m_thisScale;
	ObjInfo[m_MaxObj].SelType = m_thisType;
	++m_MaxObj;
}

//==========================================================================================
//オブジェクトの種類を変える処理
//==========================================================================================
void CMapEdit::SelectObjType()
{
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_1) == true)	{
		--m_thisType;
		if (m_thisType < 0)	{
			m_thisType = MAX_OBJNUM - 1;
		}
		SelectObject();
	}
	else if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_2) == true)	{
		++m_thisType;
		if (m_thisType > MAX_OBJNUM - 1)	{
			m_thisType = 0;
		}
		SelectObject();
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//==========================================================================================
//オブジェクトの位置を変える処理
//==========================================================================================
void CMapEdit::SetEditPos()
{
	float ADD_Move = MOVE_SCALE * 4;

	if (CManager::GetInstance()->GetKeyboard()->GetRepeat(DIK_LSHIFT) == true)
	{
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_W) == true)
		{
			m_thisPos.z += ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_S) == true)
		{
			m_thisPos.z -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_A) == true)
		{
			m_thisPos.x -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_D) == true)
		{
			m_thisPos.x += ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_E) == true)
		{
			m_thisPos.y -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetPress(DIK_Q) == true)
		{
			m_thisPos.y += ADD_Move;
		}
	}
	else
	{

		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_W) == true)
		{
			m_thisPos.z += ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_S) == true)
		{
			m_thisPos.z -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_A) == true)
		{
			m_thisPos.x -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_D) == true)
		{
			m_thisPos.x += ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_E) == true)
		{
			m_thisPos.y -= ADD_Move;
		}
		if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_Q) == true)
		{
			m_thisPos.y += ADD_Move;
		}
	}
	m_SelectObject->SetPos(m_thisPos);
}

//==========================================================================================
//オブジェクトの向きを変える処理
//==========================================================================================
void CMapEdit::SetEditRot()
{
	float ADD_Rot = D3DX_PI * 0.125f;

	if (CManager::GetInstance()->GetKeyboard()->GetRepeat(DIK_LSHIFT) == true)
	{
		ADD_Rot *= 4;
	}

	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_Y) == true)
	{
		m_thisRot.y += ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_H) == true)
	{
		m_thisRot.y -= ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_N) == true)
	{
		m_thisRot.y = 0;
	}

	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_T) == true)
	{
		m_thisRot.x -= ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_G) == true)
	{
		m_thisRot.x += ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_B) == true)
	{
		m_thisRot.x = 0;
	}

	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_U) == true)
	{
		m_thisRot.z += ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_J) == true)
	{
		m_thisRot.z -= ADD_Rot;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_M) == true)
	{
		m_thisRot.z = 0;
	}

	if (m_thisRot.x < -D3DX_PI)
	{
		m_thisRot.x += D3DX_PI * 2;
	}
	else if (m_thisRot.x > D3DX_PI)
	{
		m_thisRot.x -= D3DX_PI * 2;
	}
	if (m_thisRot.y < -D3DX_PI)
	{
		m_thisRot.y += D3DX_PI * 2;
	}
	else if (m_thisRot.y > D3DX_PI)
	{
		m_thisRot.y -= D3DX_PI * 2;
	}
	if (m_thisRot.z < -D3DX_PI)
	{
		m_thisRot.z += D3DX_PI * 2;
	}
	else if (m_thisRot.z > D3DX_PI)
	{
		m_thisRot.z -= D3DX_PI * 2;
	}

	m_SelectObject->SetRot(m_thisRot);

}

//==========================================================================================
//オブジェクトの大きさを変える処理
//==========================================================================================
void CMapEdit::SetEditScale()
{
	float ADD_SIZE = 0.05f;

	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_C) == true)
	{
		m_thisScale += {ADD_SIZE, ADD_SIZE, ADD_SIZE};
		m_CamDis += SizeDownScale;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_Z) == true)
	{
		m_thisScale -= {ADD_SIZE, ADD_SIZE, ADD_SIZE};
		m_CamDis -= SizeDownScale;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_X) == true)
	{
		m_thisScale = {1.0f, 1.0f, 1.0f };
		m_CamDis = m_SaveDis;
	}
	m_SelectObject->SetSize(m_thisScale);
	CManager::GetInstance()->GetCamera()->SetCameraDistance(m_CamDis);

}
//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//

//==========================================================================================
//デバッグ表示用初期化処理
//==========================================================================================
void CMapEdit::InitFont()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得

	//デバッグ表示用フォントの生成
	D3DXCreateFont(pDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "HGPｺﾞｼｯｸE", &m_pFont);
}

//==========================================================================================
//デバッグ表示用終了処理
//==========================================================================================
void CMapEdit::UninitFont()
{
	//デバッグ表示用フォントの廃棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}
//==========================================================================================
//デバッグ表示用描画処理
//==========================================================================================
void CMapEdit::DrawFont()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1024] = {};

	sprintf(&str[0], "現在のオブジェクトの総数：%d\n"
		"現在のオブジェクトの位置：< %.2f,%.2f,%.2f >\n"
		"現在のオブジェクトの回転：< %.2f,%.2f,%.2f >\n"
		"現在のオブジェクトのスケール：< %.2f >\n"
		"現在のオブジェクトの種類：< %s >\n\n"
		"ギズモ：< %s>",
		m_MaxObj,
		m_thisPos.x, m_thisPos.y, m_thisPos.z, 
		m_thisRot.x, m_thisRot.y, m_thisRot.z,
		m_thisScale.x,
		typeName[m_thisType],
		Gizmodraw[(unsigned int)(m_Gizmo->m_bdraw)]);

	 //テキストの描画
	m_pFont->DrawText(NULL, 
		&str[0],
		-1, 
		&rect, 
		DT_LEFT, 
		D3DCOLOR_RGBA(255, 255, 255, 255));

	//memset(&str[0], NULL, sizeof(str));
}
//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//==========================================================================================
//外部から設置する処理
//==========================================================================================
void CMapEdit::SetLoadMap()
{
	ini::IniFile myIni;

	std::string MapData;
	std::string filename = "data\\TEXT\\Config.ini";

	//INIファイルを読み込む
	myIni.load(filename);

	std::string temp3 = myIni["LoadMap"]["MapTextName"].as<std::string>();
	if (!temp3.empty())
	{
		MapData = temp3;
	}
	else
	{
		assert(temp3.empty());
	}

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 scale;
		int SelType;
	}SetInfo;
	SetInfo ObjInfo[1024] = {};
	int MaxObj = 0;
	FILE* pFile;
	pFile = fopen(MapData.c_str(), "r");

	if (pFile != nullptr)
	{
		int nGetCnt = 0;
		fscanf(pFile, "%d\n", &nGetCnt);
		MaxObj = nGetCnt;
		D3DXVECTOR3 GetPos;
		D3DXVECTOR3 GetRot;
		D3DXVECTOR3 GetScale;

		int nGetType;
		for (int i = 0; i < nGetCnt; ++i)
		{
			fscanf(pFile, "%d\n", &nGetType);
			fscanf(pFile, "%f,", &GetPos.x);
			fscanf(pFile, "%f,", &GetPos.y);
			fscanf(pFile, "%f\n", &GetPos.z);

			fscanf(pFile, "%f,", &GetRot.x);
			fscanf(pFile, "%f,", &GetRot.y);
			fscanf(pFile, "%f\n", &GetRot.z);

			fscanf(pFile, "%f,", &GetScale.x);
			fscanf(pFile, "%f,", &GetScale.y);
			fscanf(pFile, "%f\n", &GetScale.z);

			ObjInfo[i].pos = GetPos;
			ObjInfo[i].rot = GetRot;
			ObjInfo[i].scale = GetScale;

			ObjInfo[i].SelType = nGetType;
		}
		for (int j = 0; j < MaxObj; ++j)
		{
			CMeshObstacle::Create(ObjInfo[j].pos, ObjInfo[j].rot, ObjInfo[j].scale, ObjInfo[j].SelType);
		}
		fclose(pFile);
	}
}

void CGizmo::Init()
{
	CObject::SetType(TYPE_3D_BLOCK);
	CObjectX::Init();

}

void CGizmo::Draw()
{
	if (m_bdraw)
	{
		CObjectX::Draw();
	}
}

//==========================================================================================
//生成処理
//==========================================================================================
CGizmo* CGizmo::Create(D3DXVECTOR3 pos)
{
	CGizmo* gzm = new CGizmo;

	gzm->BindModel("data\\MODEL\\Gizmo.x");
	gzm->SetModelParam(pos);
	gzm->Init();

	return gzm;
}