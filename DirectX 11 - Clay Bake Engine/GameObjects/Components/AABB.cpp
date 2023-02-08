#include "AABB.h"
#include "Rigidbody.h"
#include "../GameObject.h"

AABB::AABB(float width, float height)
{
	_width = width;
	_height = height;
}

bool AABB::Overlaps(AABB* collider, float deltaTime)
{
	DirectX::BoundingBox me;
	me.Center.x = _gameObject->GetTransform()->GetPosition().x;
	me.Center.y = _gameObject->GetTransform()->GetPosition().y;
	me.Center.z = 0.0f;
	me.Extents.x = _width / 2;
	me.Extents.y = _height / 2;
	me.Extents.z = 0.5f;

	DirectX::BoundingBox other;
	other.Center.x = collider->GetGameObject()->GetTransform()->GetPosition().x;
	other.Center.y = collider->GetGameObject()->GetTransform()->GetPosition().y;
	other.Center.z = 0.0f;
	other.Extents.x = collider->_width / 2;
	other.Extents.y = collider->_height / 2;
	other.Extents.z = 0.5f;

	return me.Intersects(other);
}

Vector2 AABB::GetSize()
{
	return Vector2(_width, _height);
}
