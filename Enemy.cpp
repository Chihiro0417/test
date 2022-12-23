#include "Enemy.h"
#include "Input.h"
#include <DirectXMath.h>
#include <DirectXTex/Texture.h>
#include "Sprite.h"

DirectX::XMFLOAT4X4 mat[3];
Enemy::Enemy(static Model* ModelList)
	: m_pos(4.0f, 0.5f, 1.0f)
	, PlayerPos(m_pos)
	, m_speed(0.0f)
	, m_pCamera(NULL)
	, m_bTouch(false)
	, m_bEnemy(false)
{
	//	if (LoadModel == false)
	//	{
	//		g_pModel = new Model();
	//		g_pModel->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);
	//
	//		LoadModel = true;
	//	}5
	isTouch = false;
	m_frame = 0;
	m_pModel = ModelList;


	m_pVS = new VertexShader;
	if (FAILED(m_pVS->Load("Assets/Shader/ModelVS.cso")))
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);

	m_pModel->SetVertexShader(m_pVS);


	m_pWVP = new ConstantBuffer();//���f����\�����邽�߂�
	m_pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);

	// �e�̏�����
	LoadTextureFromFile("2D/shadow.png", &m_pShadowTex);
	m_shadowPos = m_pos;


	LoadTextureFromFile("2D/explosion.png",
		&m_pEffectTex);

	m_uvX = 0.25f;
	m_uvY = 0.25f;



}

Enemy::~Enemy()
{
	m_pEffectTex->Release();
	m_pShadowTex->Release();
	delete m_pWVP;
	delete m_pVS;
	//delete m_pModel; //ModelList��delete
}

void Enemy::Update()
{
	vMove = DirectX::XMVectorZero();
	vMove = DirectX::XMVectorAdd(vMove, vFront);
	//�v���C���[�ɐG���Ă��Ȃ�������
	if (m_speed > 0.0f)
	{
		m_speed = m_speed - 0.001f;
	}
	else
	{
		m_bEnemy = true;
		m_speed = 0.0f;
	}
	vMove = DirectX::XMVectorScale(vMove, m_speed);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);

	m_pos.x += move.x;
	//m_pos.y += move.y;
	m_pos.z += move.z;

	m_shadowPos = m_pos;

	for (int i = 0; i < 3; i++)
	{
		m_uvX += 0.25f;
		if (m_uvX == 1.0f)
		{
			m_uvX = 0.25f;
			m_uvY += 0.25f;
		}
		if (m_uvY == 1.0f)
		{
			m_uvY = 0.25f;
		}
	}
	m_CurrentCnt++;
}

void Enemy::Draw()
{
	if (!m_pCamera)
	{
		return;
	}

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	m_pWVP->Write(mat);
	m_pWVP->BindVS(0);
	m_pModel->Draw();
	if (isTouch == true && m_frame < 20)
	{
		Effect();
		m_frame++;
	}

	
}

void Enemy::Effect()
{
	// �e�̕`��
	//DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation
	(
		m_shadowPos.x, m_shadowPos.y, m_shadowPos.z
	);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationX
	(
		DirectX::XMConvertToRadians(90.0f)
	);
	DirectX::XMStoreFloat4x4(&mat[0], // world�s��
		DirectX::XMMatrixTranspose(R * T));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	Sprite::SetSize(DirectX::XMFLOAT2(0.3f, 0.3f));
	Sprite::SetTexture(m_pShadowTex);

	Sprite::Draw();

	// �o�[�̕\��
	// �J�����̉�]��ł������s��(�t�s��)�����߂�
	DirectX::XMFLOAT4X4 fInv;
	DirectX::XMMATRIX matInv;
	fInv = m_pCamera->GetViewMatrix();
	matInv = DirectX::XMLoadFloat4x4(&fInv);
	// �]�u�s�񂩂��x���ɖ߂�
	matInv = DirectX::XMMatrixTranspose(matInv);
	// �J�����̉�]������ł����������̂ŁA
	// �J�����̈ړ�������0(�ړ����Ȃ��悤)�ɂ���
	// �������A�s��̒��g�𒼐ڕς���ꍇ��FLOAT4x4��
	// �Ȃ��Ƃ����Ȃ��̂ŁAMATRIX����FLOAT4x4�ɕϊ�����
	DirectX::XMStoreFloat4x4(&fInv, matInv);
	fInv._41 = fInv._42 = fInv._43 = 0.0f;
	// �t�s��ւ̕Ԋ҂�MATRIX�^�ōs���̂ł܂��ϊ�...
	matInv = DirectX::XMLoadFloat4x4(&fInv);
	matInv = DirectX::XMMatrixInverse(nullptr, matInv);

	// �o�[��z�u����s��(���[���h)�����߂�
	T = DirectX::XMMatrixTranslation(
		m_pos.x, m_pos.y + 1.0f, m_pos.z
	);
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(matInv * T));
	Sprite::SetWorld(mat[0]);
	// View��Projection�͉e��\�����鎞�ɐݒ肵���̂ō���͐ݒ肵�Ȃ�
	Sprite::SetUVPos({ -0.25f, -0.25f }); //�X�v���C�g�V�[�g���S�������Ă���
	Sprite::SetUVScale({ m_uvX, m_uvY }); //�X�v���C�g�V�[�g�̕\������ꏊ
	Sprite::SetSize(DirectX::XMFLOAT2(4.0f, 4.0f));
	Sprite::SetTexture(m_pEffectTex);

	EnableDepth(false);
	Sprite::Draw();
	EnableDepth(true);


}

void Enemy::Touch()
{
	isTouch = true;
}

void Enemy::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Enemy::GetPos()
{
	DirectX::XMFLOAT3 pos = m_pos;
	pos.y += 0.1f;
	return pos;
}

void Enemy::Collision(DirectX::XMFLOAT3 pos)
{


	//�G�ƃv���C���[�̍��W�������Z����
	//�G���O���ɐ�����ԃx�N�g��
	PlayerPos = pos;
	DirectX::XMFLOAT3 EnemyPos = m_pos;
	PlayerPos.y = 0.0f;
	EnemyPos.y = 0.0f;
	DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);
	DirectX::XMVECTOR vEnemyPos = DirectX::XMLoadFloat3(&EnemyPos);

	vFront = DirectX::XMVectorSubtract(vEnemyPos, vPlayerPos);

	//���K��
	vFront = DirectX::XMVector3Normalize(vFront);

	//������ԏ����̃X�s�[�h
	m_speed = 0.6f;
	//�G�l�~�[����������false�ɂ���
	m_bEnemy = false;
}

void Enemy::Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal, DirectX::XMFLOAT3 out)
{
	if (planenormal.y > 0.5f)
	{
		m_pos.y = hitpoint.y;
	}
	else
	{
		m_pos.y = hitpoint.y - 0.1f;
		m_pos.x = hitpoint.x;
		m_pos.z = hitpoint.z;
	}

	vFront = DirectX::XMLoadFloat3(&out);
}

DirectX::XMFLOAT3 Enemy::GetVector()
{
	DirectX::XMFLOAT3 Vector;
	DirectX::XMStoreFloat3(&Vector, vFront);
	return Vector;
}
void Enemy::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

//-----�ǉ��֐�----
void Enemy::TochtEnemy()
{
	m_bTouch = true;
}

bool Enemy::CheckTouch()
{
	return m_bTouch;
}


bool Enemy::CheckEnemy()
{
	return m_bEnemy;
}

float Enemy::GetCurrent()
{
	return m_CurrentCnt;
}


