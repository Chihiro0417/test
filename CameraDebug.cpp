#include "CameraDebug.h"

CameraDebug::CameraDebug()
	:m_radY(0.0f)
	, m_radXZ(0.0f)
	, m_radius(5.0f)
{

}

CameraDebug::~CameraDebug()
{

}

 void CameraDebug::Update()
 {
 //// ���A���A���A���AShift�ACtrl�ŃJ�����̒����_�𓮂���
 //if (IsKeyPress(VK_UP)) m_look.z += 0.01f;
 //if (IsKeyPress(VK_DOWN)) m_look.z -= 0.01f;
 //if (IsKeyPress(VK_RIGHT)) m_look.x += 0.01f;
 //if (IsKeyPress(VK_LEFT)) m_look.x -= 0.01f;
 //if (IsKeyPress(VK_SHIFT)) m_look.y += 0.01f;
 //if (IsKeyPress(VK_CONTROL)) m_look.y -= 0.01f;
 // W,A,S,D,Q,E�ŃJ��������]������
 if (IsKeyPress('W'))	m_radY += 0.1f;
 if (IsKeyPress('A'))	m_radXZ -= 0.1f;
 if (IsKeyPress('S'))	m_radY -= 0.1f;
 if (IsKeyPress('D'))	m_radXZ += 0.1f;
 // Q,E�ŃJ�����ƒ����_�̋�����ς���
 if (IsKeyPress('Q'))	m_radius += 0.1f;
 if (IsKeyPress('E'))	m_radius -= 0.1f;

 // �p�x�Ƌ����A�����_����J�����̈ʒu���v�Z
 m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
 m_pos.y = sin(m_radY) * m_radius + m_look.y;
 m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}