//////////////////////////////////////////////////////////////////////////
// ���̴�
//////////////////////////////////////////////////////////////////////////

// �� ���� ���� ����

float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color
int g_nNumLights;

float3 g_LightDir[3];               // Light's direction in world space
float4 g_LightDiffuse[3];           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color

texture g_MeshTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix



//--------------------------------------------------------------------------------------
// Texture ���÷�
//
// �ؽ��� ���� ��Ģ�� �����ش�.
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	// ���� ����
};


// CUSTOM_VERTEX ������ �ϴ� ����ü
// ��ǥ, ����, UV �ؽ��� ��ǥ ����
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// ���ؽ� ���� �Լ�
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, float3 vNormal : NORMAL, float2 vTexCoord0 : TEXCOORD0,
                         uniform int nNumLights, uniform bool bTexture, uniform bool bAnimate )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    float4 vAnimatedPos = vPos;
    
    // ������ �÷��װ� true�� ���
	// C++ �������� �޾ƿ� g_fTime �ð��� ���� ��� ��
	if ( bAnimate )
	{
		vAnimatedPos += float4( vNormal, 0 ) * ( sin( g_fTime + 5.5 ) + 0.5 ) * 5;
	}

	// �ð� �ְ�, ��� �ְ�(�׷��� ���� �� �ɾ ��),
	// ���̴����� ����� ��Ű�� �������� ���� ����� ����
    
    // ���̴� �ʿ��� ����� ��İ� ���� ������ ����� ��ǥ�� ������
    Output.Position = mul(vAnimatedPos, g_mWorldViewProjection);
    
    // ���� ��ǥ�� ��ֶ����� ��
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld));
    
    // ���� �ʱ�ȭ
    float3 vTotalLightDiffuse = float3(0,0,0);

	for ( int i = 0; i < nNumLights; i++ )
	{
		vTotalLightDiffuse += g_LightDiffuse[i] * max( 0, dot( vNormalWorldSpace, g_LightDir[i] ) );
		// �� ��� ���� ����
	}
        
    Output.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse + g_MaterialAmbientColor * g_LightAmbient;   
    Output.Diffuse.a = 1.0f; 
    
    // �ؽ��İ� ������ UV ��ǥ ����, �ƴϸ� 0����
	if ( bTexture )
	{
		Output.TextureUV = vTexCoord0;
	}
	else
	{
		Output.TextureUV = 0;
	}
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// �ȼ� ����ü
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// �ȼ� ���� �Լ�
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In, uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // �ؽ��İ� ������ �ռ� ���ؽ� ���̴����� ����� Diffuse ����
	// ���÷� ���� �Ǿ� ���� �� �ؽ��� ���� ����
	if ( bTexture )
	{
		Output.RGBColor = tex2D( MeshTextureSampler, In.TextureUV ) * In.Diffuse;
	}
	else
	{
		Output.RGBColor = In.Diffuse;
	}

    return Output;
}


// ��ũ�� ����
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 1, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true );
    }
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 2, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true );
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 3, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true );
    }
}

technique RenderSceneNoTexture
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 1, false, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false );
    }
}
