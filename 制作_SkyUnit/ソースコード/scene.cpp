//===============================================================================
//
//  シーン基底クラス(scene.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "scene.h"

#include "map_edit.h"
#include "object.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "result.h"
#include "gameover.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CScene::CScene()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CScene::~CScene()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CScene::Init()
{
	CManager::GetInstance()->GetCamera()->SetCamDefault();

	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CScene::Uninit()
{
	CManager::GetInstance()->GetSound()->StopSound();
	CObject::ReleaseAll();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CScene::Update()
{

}

//==========================================================================================
//描画処理
//==========================================================================================
void CScene::Draw()
{
	CObject::DrawAll();
}

//==========================================================================================
//生成処理
//==========================================================================================
CScene* CScene::Create(CScene::MODE mode)
{
	CScene* scene;
	switch (mode)
	{
	case MODE_TITLE:
		scene = new CTitle();
		scene->m_mode = mode;
		return scene;
		break;
	case MODE_TUTORIAL:
		scene = new CTutorial();
		scene->m_mode = mode;
		return scene;
		break;
	case MODE_GAME:
		scene = new CGame();
		scene->m_mode = mode;
		return scene;
		break;
	case MODE_RESULT:
		scene = new CResult();
		scene->m_mode = mode;
		return scene;
		break;
	case MODE_GAMEOVER:
		scene = new CGameover();
		scene->m_mode = mode;
		return scene;
		break;
	case MODE_MAPEDIT:
		scene = new CMapEdit();
		scene->m_mode = mode;
		return scene;
		break;
	}
}

//==========================================================================================
//現在のシーンを返す処理
//==========================================================================================
CScene::MODE CScene::GetSceneMode()
{
	return m_mode;
}