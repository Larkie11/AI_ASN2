#pragma once
#include "Vector3.h"
#include "../GenericEntity.h"
#include "MatrixStack.h"


#include <vector>
using std::vector;
struct Moving : public GenericEntity
{
	typedef enum Movement
	{
		IDLE,
		MOVE,
		CHASE,
		END,
	};
	Vector3 position;
	vector<Vector3>WayPoints;
	Vector3 lastsavedposition;
	Vector3 nextPoint;
	int wayPointID;
	bool changepos;
	bool arrived = false;
	Movement move;
	void Init();
	void Update(double dt, Vector3 playerpos);
	void Render();
	Vector3 GetPosition();
	Moving();
	~Moving();
};