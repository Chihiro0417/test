#pragma once
#include "CameraBase.h"
#include "Enemy.h"

class CameraEnemy : public CameraBase
{
public:
	CameraEnemy(Enemy* pEnemy);
	~CameraEnemy();
	void Update();
private:
	Enemy* m_pEnemy;
	float m_radY;
	float m_radXZ;
	float m_radius;
	float m_lateXZ;
	float m_lateY;	// 目的位置に対して遅れてついていく角度(現在の角度
};