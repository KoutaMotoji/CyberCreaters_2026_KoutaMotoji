//===============================================================================
//
//  描画管理クラス(renderer.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "main.h"
#include "object.h"
#include "renderer.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_D3DDevice = nullptr;
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CRenderer::~CRenderer()
{
	//Direct3Dデバイスの破棄
	if (m_D3DDevice != NULL)
	{
		m_D3DDevice->Release();
		m_D3DDevice = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm; //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;
	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	ZeroMemory(&d3dpp, sizeof(d3dpp));//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	/*
	//デバッグ用表示フォントの生成
	D3DXCreateFont(m_D3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);
	*/

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_D3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理をハードウェア、頂点処理をCPUで行う
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_D3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_D3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//--------------------------------
	//各種オブジェクトの初期化処理
	//--------------------------------

	//レンダーステートの設定
	m_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// サンプラーステートの設定
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	// テクスチャステージステートの設定
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CRenderer::Uninit(void)
{
	/*
	//デバッグ用表示フォントの破棄k
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	*/
}

//==========================================================================================
//更新処理
//==========================================================================================
void CRenderer::Update(void)
{
	//--------------------------------
	//各種オブジェクトの更新処理
	//--------------------------------

	CObject::UpdateAll();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CRenderer::Draw(CScene* pScene)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	m_D3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(50, 50, 50, 0), 1.0f, 0);
	//描画開始
	if (SUCCEEDED(m_D3DDevice->BeginScene()))
	{//描画開始が成功した場合
			//--------------------------------
			//各種オブジェクトの描画処理
			//--------------------------------
		CManager::GetInstance()->GetCamera()->SetCamera();

		//ポリゴンの描画処理
		pScene->Draw();

		CManager::GetInstance()->GetFade()->Draw();

		//描画終了
		m_D3DDevice->EndScene();
		//バックバッファとフロントバッファの入れ替え
		m_D3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//==========================================================================================
//デバイスの取得
//==========================================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_D3DDevice;
}