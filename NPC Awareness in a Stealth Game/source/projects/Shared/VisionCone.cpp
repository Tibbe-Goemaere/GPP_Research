#include "stdafx.h"
#include "VisionCone.h"

using namespace Elite;

VisionCone::VisionCone(const Elite::Vector2& direction, const Elite::Vector2& origin, const float fovAngle, const float distance)
{
}

VisionCone::~VisionCone()
{
}

void VisionCone::UpdatePos(const Elite::Vector2& direction, const Elite::Vector2& origin)
{
	m_Direction = direction;
	m_Origin = origin;
}

void VisionCone::Render() const
{
	Vector2 upperSide{ m_Direction.x + cosf(m_FOVAngle), m_Direction.y + sinf(m_FOVAngle) };
	Vector2 lowerSide{ m_Direction.x - cosf(m_FOVAngle), m_Direction.y - sinf(m_FOVAngle) };

	DEBUGRENDERER2D->DrawDirection(m_Origin, lowerSide, m_Distance, Color{ 0,0,0 });
	DEBUGRENDERER2D->DrawDirection(m_Origin, upperSide, m_Distance, Color{ 0,0,0 });
}

bool VisionCone::IsOverlapping(const Elite::Vector2& pos)
{
	return false;
}
