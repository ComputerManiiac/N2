#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H


#include "System.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "Entity.h"
#include <map>
#include <vector>
#include <algorithm>

struct CollisionHit {

	CollisionHit() : rigidbody(nullptr), transform(nullptr), collider(nullptr), mtvAxis(0, 0, 0), mtvAmount(0.0f){}

	CollisionHit(ColliderComponent* collider, Vector3 mtvAxis, float mtvAmount) : collider(collider), mtvAxis(mtvAxis), mtvAmount(mtvAmount)
	{
		Entity* entity = collider->getParent();
		rigidbody = entity->getComponent<RigidbodyComponent>();
		transform = entity->getComponent<TransformComponent>();
	}

	void setValues(ColliderComponent* collider, Vector3 mtvAxis, float mtvAmount)
	{
		this->collider = collider;
		Entity* entity = collider->getParent();
		rigidbody = entity->getComponent<RigidbodyComponent>();
		transform = entity->getComponent<TransformComponent>();

		this->mtvAxis = mtvAxis;
		this->mtvAmount = mtvAmount;
	}

	RigidbodyComponent* rigidbody;
	TransformComponent* transform;
	ColliderComponent* collider;
	Vector3 mtvAxis;
	float mtvAmount;
};

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Initialize();
	void Update(double& dt);

	void registerComp(Component* component);
	void removeComp(Component* component);

	/* Intersection */
	bool queryCollision(ColliderComponent* collider, std::vector<ColliderComponent*>& nearby, const Vector3& translation, const Vector3& rotation, CollisionHit& hit);
	void getCollisionData(ColliderComponent* collider, ColliderComponent* other, const Vector3& translation, const Vector3& rotation, CollisionHit& hit);


	bool checkCollisionFast(ColliderComponent* collider, ColliderComponent* other, const Vector3& translation);
	
	void resolveCollisions(ColliderComponent* collider, ColliderComponent* other, CollisionHit& hit, double dt);

private:

	std::map<unsigned int, std::map<unsigned int, CollisionHit*>> collisionLinks;
	std::vector<CollisionHit> collisions;


	std::vector<ColliderComponent*> getNearbyColliders(ColliderComponent* self);

	/* Separating Axis Theorem */
	bool checkSeparatingPlaneExists(const Vector3& pos, const Vector3& planeAxis, TransformComponent* transform, TransformComponent* other, Vector3* transformLocal, Vector3* otherLocal);
	void getProjOverlap(ColliderComponent* collider, const Vector3& translaation, const Vector3& rotation, const Vector3& axis, float& min, float& max);
	float getLineOverlap(float& aMin, float& aMax, float& bMin, float& bMax);
	void generateAxes(TransformComponent* collider, TransformComponent* other, std::vector<Vector3>& axes);

	std::vector<RigidbodyComponent*> subscribers;
	Vector3 gravity;
	CollisionHit hit;
};

#endif