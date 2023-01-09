#include "stdafx.h"
#include "VisionCone.h"

using namespace Elite;

VisionCone::VisionCone(const float rotation, const Elite::Vector2& origin, const float fovAngle, const float distance)
	:m_Rotation{rotation}
	,m_Origin{origin}
	,m_FOVAngle{fovAngle}
	,m_Distance{distance}
	,m_VisionCone{}
{
	
}

VisionCone::~VisionCone()
{
}

void VisionCone::UpdatePos(const float rotation, const Elite::Vector2& origin)
{
	m_Rotation = rotation;
	m_Origin = origin;
	const float lowerAngle{ m_Rotation - ToRadians(m_FOVAngle) / 2.f };
	const float upperAngle{ m_Rotation + ToRadians(m_FOVAngle) / 2.f };

	Vector2 lowerSide{ cosf(lowerAngle) * m_Distance,sinf(lowerAngle) * m_Distance };
	Vector2 upperSide{ cosf(upperAngle) * m_Distance,sinf(upperAngle) * m_Distance };

	m_VisionCone.p1 = m_Origin;
	m_VisionCone.p2 = m_Origin + lowerSide;
	m_VisionCone.p3 = m_Origin + upperSide;
}

void VisionCone::Render() const
{
	DEBUGRENDERER2D->DrawSegment(m_VisionCone.p1, m_VisionCone.p2,Color{0,0,0});
	DEBUGRENDERER2D->DrawSegment(m_VisionCone.p2, m_VisionCone.p3, Color{ 0,0,0 });
	DEBUGRENDERER2D->DrawSegment(m_VisionCone.p3, m_VisionCone.p1, Color{ 0,0,0 });
}

bool VisionCone::IsOverlapping(Elite::Vector2 pos)
{
	const float triangleArea{ Area(m_VisionCone.p1,m_VisionCone.p2,m_VisionCone.p3)};
	const float area1{ Area(pos,m_VisionCone.p1,m_VisionCone.p2) };
	const float area2{ Area(pos,m_VisionCone.p2,m_VisionCone.p3) };
	const float area3{ Area(pos,m_VisionCone.p3,m_VisionCone.p1) };

	return triangleArea == area1 + area2 + area3;
}
