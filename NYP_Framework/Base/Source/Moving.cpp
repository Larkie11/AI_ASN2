#include "Moving.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include <iostream>
#include "MeshBuilder.h"
Moving::Moving() :GenericEntity(NULL)
{

}
Moving::~Moving()
{

}
void Moving::Init()
{
	arrived = false;
	changepos = false;
	move = MOVE;
	wayPointID = 1;
	scale.Set(5, 5, 5);
	position.Set(300,0,300);
	nextPoint.SetZero();
	lastsavedposition.SetZero();
	this->SetCollider(false);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
}
void Moving::Update(double dt, Vector3 playerpos)
{
	move = MOVE;

	if ((playerpos - position).LengthSquared() > 2000.f)
	{
		if (!changepos)
		{
			changepos = true;
			position = WayPoints[0];
			wayPointID = 1;
		}

		nextPoint = WayPoints[wayPointID];

		Vector3 direction2;
		if (nextPoint != position)
		{
			direction2 = (nextPoint - position).Normalize();
		}
		else
		{
			changepos = false;
		}
		float distance2 = (nextPoint - position).LengthSquared();

		if (distance2 < 0.5)
		{
			position = nextPoint;
			arrived = true;
		}
		else
			position = position + direction2* 0.7;

		if (arrived)
		{
			if (wayPointID < WayPoints.size() - 1)
			{
				wayPointID++;
				arrived = false;
			}
			else
				wayPointID = 0;
		}
	}
	else
	{
		Vector3 dir3;
		dir3 = (playerpos - position).Normalize();
		position = position + dir3* 0.7;
	}
}
void Moving::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("robot1_mid"));

	modelStack.PopMatrix();
}