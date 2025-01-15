//===============================================================================
//
//  C++使った2D(go_player.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "go_player.h"
#include "shadow.h"

const float CGameOverPlayer::MOVE_SPEED = 0.55f;


//==========================================================================================
//コンストラクタ
//==========================================================================================
CGameOverPlayer::CGameOverPlayer(int nPriority) : CObject(nPriority)
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i] = nullptr;
	}
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CGameOverPlayer::~CGameOverPlayer()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CGameOverPlayer::Init()
{
	ModelDataLoad();
	CObject::SetType(TYPE_3D_PLAYER);
	m_pShadow = CShadow::Create(m_pos, 50);

	//CManager::GetInstance()->GetCamera()->SetRotz(D3DX_PI);
}

//==========================================================================================
//終了処理
//==========================================================================================
void CGameOverPlayer::Uninit()
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Uninit();
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CGameOverPlayer::Update()
{
	m_pShadow->SetShadowGround(m_pos);

	m_pos += m_move;
	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.017f;
	m_move.y += (0.0f - m_move.y) * 0.017f;
	m_move.z += (0.0f - m_move.z) * 0.017f;
}

//==========================================================================================
//描画処理
//==========================================================================================
void CGameOverPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxSize);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Draw();
	}
}

//==========================================================================================
//生成処理
//==========================================================================================
CGameOverPlayer* CGameOverPlayer::Create(D3DXVECTOR3 pos)
{
	CGameOverPlayer* player = new CGameOverPlayer;
	player->Init();

	player->m_pos = pos;
	player->m_move = { 0.0f,0.0f,0.0f };
	player->m_rot = { 0.0f,0.0f,0.0f };
	player->m_size = { 1.0f,1.0f,1.0f };
	player->m_OldPos = pos;
	return player;
}

//==========================================================================================
//床当たり判定
//==========================================================================================
void CGameOverPlayer::FloorCollision()
{

}


//==========================================================================================
//モーションをファイルから読み込み
//==========================================================================================
void CGameOverPlayer::ModelDataLoad()
{
	char LoadData[128];
	char ModelPath[128];

	int nNumModel = 0;
	FILE* pFile;
	int nCnt = 0;

	int nMotionCnt = 0;
	int nKeySet = 0;
	int nKey = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	int nFilenameCnt = 0;
	int nParent = 0;
	int nIndex = 0;
	int nModelCnt = 0;

	pFile = fopen("data\\TEXT\\motion_gameover_player.txt", "r");

	if (pFile != nullptr)
	{
		while (1)
		{
			fscanf(pFile, "%s", LoadData);

			if (!strcmp(LoadData, "END_SCRIPT"))	// ファイルの最後
			{
				fclose(pFile);
				break;
			}

			if (LoadData[0] == '#')		// 文字飛ばし
			{
				continue;
			}

			//モデルの読み込み
			if (!strcmp(LoadData, "NUM_MODEL"))
			{
				fscanf(pFile, "%s", LoadData);
				fscanf(pFile, "%d", &m_ModelParts);
			}
			//モデルのファイル名読み込み
			if (!strcmp(LoadData, "MODEL_FILENAME"))
			{
				fscanf(pFile, "%s", LoadData);

				fscanf(pFile, "%s", ModelPath);

				m_pModelFileName[nFilenameCnt] = ModelPath;

				m_apModelParts[nFilenameCnt] = CModelParts::Create(pos, m_pModelFileName[nFilenameCnt]);

				++nFilenameCnt;
			}
			//キャラクターの設定の読み込み開始
			if (!strcmp(LoadData, "CHARACTERSET"))
			{
				while (1)
				{
					fscanf(pFile, "%s", LoadData);

					if (!strcmp(LoadData, "END_CHARACTERSET"))//読み込みを終了
					{
						break;
					}
					else if (!strcmp(LoadData, "PARTSSET"))
					{
						while (1)
						{
							fscanf(pFile, "%s", LoadData);

							if (LoadData[0] == '#')
							{//文字飛ばし
								fgets(LoadData, 100, pFile);
								continue;
							}

							if (!strcmp(LoadData, "END_PARTSSET"))
							{
								//読み込みを終了
								break;
							}
							//各パーツのモーションpos値
							else if (!strcmp(LoadData, "INDEX"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nIndex);
							}

							//各パーツのモーションrot値
							else if (!strcmp(LoadData, "PARENT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nParent);
							}
							//各パーツのモーションpos値
							else if (!strcmp(LoadData, "POS"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}

							//各パーツのモーションrot値
							else if (!strcmp(LoadData, "ROT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
						}
						m_apModelParts[nModelCnt]->SetPos(pos);
						m_apModelParts[nModelCnt]->SetRot(rot);

						m_apModelParts[nModelCnt]->SetDefault();
						m_apModelParts[nModelCnt]->SetIndex(nIndex);
						if (nParent != -1)
						{
							m_apModelParts[nModelCnt]->SetParent(m_apModelParts[nParent]);
						}
						++nModelCnt;
					}
				}
			}
		}
	}
	else
	{
		assert(pFile == nullptr);
	}
}