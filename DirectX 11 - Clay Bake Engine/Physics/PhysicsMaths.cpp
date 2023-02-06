#include "PhysicsMaths.h"

bool PhysicsMaths::AABBvsAABB(AxisAlignedBoundingBox a, AxisAlignedBoundingBox b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.y) 
	{  return false;  };
	if (a.max.y < b.min.y || a.min.y > b.max.y)
	{  return false;  }

	return true;

}

