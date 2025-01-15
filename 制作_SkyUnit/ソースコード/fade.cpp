//===============================================================================
//
//  C++使った3D(result.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "manager.h"
#include "texture.h"
#include "fade.h"

const float CFade::ALPHA_DEC = 5.0f;
const int CFade::MAX_ALPHA = 255;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CFade::CFade():m_bFinish(false),m_bUse(false), m_FADE_type(FADE_IN)
{
	m_pVtxBuffFade = nullptr;
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CFade::~CFade()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CFade::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffFade,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = { 0.0f,0.0f,0.0f };
	pVtx[1].pos = {SCREEN_WIDTH,0.0f,0.0f};
	pVtx[2].pos = {0.0f,SCREEN_HEIGHT,0.0f};
	pVtx[3].pos = {SCREEN_WIDTH,SCREEN_HEIGHT,0.0f};

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);

	//頂点バッファのアンロック
	m_pVtxBuffFade->Unlock();

	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CFade::Uninit()
{

	//頂点バッファの破棄
	if (m_pVtxBuffFade != nullptr)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = nullptr;
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CFade::Update()
{	
	if (!m_bFinish)
	{
		if (m_FADE_type == FADE_IN)
		{
			if (FadeIn())
			{
				m_bFinish = true;
				m_bUse = false;
				CManager::GetInstance()->SetMode(setmode);
				CManager::GetInstance()->GetSound()->StopSound();

				SetFade(FADE_OUT, setmode);
			}
		}
		else if(m_FADE_type == FADE_OUT)
		{
			if (FadeOut())
			{
				m_bFinish = true;
			}
		}
	}
	else if (m_bFinish)
	{
		m_bUse = false;
	}

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = { 0.0f,0.0f,0.0f };
	pVtx[1].pos = { SCREEN_WIDTH,0.0f,0.0f };
	pVtx[2].pos = { 0.0f,SCREEN_HEIGHT,0.0f };
	pVtx[3].pos = { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f };

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_nAlpha);

	//頂点バッファのアンロック
	m_pVtxBuffFade->Unlock();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CFade::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//==========================================================================================
//描画処理
//==========================================================================================
void CFade::SetFade(FADE_TYPE type, CScene::MODE mode)
{
	if (!m_bUse)
	{
		m_FADE_type = type;
		m_bUse = true;
		m_bFinish = false;

		setmode = mode;

		if (type == FADE_IN)
		{
			m_nAlpha = 0;
		}
		else
		{
			m_nAlpha = 255;
		}
		if (m_pVtxBuffFade == nullptr)
		{
			Init();
		}
	}
}

//==========================================================================================
//フェードが行われてるかを返す処理
//==========================================================================================
bool CFade::GetUse()
{
	return m_bUse;
}

//==========================================================================================
//フェードイン処理
//==========================================================================================
bool CFade::FadeIn()
{
	m_nAlpha += ALPHA_DEC;

	return m_nAlpha >= MAX_ALPHA;
}

//==========================================================================================
//フェードアウト処理
//==========================================================================================
bool CFade::FadeOut()
{
	m_nAlpha -= ALPHA_DEC;

	return m_nAlpha <= 0;
}
