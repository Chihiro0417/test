////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Gauge.cpp
//		
//		�X�V����	�X�V�����l	�X�V���e
//		------------------------------------------
//		2022/12/13	�X�{		Gauge.h/cpp�쐬
//		
//		
//		
//		
//		���e
//		�E�|���S���̌��_�̈ʒu�����[�ɕς���Srite.cpp�����̂��̂ł�
//		�E�g������Sprite.cpp�Ɠ��l�ł�
//
//
//		���ӓ_
//		�E�d�l�̍ۂ�Sprite.h/cpp���lMain.cpp�ɂăC���N���[�h��Init/UnInit�̌Ăяo�������肢���܂�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Gauge.h"

MeshBuffer*					CGauge::m_pMesh;
ConstantBuffer*				CGauge::m_pBuf[2];
DirectX::XMFLOAT4X4			CGauge::m_matrix[3];
DirectX::XMFLOAT4			CGauge::m_param[3];
ID3D11ShaderResourceView*	CGauge::m_pTexture;
VertexShader*				CGauge::m_pVS;
PixelShader*				CGauge::m_pPS;
VertexShader*				CGauge::m_pDefVS;
PixelShader*				CGauge::m_pDefPS;

void CGauge::Init()
{
	const char* VS = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Param : register(b1) {
	float2 offset;
	float2 size;
	float2 uvPos;
	float2 uvScale;
	float4 color;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos.xy *= size;
	vout.pos.xy += offset;
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.uv *= uvScale;
	vout.uv += uvPos;
	vout.color = color;
	return vout;
})EOT";
	const char* PS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
	return tex.Sample(samp, pin.uv) * pin.color;
})EOT";

	struct Vertex
	{
		float pos[3];
		float uv[2];
	} vtx[] = {
		{{ 0.0f, 0.5f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f, 0.5f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.0f,-0.5f, 0.0f}, {0.0f, 1.0f}},
		{{ 1.0f,-0.5f, 0.0f}, {1.0f, 1.0f}},
	};

	// ���b�V��
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = _countof(vtx);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_pMesh = new MeshBuffer(desc);

	// �萔�o�b�t�@
	m_pBuf[0] = new ConstantBuffer;
	m_pBuf[0]->Create(sizeof(m_matrix));
	m_pBuf[1] = new ConstantBuffer;
	m_pBuf[1]->Create(sizeof(m_param));

	// �p�����[�^�[
	m_param[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_param[1] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_param[2] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&m_matrix[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_matrix[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_matrix[2], DirectX::XMMatrixIdentity());

	// �V�F�[�_�[
	m_pDefVS = new VertexShader;
	m_pDefVS->Compile(VS);
	m_pDefPS = new PixelShader;
	m_pDefPS->Compile(PS);
	m_pVS = m_pDefVS;
	m_pPS = m_pDefPS;
}
void CGauge::Uninit()
{
	delete m_pDefPS;
	delete m_pDefVS;
	delete m_pBuf[1];
	delete m_pBuf[0];
	delete m_pMesh;
}
void CGauge::Draw()
{
	m_pVS->Bind();
	m_pPS->Bind();
	m_pBuf[0]->Write(m_matrix);
	m_pBuf[0]->BindVS(0);
	m_pBuf[1]->Write(m_param);
	m_pBuf[1]->BindVS(1);
	SetTexturePS(m_pTexture);
	m_pMesh->Draw();
}

void CGauge::SetOffset(DirectX::XMFLOAT2 offset)
{
	m_param[0].x = offset.x;
	m_param[0].y = offset.y;
}
void CGauge::SetSize(DirectX::XMFLOAT2 size)
{
	m_param[0].z = size.x;
	m_param[0].w = size.y;
}

void CGauge::SetUVPos(DirectX::XMFLOAT2 pos)
{
	m_param[1].x = pos.x;
	m_param[1].y = pos.y;
}
void CGauge::SetUVScale(DirectX::XMFLOAT2 scale)
{
	m_param[1].x = scale.x;
	m_param[1].y = scale.y;
}
void CGauge::SetColor(DirectX::XMFLOAT4 color)
{
	m_param[2] = color;
}
void CGauge::SetTexture(ID3D11ShaderResourceView* pTex)
{
	m_pTexture = pTex;
}
void CGauge::SetWorld(DirectX::XMFLOAT4X4 world)
{
	m_matrix[0] = world;
}
void CGauge::SetView(DirectX::XMFLOAT4X4 view)
{
	m_matrix[1] = view;
}
void CGauge::SetProjection(DirectX::XMFLOAT4X4 proj)
{
	m_matrix[2] = proj;
}
void CGauge::SetDefaultVertexShader()
{
	m_pVS = m_pDefVS;
}
void CGauge::SetDefaultPixelShader()
{
	m_pPS = m_pDefPS;
}
void CGauge::SetVertexShader(VertexShader* pVS)
{
	m_pVS = pVS;
}
void CGauge::SetPixelShader(PixelShader* pPS)
{
	m_pPS = pPS;
}
