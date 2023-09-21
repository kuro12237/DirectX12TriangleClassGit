#pragma once
#include"Pch.h"

class Collider {
public:

	float GetRadious() { return radious_; }
	void SetRadious(float radious) { radious_ = radious; }

	virtual Vector3 GetWorldPosition() = 0;
	virtual void OnCollision() = 0;

	uint32_t GetCollosionAttribute() const { return collisionAttribute_; }
	void SetCollosionAttribute(uint32_t collisionAttribute) {collisionAttribute_ = collisionAttribute;}

	uint32_t GetCollisionMask() const { return CollisionMask_; }
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

private:

	//Ž©•ª
	uint32_t collisionAttribute_ = 0xffffffff;
	//‘ŠŽè
	uint32_t CollisionMask_ = 0xffffffff;

	float radious_ = 0.5f;
};
