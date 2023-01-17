#pragma once
class Component
{
public:
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float timeStep);
	virtual void Stop();
};

