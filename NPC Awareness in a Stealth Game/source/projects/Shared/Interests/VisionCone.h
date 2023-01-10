#pragma once
#include "framework/EliteMath/EMath.h"
class VisionCone
{
public:
	VisionCone(const float rotation, const Elite::Vector2& origin, const float fovAngle = 60.f, const float distance = 20.f);
	~VisionCone();

	void UpdatePos(const float rotation, const Elite::Vector2& origin);
	void Render() const;
	bool IsOverlapping(Elite::Vector2 pos);
private:
	float m_Rotation{};
	Elite::Vector2 m_Origin{};
	const float m_FOVAngle{};
	const float m_Distance{};
	Elite::Triangle m_VisionCone{};
};

