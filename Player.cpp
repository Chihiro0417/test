#include "Player.h"
#include "Input.h"
#include <DirectXMath.h>
#include <DirectXTex/Texture.h>
#include "Sprite.h"

Player::Player(static Model* ModelList)
	:m_pos(1.0f, 0.5f, 1.0f)
	, m_gravity(-0.04f)
	, m_acc(0.0f)
	, m_CanJump(true)
	, m_pCamera(NULL)
{
	//m_pModel = new Model();
	//m_pModel->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

	m_pModel = ModelList;
	m_pVS = new VertexShader;
	if (FAILED(m_pVS->Load("Assets/Shader/ModelVS.cso")))
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
	m_pModel->SetVertexShader(m_pVS);

	// �萔�o�b�t�@�쐬
	m_pWVP = new ConstantBuffer();
	m_pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);

	// �e�̏�����
	LoadTextureFromFile("2D/shadow.png", &m_pShadowTex);
	m_shadowPos = m_pos;

	// �o�[�̏�����
	LoadTextureFromFile("2D/Glass19.png", &m_pBarTex);

	// �O�Ղ̏�����
	m_pPolyline = new GeometoryPolyline(15);
}

Player::~Player()
{
	delete m_pPolyline;
	m_pBarTex->Release();
	m_pShadowTex->Release();
	delete m_pWVP;
	delete m_pVS;
	//	delete m_pModel;//	ModelList��delete
}

void Player::Update()
{
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
	DirectX::XMFLOAT3 camLook = m_pCamera->GetLook();
	camPos.y = 0.0f;
	camLook.y = 0.0f;
	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);

	DirectX::XMVECTOR vFront;
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	vFront = DirectX::XMVector3Normalize(vFront);

	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationY((90 * 3.14f) / 180.0f);
	DirectX::XMVECTOR vSide = DirectX::XMVector3TransformCoord(vFront, matRotSide);

	DirectX::XMVECTOR vMove = DirectX::XMVectorZero();
	if (IsKeyPress('W'))	vMove = DirectX::XMVectorAdd(vMove, vFront);
	if (IsKeyPress('A'))	vMove = DirectX::XMVectorSubtract(vMove, vSide);
	if (IsKeyPress('S'))	vMove = DirectX::XMVectorSubtract(vMove, vFront);
	if (IsKeyPress('D'))	vMove = DirectX::XMVectorAdd(vMove, vSide);
	vMove = DirectX::XMVectorScale(vMove, 0.1f);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);

	if (m_CanJump && IsKeyTrigger(VK_SPACE))
	{
		m_CanJump = false;
		m_acc = 0.4f;
	}

	m_acc += m_gravity;
	if (m_acc < -0.25f)
	{
		m_acc = -0.25f;
	}
	move.y += m_acc;//

	m_pos.x += move.x;
	m_pos.y += move.y;
	m_pos.z += move.z;

	// �e�̍X�V
	// �{���͓����蔻����g����
	// �n�ʂ̉ӏ��ɔz�u�ł����ق����悢
	m_shadowPos = m_pos;
	//m_shadowPos.y = 0.0f;
}

void Player::Draw()
{
	if (!m_pCamera)
	{
		return;
	}
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	m_pWVP->Write(mat);
	m_pWVP->BindVS(0);
	m_pModel->Draw();

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
		m_pos.x, m_pos.y + 1.7f, m_pos.z
	);
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(matInv * T));
	Sprite::SetWorld(mat[0]);
	// View��Projection�͉e��\�����鎞�ɐݒ肵���̂ō���͐ݒ肵�Ȃ�
	Sprite::SetSize(DirectX::XMFLOAT2(0.6f, 0.04f));
	Sprite::SetTexture(m_pBarTex);
	EnableDepth(false);
	Sprite::Draw();
	EnableDepth(true);

	// �O�Ղ̒ǉ�
	GeometoryPolyline::Point point = {};
	point.pos = m_pos;
	point.width = 0.7f;
	// �ݒu�����ʒu����J�����Ɍ������x�N�g�����|�����C���̖@���ƂȂ�
	DirectX::XMVECTOR vCamPos =
		DirectX::XMLoadFloat3(&m_pCamera->GetPos());
	DirectX::XMVECTOR vPos =
		DirectX::XMLoadFloat3(&m_pos);
	DirectX::XMStoreFloat3(&point.normal,
		DirectX::XMVectorSubtract(vCamPos, vPos));
	m_pPolyline->PushPoint(point);
	// �O�Ղ̕\��
	m_pPolyline->SetView(m_pCamera->GetViewMatrix());
	m_pPolyline->SetProjection(m_pCamera->GetProjectionMatrix());
	m_pPolyline->Draw();
}

void Player::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Player::GetPos()
{
	DirectX::XMFLOAT3 pos = m_pos;
	pos.y += 0.2f;
	return pos;
}

void Player::Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal)
{
	if (planenormal.y > 0.5f)
	{
		m_pos.y = hitpoint.y;
	}
	else
	{
		m_pos.y = hitpoint.y - 0.2f;
		m_pos.x = hitpoint.x;
		m_pos.z = hitpoint.z;
	}

	m_acc = 0.0f;
	m_CanJump = true;
}