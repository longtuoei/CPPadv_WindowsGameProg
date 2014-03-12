//////////////////////////////////////////////////////////////////////////
// http://blog.naver.com/poweryang1/110031563312
// http://blog.naver.com/winkey83/43694984 ����
//////////////////////////////////////////////////////////////////////////

#include "DXUT.h"
#include "DXUTcamera.h"
#include "SDKmisc.h"
#include "resource.h"

#pragma once

CModelViewerCamera          g_Camera;               // ī�޶�
ID3DXEffect*                g_pEffect = NULL;       // ���̴�
ID3DXMesh*                  g_pMesh = NULL;         // �޽�
IDirect3DTexture9*          g_pMeshTexture = NULL;  // �ؽ���

//////////////////////////////////////////////////////////////////////////
// A shader compile flag that gives the compiler hints about how the shader will be used.
bool                        g_bEnablePreshader;

D3DXMATRIXA16               g_mCenterWorld;			// ���� ��Ʈ����

#define MAX_LIGHTS 3								// �ִ� ���� ���� 3��

//////////////////////////////////////////////////////////////////////////
// ���� �� Ŭ���� - ���� ����
CDXUTDirectionWidget		g_LightControl[MAX_LIGHTS];
//////////////////////////////////////////////////////////////////////////

float                       g_fLightScale;			// �� ���
int                         g_nNumActiveLights;		// ���� ���� ����
int                         g_nActiveLight;			// ���� Ȱ��ȭ ��(���� ��) ����

bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK OnLostDevice( void* pUserContext );
void CALLBACK OnDestroyDevice( void* pUserContext );

void InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	//////////////////////////////////////////////////////////////////////////
	// DXUT�� �����ϴ� �͵�
	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	// Direct3D ����̽� ����
	// Direct3D ����̽� ����
	// ����̽� �̺�Ʈ ó��
	// ������ �̺�Ʈ ó��
	// â ���� ��ü ȭ�� ��� ������ ��ȯ

	//////////////////////////////////////////////////////////////////////////
	// �ݹ� �Լ� ���
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// D3D �ݹ� ����

	// ��ġ�� ��� �� �� �ִ���
	DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );

	// Direct3D ����̽� ����
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
	
	// �ʱ�ȭ �Ǵ� Lost�� ���ҽ� �����
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
	
	// ����
    DXUTSetCallbackD3D9FrameRender( OnFrameRender );
	
	// ȭ�� ���(â ���, ��ü ȭ�� ���) ���� �� ó��
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
	
	// ���ҽ� ����
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
	//
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �Ϲ� �ݹ� ����

	
	// �޽��� �ڵ鸵
    DXUTSetCallbackMsgProc( MsgProc );
	
	// DXUT�� ������ ���� �� �� �����Ӹ��� ���� ȣ��
	// http://blog.naver.com/poweryang1/110031577088 ����
    DXUTSetCallbackFrameMove( OnFrameMove );
	
	//////////////////////////////////////////////////////////////////////////

	InitApp();
	// ���� �ʱ�ȭ

	DXUTInit( false, false );
	// ��ɾ� �� ���� ����, ���� �߻� �� �޽��� �ڽ� �� ��� ���� �ʱ� ����
	// http://cafe.naver.com/makkolli/55
	// http://cafe.naver.com/cafec/234056

    DXUTSetHotkeyHandling( true, true, false );
	//////////////////////////////////////////////////////////////////////////
	// �⺻ Ű �Է��� ó�� �� ���ΰ�
	//
	// Alt + Enter ��üâ
	// ESC�� ������
	// Pause�� ���߱�
	//////////////////////////////////////////////////////////////////////////

	// â ����
	DXUTCreateWindow( L"BasicHLSL" );
	
	// �ش� width, height�� ���� ������� ������ ���ΰ�, ��üâ���� ������ ���ΰ�
    DXUTCreateDevice( true, 800, 480 );
	
	// ���� ����
    DXUTMainLoop();
	
	//////////////////////////////////////////////////////////////////////////
	// ����� ���������� ���ҽ� ������ �̰��� ����
	//////////////////////////////////////////////////////////////////////////

	// DXUT�� ���� �ڵ带 �޾ƿ�
    return DXUTGetExitCode();
}


//////////////////////////////////////////////////////////////////////////
// ���α׷� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////
void InitApp()
{
    g_bEnablePreshader = true;

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� ���� �ʱ�ȭ
    for( int i = 0; i < MAX_LIGHTS; i++ )
	{
		g_LightControl[i].SetLightDirection( D3DXVECTOR3( sinf( D3DX_PI * 2 * i / MAX_LIGHTS - D3DX_PI / 6 ),
			0, -cosf( D3DX_PI * 2 * i / MAX_LIGHTS - D3DX_PI / 6 ) ) );
	}

    g_nActiveLight = 0;
	// index�� 0��

    g_nNumActiveLights = 1;
	// 1�� ���� ����

    g_fLightScale = 1.0f;
	// ������ Ǯ
}

//////////////////////////////////////////////////////////////////////////
// ��ġ�� ��� �� �� �ִ°�?
//////////////////////////////////////////////////////////////////////////
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// http://msdn.microsoft.com/en-us/library/bb172513(VS.85).aspx
	// �ȼ� ���̴� ���� üũ
	if ( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
	{
		return false;
	}

	// DXUT���� D3D9 ������Ʈ�� �����ͼ�
	IDirect3D9* pD3D = DXUTGetD3D9Object();

	//////////////////////////////////////////////////////////////////////////
	// ���ĺ��� üũ
	// http://ozlael.egloos.com/viewer/3362810
	//////////////////////////////////////////////////////////////////////////
	if ( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType, AdapterFormat,
		D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ����̽� ����
//////////////////////////////////////////////////////////////////////////
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

	// �޽� �ε� �� ����ȭ - �ش� �Լ� ����
    V_RETURN( LoadMesh( pd3dDevice, L"tiny\\tiny.x", &g_pMesh ) );

    D3DXVECTOR3* pData;
    D3DXVECTOR3 vCenter;
    FLOAT fObjectRadius;

	// �޽��� ���� �ɾ ���� ������ ������
    V( g_pMesh->LockVertexBuffer( D3DLOCK_READONLY, ( LPVOID* )&pData ) );
	
	//////////////////////////////////////////////////////////////////////////
	// �޽� ���(�浹 üũ�� ����) ����
	V( D3DXComputeBoundingSphere( pData, g_pMesh->GetNumVertices(), D3DXGetFVFVertexSize( g_pMesh->GetFVF() ), &vCenter, &fObjectRadius ) );
	
	// �޽� �� ���� - ���� ��
    V( g_pMesh->UnlockVertexBuffer() );

    D3DXMatrixTranslation( &g_mCenterWorld, -vCenter.x, -vCenter.y, -vCenter.z );
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI );
    g_mCenterWorld *= m;
    D3DXMatrixRotationX( &m, D3DX_PI / 2.0f );
    g_mCenterWorld *= m;

	// ���� �� Ŭ���� DXUTDirectionWidget�� �̿��� ��ġ ����
    V_RETURN( CDXUTDirectionWidget::StaticOnD3D9CreateDevice( pd3dDevice ) );
	
	for ( int i = 0; i < MAX_LIGHTS; i++ )
	{
		// ī�޶� ȸ�� �� ���� ���� ����
		g_LightControl[i].SetRadius( fObjectRadius );
	}

	// ���̴��� �����͸� �޸𸮿� �������� �������ν� ���̴��� �޸� ��뷮�� 50% ���� ���δ�.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
	// �޸� ����ȭ

#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	// InitApp() ���� true�� �̸� ���� �Ǿ� ����
	if ( !g_bEnablePreshader )
	{
		dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
	}

    // D3DX Effect ���� ã�ƺ���
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"BasicHLSL.fx" ) );

    // ������ ã�� �� ���� ��, ������ Effect ����
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL ) );

    // �ؽ��� ���� ã��
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"tiny\\tiny_skin.dds" ) );

	// ���� �ؽ��� ����
	V_RETURN( D3DXCreateTextureFromFileEx( pd3dDevice, str, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pMeshTexture ) );

    D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    D3DXCOLOR colorMtrlAmbient( 0.35f, 0.35f, 0.35f, 0 );

	//////////////////////////////////////////////////////////////////////////
	// �������
	V_RETURN( g_pEffect->SetValue( "g_MaterialAmbientColor", &colorMtrlAmbient, sizeof( D3DXCOLOR ) ) );
	V_RETURN( g_pEffect->SetValue( "g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof( D3DXCOLOR ) ) );
	V_RETURN( g_pEffect->SetTexture( "g_MeshTexture", g_pMeshTexture ) );
	// ������� ���̴� ����� ���� ����
	//////////////////////////////////////////////////////////////////////////

	// ī�޶� ����
    D3DXVECTOR3 vecEye( 0.0f, 0.0f, -15.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );

	// ī�޶� ���� ����
    g_Camera.SetViewParams( &vecEye, &vecAt );

	// ī�޶� ȸ�� �� ���� ���� ����
    g_Camera.SetRadius( fObjectRadius * 3.0f, fObjectRadius * 0.5f, fObjectRadius * 10.0f );

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// �޽� �ε��ϰ� ����ȭ �ϱ�
//////////////////////////////////////////////////////////////////////////
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
{
    ID3DXMesh* pMesh = NULL;
    WCHAR str[MAX_PATH];
    HRESULT hr;

    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );
	// �޽� ���� ã��

    V_RETURN( D3DXLoadMeshFromX( str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh ) );
	// �ε�

    DWORD* rgdwAdjacency = NULL;

	//////////////////////////////////////////////////////////////////////////
    // �޽� �ȿ� ��� ���Ͱ� ���� ��� ��� ���� ����
	if ( !( pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		ID3DXMesh* pTempMesh;
		V( pMesh->CloneMeshFVF( pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, pd3dDevice, &pTempMesh ) );
		V( D3DXComputeNormals( pTempMesh, NULL ) );

		SAFE_RELEASE( pMesh );
		pMesh = pTempMesh;
	}

    rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
    if( rgdwAdjacency == NULL )
        return E_OUTOFMEMORY;

	//////////////////////////////////////////////////////////////////////////
	// ���� ����ȭ�� ���ؼ� �޽��� ���ؽ����� ���� ���� ���� ��������
    V( pMesh->GenerateAdjacency( 1e-6f, rgdwAdjacency ) );

    V( pMesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL ) );
	//
	// �ƿ�ǲ �Ǿ� ���� ������ ������ �� �Ǿ� ���� �� ������ ������ �� �����ؼ� ĳ�ÿ� �־��
	//
	// ����ȭ - ĳ�� ����ϱ� �ɼ�
	// http://coreafive.tistory.com/201 ����
	//////////////////////////////////////////////////////////////////////////

    delete []rgdwAdjacency;

    *ppMesh = pMesh;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// http://blog.naver.com/proonan29/130065459062
//
// D3DPOOL_DEFAULT ���·� ���� �� ���ҽ���, Alt + Tab ������ ���α׷��� Focus�� �ٲ�ų�,
// â ��� <-> Ǯ ��� �� ��ȯ ������ LostDevice�� ���ҽ� Reset�� �߻��ϸ� ���⼭ �������
//////////////////////////////////////////////////////////////////////////
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	if ( g_pEffect )
	{
		V_RETURN( g_pEffect->OnResetDevice() );
	}

	for ( int i = 0; i < MAX_LIGHTS; i++ )
	{
		g_LightControl[i].OnD3D9ResetDevice( pBackBufferSurfaceDesc );
	}

	// ���� ����
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 2.0f, 4000.0f );
	
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );
	//////////////////////////////////////////////////////////////////////////
	// �� ȸ��
	// ��
	// ī�޶� ȸ��
	//////////////////////////////////////////////////////////////////////////

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// DXUT�� ������ ���� �� �� �����Ӹ��� ���� ȣ��, ���� �������� ���� ����
// ���� Object Update�� �� ������
//////////////////////////////////////////////////////////////////////////
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	// ī�޶� ��ġ ���
    g_Camera.FrameMove( fElapsedTime );
}

//////////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////////
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
    D3DXMATRIXA16 mWorldViewProjection;
    D3DXVECTOR3 vLightDir[MAX_LIGHTS];
    D3DXCOLOR vLightDiffuse[MAX_LIGHTS];
    UINT iPass, cPasses;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;

    // �ʱ�ȭ
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR( 0.0f, 0.25f, 0.25f, 0.55f ), 1.0f, 0 ) );

    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // ī�޶�κ��� ��� ������
		mWorld = g_mCenterWorld * ( *( g_Camera.GetWorldMatrix() ) );
        mProj = *( g_Camera.GetProjMatrix() );
		mView = *( g_Camera.GetViewMatrix() );

		mWorldViewProjection = mWorld * mView * mProj;

        // ���� ������ �׷��ִ� ��
        for( int i = 0; i < g_nNumActiveLights; i++ )
        {
            D3DXCOLOR arrowColor = ( i == g_nActiveLight ) ? D3DXCOLOR( 1, 1, 0, 1 ) : D3DXCOLOR( 1, 1, 1, 1 );

			// ����, Viewing Matrix, Projection Matrix, Eye Vector�� �����ϱ�
            V( g_LightControl[i].OnRender9( arrowColor, &mView, &mProj, g_Camera.GetEyePt() ) );

			// ���� ���� ����
            vLightDir[i] = g_LightControl[i].GetLightDirection();

			// ���� �� ����
            vLightDiffuse[i] = g_fLightScale * D3DXCOLOR( 1, 1, 1, 1 );
        }

		//////////////////////////////////////////////////////////////////////////
		// ���̴� ���� ����
		//
		// ���� ����
        V( g_pEffect->SetValue( "g_LightDir", vLightDir, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS ) );

		// ���� ����
        V( g_pEffect->SetValue( "g_LightDiffuse", vLightDiffuse, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS ) );

		// ���̴��� ��Ʈ���� �ǳ��� World * View * Projection Matrix
        V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );

		// ���̴��� ��Ʈ���� �ǳ��� World Matrix
        V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
		
		// ���̴��� �ð� ���� �ǳ���
		V( g_pEffect->SetFloat( "g_fTime", ( float )fTime * 10 ) );

        D3DXCOLOR vWhite = D3DXCOLOR( 1, 1, 1, 1 );

		// ���� �� ����
        V( g_pEffect->SetValue( "g_MaterialDiffuseColor", &vWhite, sizeof( D3DXCOLOR ) ) );
        
        V( g_pEffect->SetInt( "g_nNumLights", g_nNumActiveLights ) );

		// ���� ���� .fx ���Ͽ��� ���� �� ��� �����ϱ� ����
		switch ( g_nNumActiveLights )
		{
			case 1:
				V( g_pEffect->SetTechnique( "RenderSceneWithTexture1Light" ) );
				break;
			case 2:
				V( g_pEffect->SetTechnique( "RenderSceneWithTexture2Light" ) );
				break;
			case 3:
				V( g_pEffect->SetTechnique( "RenderSceneWithTexture3Light" ) );
				break;
		}


        // ������ ���̴� �����ϱ� ����
        V( g_pEffect->Begin( &cPasses, 0 ) );

		// �н� Ƚ�� ��ŭ �׷���
        for( iPass = 0; iPass < cPasses; iPass++ )
        {
			// http://blog.naver.com/unisocket/90017223793
			// ����
            V( g_pEffect->BeginPass( iPass ) );

			// ���̴����� ��ȯ�ϴ� technique�� �����Ͽ� �޽��� ������ �ϱ� ����
            V( g_pMesh->DrawSubset( 0 ) );

            V( g_pEffect->EndPass() );
			// ��
        }
        V( g_pEffect->End() );
		// ���̴� ��� ��

        V( pd3dDevice->EndScene() );
    }
}


//////////////////////////////////////////////////////////////////////////
// �޽��� �߻� ó���ϴ� �ݹ� �Լ�
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    g_LightControl[g_nActiveLight].HandleMessages( hWnd, uMsg, wParam, lParam );
	// ���� �� �ش� ��°�� ���� ��ȣ�� �´� CDXUTDirectionWidget ��ü���� �޽��� �߻��� �޾Ƽ� ó�� ��
	// �� Ŭ���� ���� ���ʿ��� ó��

    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
	// CModelViewerCamera ��ü���� �޽��� �߻��� �޾Ƽ� ó��
	// �� Ŭ���� ���� ���ʿ��� ó��

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// â�� �����ϴ�(����̽���) Focus�� �Ҿ����
//////////////////////////////////////////////////////////////////////////
void CALLBACK OnLostDevice( void* pUserContext )
{
    CDXUTDirectionWidget::StaticOnD3D9LostDevice();

    if( g_pEffect )
        g_pEffect->OnLostDevice();
}

//////////////////////////////////////////////////////////////////////////
// ���ҽ� ����
//////////////////////////////////////////////////////////////////////////
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    CDXUTDirectionWidget::StaticOnD3D9DestroyDevice();
	// �� ���� �� ���ҽ� ����

    SAFE_RELEASE( g_pEffect );
	// ���̴�

    SAFE_RELEASE( g_pMesh );
    SAFE_RELEASE( g_pMeshTexture );
}



