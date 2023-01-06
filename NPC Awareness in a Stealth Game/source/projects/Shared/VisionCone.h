#pragma once
#include "framework/EliteMath/EMath.h"
class VisionCone
{
public:
	VisionCone(const Elite::Vector2& direction, const Elite::Vector2& origin, const float fovAngle = 45.f, const float distance = 10.f);
	~VisionCone();

	void UpdatePos(const Elite::Vector2& direction, const Elite::Vector2& origin);
	void Render() const;
	bool IsOverlapping(const Elite::Vector2& pos);
private:
	Elite::Vector2 m_Direction{};
	Elite::Vector2 m_Origin{};
	const float m_FOVAngle{};
	const float m_Distance{};
	
};

