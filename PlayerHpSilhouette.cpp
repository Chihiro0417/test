////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PlayerHpSilhouette.cpp
//		
//		�X�V����	�X�V�����l	�X�V���e
//		----------------------------------
//		2022/12/14	�X�{		PlayerHpPlayerHpSilhouette.h/cpp�쐬
//		
//		
//		
//		���e
//		�E�v���C���[��HP�o�[�֘A�̃v���O�����ł�
//
//
//		���ӓ_
//		�E����cpp�̊֐���GameUI.cpp�Ŋ�{�I�ɌĂт����Ă�������
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PlayerHpSilhouette.h"
#include "Gauge.h"
#include "DirectXTex/Texture.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHpSilhuette.cpp:�R���X�g���N�^
//
//	�N���X���FCPlayerHpSilhouette
//
//	�ϐ���			�^��						�T�v
//	m_fPlayerHpPos	Float3						HP�o�[�̔w�i�̈ʒu
//	m_fMaxHp		float						HP�o�[�̔w�i�̒���
//	m_pBar			ID3D11ShaderResourceView	HP�o�[�̔w�i�̃e�N�X�`��
//
//	���ӓ_
//	�EHP�o�[�ɓ\��e�N�X�`���͏㉺���E���������葵���Ă���摜�łȂ��ƃ_���[�W���󂯂��ۂɃY���Ă����Ă��܂�
//	�EPlayerHp�Ɠ��������A�ʒu�ɂ��ăe�N�X�`���͈Ⴄ�F�̂��̂��g�p���Ă�������
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerHpSilhouette::CPlayerHpSilhouette()
{
	m_fPlayerHpPos = DirectX::XMFLOAT3(70.0f, 30.0f, 0.0f);
	m_fMaxHp = 200.0f;
	LoadTextureFromFile("Assets/HpBarSilhouette.png", &m_pBar);
}

CPlayerHpSilhouette::~CPlayerHpSilhouette()
{
	m_pBar->Release();
}

void CPlayerHpSilhouette::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];;
	DirectX::XMMATRIX T =
		DirectX::XMMatrixTranslation(m_fPlayerHpPos.x, m_fPlayerHpPos.y, m_fPlayerHpPos.z);
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(T));
	DirectX::XMStoreFloat4x4(&mat[1],
		DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,		// ��ʍ��[�̍��W
				1280.0f,	// ��ʉE�[�̍��W
				720.0f,		// ��ʉ��[�̍��W
				0.0f,		// ��ʏ�[�̍��W
				-1.0f,		// �y���W�Ŏʂ���͈͂̎�O
				1.0f		// �y���W�Ŏʂ���͈͂̉�
			)));

	CGauge::SetWorld(mat[0]);
	CGauge::SetView(mat[1]);
	CGauge::SetProjection(mat[2]);
	CGauge::SetSize(DirectX::XMFLOAT2(m_fMaxHp, -20.0f));
	CGauge::SetTexture(m_pBar);
	CGauge::Draw();
}
