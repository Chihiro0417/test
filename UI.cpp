#include "UI.h"
#include "Sprite.h"
#include "DirectXTex/Texture.h"

UI::UI()
{
	LoadTextureFromFile("2D/Glass19.png", &m_pBar);
}

UI::~UI()
{
	m_pBar->Release();
}

void UI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];
	// 2D�Ŕz�u����ʒu�����߂�
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(1110.0f, 690.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(T));
	// 2D�ł̓J�������s�v�Ȃ̂ŁA�ω����Ȃ��s���ݒ肷��
	// �ω����Ȃ��s��...���g���S��0���ƁA�|���Z����ƑS��0�ɂȂ�
	// �g��E�k���̕�����1���Œ肳��Ă���s�񂪕K�v
	// �ω����Ȃ��s���"�P�ʍs��"�Ƃ���
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	// �J�������܂��������Ă���(���s���e)�s���ݒ�
	DirectX::XMStoreFloat4x4(&mat[2],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,		// ��ʍ��[�̍��W
				1280.0f,	// ��ʉE�[�̍��W
				720.0f,		// ��ʉ��[�̍��W
				0.0f,		// ��ʏ�[�̍��W
				-1.0f,		// z���W�Ŏʂ���͈͂̎�O
				1.0f		// z���W�Ŏʂ���͈͂̉���
			)));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -20.0f));
	Sprite::SetTexture(m_pBar);
	Sprite::Draw();
}