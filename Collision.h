#pragma once
#include <DirectXMath.h>

class Collision
{
public:
	//--- �����蔻����s���`��
	struct Plane
	{
		DirectX::XMFLOAT3 normal;	// �ʂ̖@��
		DirectX::XMFLOAT3 pos;		// �ʏ�̍��W
	};
	struct Ray
	{
		DirectX::XMFLOAT3 start;	// ���C�̊J�n�_
		DirectX::XMFLOAT3 direction;// ���C�̕���
		float length;				// ���C�̒���
	};

	//--- �����蔻��̌���
	struct Result
	{
		bool hit;
		DirectX::XMFLOAT3 point;
	};

	struct Triangle
	{
		DirectX::XMFLOAT3 p[3];	// �O�p�`�̍��W
	};

	//--- �`�󂲂Ƃ̓����蔻�������
	static Result CheckRayPlane(Ray ray, Plane plane);

	static Result CheckCirclePlane(DirectX::XMFLOAT3 CircleCenter, float r, Plane plane, Collision::Triangle triangle[2]);

	static Result CheckPointTriangle(DirectX::XMFLOAT3 point, Triangle triangle);
private:
	DirectX::XMFLOAT3 CircleCenter;		//���̒��S���W(�v���C���[�̍��W)
	float r;							//���̔��a(�K��)
	Plane plane;						//�ʂ̏��(plane)
	Triangle triangle[2];	//2�̎O�p�`�̒��_(triangle[2])
};