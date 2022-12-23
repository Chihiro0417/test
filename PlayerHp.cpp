////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PlayerHp.cpp
//		
//		�X�V����	�X�V�����l	�X�V���e
//		----------------------------------
//		2022/12/13	�X�{		PlayerHp.h/cpp�쐬
//		2022/12/14�@�X�{		�R�����g�̒ǉ�
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

#include "PlayerHp.h"
#include "Gauge.h"
#include "DirectXTex/Texture.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHp.cpp:�R���X�g���N�^
//
//	�N���X���FCPlayerHp
//
//	�ϐ���			�^��						�T�v
//	m_fPlayerHpPos	Float3						HP�o�[�̈ʒu
//	m_fHp			float						HP�o�[�̒���
//	m_fDamage		float						�_���[�W�ʁi����ɂ����HP�o�[�̌�������ς��j
//	m_nHeal			int							�񕜂���܂ł̃t���[���̃J�E���g
//	m_nFrame		float						�t���[���̃J�E���g
//	m_bInv			bool						���G���Ԃ̏����p
//	m_bInvTime		int							���G����
//	m_pBar			ID3D11ShaderResourceView	HP�o�[�̃e�N�X�`��
//	��HP�o�[�ɓ\��e�N�X�`���͏㉺���E���������葵���Ă���摜�łȂ��ƃ_���[�W���󂯂��ۂɃY���Ă����Ă��܂�
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerHp::CPlayerHp()
{
	m_fPlayerHpPos = DirectX::XMFLOAT3(70.0f, 30.0f, 0.0f);
	m_fHp = 200.0f;
	m_fDamage = 20;
	m_nHeal = 0;
	m_nFrame = 0;
	m_bInv = false;
	m_nInvTime = 120;
	LoadTextureFromFile("Assets/HpBar.png", &m_pBar);
}

CPlayerHp::~CPlayerHp()
{
	m_pBar->Release();
}

void CPlayerHp::Draw()
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
	CGauge::SetSize(DirectX::XMFLOAT2(m_fHp, -20.0f));
	CGauge::SetTexture(m_pBar);
	CGauge::Draw();
}

void CPlayerHp::Update()
{
	// �񕜊֘A�i�s���S�j
	m_nHeal++;
	if (m_fHp < 200)
	{
		if (m_nHeal > 300)
		{
			m_fHp += 10;
			CGauge::Draw();
			m_nHeal = 0;
		}
	}
	else
	{
		m_nHeal = 0;
	}


	// ���G���Ԋ֘A
	if (m_bInv == true)
		m_nFrame++;

	if (m_nFrame > 120)
	{
		m_bInv = false;
		m_nFrame = 0;
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHp.cpp�FPlayerDamage�֐�
//
//	class�FCPlayerHp
//
//	�֐���			����						
//	SetSize			Float2�i HP�o�[�̉��̒��� ,  HP�o�[�̏c�̒��� �j
//	
//	�E�v���C���[���_���[�W���󂯂�֐��ɂȂ�܂�
//	�EHP��0�ɂȂ��������͂܂������Ă��܂���(������炱�̃R�����g�������Ă�������)
//	
//	�g�p�ꏊ
//	�EGameUI.cpp��Damage�֐���
//	
//	
//////////////////////////////////////////////////////////////////////////////////////
void CPlayerHp::PlayerDamage()
{
	if (!m_bInv == true)
	{
		m_fHp -= m_fDamage;
		CGauge::SetSize(DirectX::XMFLOAT2(m_fHp, -20.0f));
		CGauge::Draw();
		m_bInv = true;
	}
}

