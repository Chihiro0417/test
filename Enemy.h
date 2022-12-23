#pragma once
#include "CameraBase.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Player.h"

class Enemy
{
public:
	Enemy(static Model* ModelList);
	~Enemy();
	void Update();
	void Draw();
	void Effect();
	void Touch();

	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();
	void Collision(DirectX::XMFLOAT3 pos);
	void Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal, DirectX::XMFLOAT3 out);
	DirectX::XMFLOAT3 GetVector();
	void SetPos(float x, float y, float z);
	//-----�ǉ��֐�----
	//�v���C���[�ɐG���Ă�����bTouch��true�ɂ���	
	void TochtEnemy();
	//�v���C���[�ɐG���Ă��邩�ǂ���
	bool CheckTouch();
	//�G�l�~�[������ł��邩�ǂ���
	bool CheckEnemy();
	//�t���[�����̉��Z
	float GetCurrent();

private:

	DirectX::XMFLOAT3 m_pos;
	Model* m_pModel;
	VertexShader* m_pVS;
	ConstantBuffer* m_pWVP;
	CameraBase* m_pCamera;
	Player* m_pPlayer;

	// �e�̏��
	DirectX::XMFLOAT3 m_shadowPos;
	ID3D11ShaderResourceView* m_pShadowTex;

	//�G�t�F�N�g
	ID3D11ShaderResourceView* m_pEffectTex;
	float m_uvX;	//�X�v���C�g�V�[�g��X���W
	float m_uvY;	//�X�v���C�g�V�[�g��Y���W
	bool isTouch;
	int m_frame;

	float m_CurrentCnt = 0;

	float m_speed;
	DirectX::XMVECTOR vFront;
	DirectX::XMVECTOR vMove;
	//-----�ǉ��֐�----
	//�G�l�~�[���U�����ꂽ���̔���
	bool m_bTouch;
	//�G�l�~�[�������Ă��邩����ł��邩����
	bool m_bEnemy;
	//���f���ǂݍ��݂���Ă��邩
	static bool LoadModel;
	//---------------------
	DirectX::XMFLOAT3 PlayerPos;
};