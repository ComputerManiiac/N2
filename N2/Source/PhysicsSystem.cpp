#include "PhysicsSystem.h"
#include "Entity.h"


PhysicsSystem::PhysicsSystem()
{
	
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Initialize()
{
	
}

void PhysicsSystem::Update(double& dt)
{
	gravity = Vector3(0, -9.8f, 0);
	

	for (RigidbodyComponent* rigidbody : subscribers)
	{
		const std::string& name = rigidbody->getParent()->getName();
		if (name == "ground") continue;
	

		ColliderComponent* collider = rigidbody->getParent()->getComponent<ColliderComponent>();
		TransformComponent* transform = collider->getParent()->getComponent<TransformComponent>();
		std::vector<ColliderComponent*> nearby = getNearbyColliders(collider);

		Vector3 velocity = rigidbody->getVelocity();

		if (!queryCollision(collider, nearby, Vector3(0, 0, 0), Vector3(0, 0, 0), hit))
		{
			transform->Move(velocity * dt);
		}
		else
		{

			/*if (name == "obj1")
				resolveCollisions(collider, hit.collider, hit, dt);*/
			//std::cout << name << " with " << hit.rigidbody->getParent()->getName() << std::endl;
			const unsigned int& subID = rigidbody->getID();
			const unsigned int& otherID = hit.rigidbody->getID();

			// Outer is the smaller ID of the two
			const unsigned int& outer = (subID < otherID) ? subID : otherID;
			// Inner is the larger ID of the two
			const unsigned int& inner = (subID > otherID) ? subID : otherID;

			bool isNewCollision = false;

			// Search the lower ID for existing collision
			if (collisionLinks.find(outer) == collisionLinks.end())
			{
				isNewCollision = true;
				collisions.push_back(hit);
				collisionLinks[outer][inner] = &collisions.at(collisions.size() - 1);
			}
			else
			{
				// Search for the higher ID within the list of the lower ID's
				std::map<unsigned int, CollisionHit*>& innerList = collisionLinks[outer];
				if (innerList.find(inner) == innerList.end())
				{
					isNewCollision = true;
					collisions.push_back(hit);
					innerList[inner] = &collisions.at(collisions.size() - 1);
				}

			}

			if (isNewCollision)
				resolveCollisions(collider, hit.collider, hit, dt);
		}

		collisions.clear();
		collisionLinks.clear();


		
	}





}



bool PhysicsSystem::queryCollision(ColliderComponent * collider, std::vector<ColliderComponent*>& nearby, const Vector3 & translation, const Vector3 & rotation, CollisionHit& hit)
{
	for (ColliderComponent* near : nearby)
	{
		if (checkCollisionFast(collider, near, translation)){
			getCollisionData(collider, near, translation, rotation, hit);
			return true;
		}
	}
	return false;
}



void PhysicsSystem::getCollisionData(ColliderComponent* collider, ColliderComponent* other, const Vector3& translation, const Vector3& rotation, CollisionHit& hit)
{
	TransformComponent* t1 = collider->getParent()->getComponent<TransformComponent>();
	TransformComponent* t2 = other->getParent()->getComponent<TransformComponent>();

	std::vector<Vector3> axes;
	generateAxes(t1, t2, axes);

	bool hasCalculated = false;
	float minOverlap = 0.0f;
	Vector3 mtv;

	for (const Vector3& axis : axes)
	{

		float minA, maxA, minB, maxB;
		getProjOverlap(collider, translation, rotation, axis, minA, maxA);
		getProjOverlap(other, Vector3(0,0,0), Vector3(0,0,0), axis, minB, maxB);

		float overlap = getLineOverlap(minA, maxA, minB, maxB);

		if (overlap == 0) return;

		if (!hasCalculated || overlap < minOverlap)
		{
			hasCalculated = true;
			minOverlap = overlap;
			mtv = axis;
		}
	}
	 


	Vector3 dir = t2->getPos() - t1->getPos();
	if (mtv.Dot(dir) >= 0)
		mtv = -mtv;


	hit.setValues(other, mtv, minOverlap);
	//return CollisionHit(other, mtv, minOverlap);
}

bool PhysicsSystem::checkCollisionFast(ColliderComponent* collider, ColliderComponent* other, const Vector3& translation)
{
	

	TransformComponent* t1 = collider->getParent()->getComponent<TransformComponent>();
	TransformComponent* t2 = other->getParent()->getComponent<TransformComponent>();

	const Vector3& pos = t2->getPos() - (t1->getPos() + translation);

	Vector3* transformLocal = t1->getLocalAxes();
	Vector3* otherLocal = t2->getLocalAxes();



	return !(checkSeparatingPlaneExists(pos, *(transformLocal), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, *(transformLocal + 1), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, *(transformLocal + 2), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, *(otherLocal), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, *(otherLocal + 1), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, *(otherLocal + 2), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*transformLocal).Cross(*otherLocal), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*transformLocal).Cross(*(otherLocal + 1)), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*transformLocal).Cross(*(otherLocal + 2)), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 1)).Cross(*otherLocal), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 1)).Cross(*(otherLocal + 1)), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 1)).Cross(*(otherLocal + 2)), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 2)).Cross(*otherLocal), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 2)).Cross(*(otherLocal + 1)), t1, t2, transformLocal, otherLocal) ||
		checkSeparatingPlaneExists(pos, (*(transformLocal + 2)).Cross(*(otherLocal + 2)), t1, t2, transformLocal, otherLocal));

}

void PhysicsSystem::resolveCollisions(ColliderComponent* collider, ColliderComponent* other, CollisionHit& hit, double dt)
{
	RigidbodyComponent* rigidbody = collider->getParent()->getComponent<RigidbodyComponent>();
	TransformComponent* transform = collider->getParent()->getComponent<TransformComponent>();


	std::vector<ColliderComponent*> nearby = getNearbyColliders(collider);

	Vector3 velocity = rigidbody->getVelocity();
	Vector3 relative = velocity - hit.rigidbody->getVelocity();
	float velAlongNormal = relative.Dot(hit.mtvAxis);

	if (velAlongNormal < 0)
	{
		float e = Math::Min(1.0f, 1.0f);
		float j = -(1 + e) * velAlongNormal;
		j /= rigidbody->getInverseMass() + hit.rigidbody->getInverseMass();
		Vector3 impulse = j * hit.mtvAxis;

		// Increment by new velocity, derived from p = mv, therefore V = p * (1/m)
		velocity += rigidbody->getInverseMass() * impulse;
		Vector3 otherVelocity = hit.rigidbody->getVelocity();
		otherVelocity -= hit.rigidbody->getInverseMass() * impulse;

		rigidbody->setVelocity(velocity);
		hit.rigidbody->setVelocity(otherVelocity);

		// Unstuck both transforms to allow movement
		transform->Move(hit.mtvAmount * hit.mtvAxis);
		
	}
	//else
	//{
	//	std::cout << relative << std::endl;
	//	std::cout << hit.getMTVAxis() << std::endl;
	//	std::cout << velAlongNormal << std::endl;
	//}
}



std::vector<ColliderComponent*> PhysicsSystem::getNearbyColliders(ColliderComponent* self)
{
	std::vector<ColliderComponent*> nearby;
	for (RigidbodyComponent* sub : subscribers)
	{
		ColliderComponent* collider = sub->getParent()->getComponent<ColliderComponent>();
		if (collider != self)
			nearby.push_back(collider);
	}
	return nearby;
}


/* Separating Axis Theorem Methods */



bool PhysicsSystem::checkSeparatingPlaneExists(const Vector3& pos, const Vector3& planeAxis, TransformComponent* transform, TransformComponent* other,
	Vector3* transformLocal, Vector3* otherLocal)
{
	if (planeAxis == Vector3(0, 0, 0)) return false;

	const Vector3& halfScale = transform->getParent()->getComponent<ColliderComponent>()->getScale() * 0.5f;
	const Vector3& halfScale2 = other->getParent()->getComponent<ColliderComponent>()->getScale() * 0.5f;

	float a1 = fabs((*(transformLocal) * halfScale.x).Dot(planeAxis));
	float a2 = fabs((*(transformLocal + 1) * halfScale.y).Dot(planeAxis));
	float a3 = fabs((*(transformLocal + 2) * halfScale.z).Dot(planeAxis));
	float b1 = fabs((*(otherLocal) * halfScale2.x).Dot(planeAxis));
	float b2 = fabs((*(otherLocal + 1) * halfScale2.y).Dot(planeAxis));
	float b3 = fabs((*(otherLocal + 2) * halfScale2.z).Dot(planeAxis));

	float result = a1 + a2 + a3 + b1 + b2 + b3;
	float c1 = fabs(pos.Dot(planeAxis));

	// If the projection of the vector between object A and B is more than 
	// the projection of half of object A and projection of half object B combined.
	// object A collides with object B. 
	// This only works with symmetrical objects.

	return (c1 >= result);
}

void PhysicsSystem::getProjOverlap(ColliderComponent* collider, const Vector3& translation, const Vector3& rotation, const Vector3& axis, float& min, float& max)
{
	const std::vector<Vector3>& vertices = collider->getVertices(translation, rotation);
	min = vertices[0].Dot(axis);
	max = min;

	for (int i = 1; i < (int) vertices.size(); i++)
	{
		float dist = vertices[i].Dot(axis);
		if (dist < min) min = dist;
		if (dist > max) max = dist;
	}

}

void PhysicsSystem::generateAxes(TransformComponent* transform, TransformComponent* other, std::vector<Vector3>& axes)
{
	Vector3* transformLocal = transform->getLocalAxes();
	Vector3* otherLocal = other->getLocalAxes();

	for (unsigned int i = 0; i < 3; ++i)
		axes.push_back(*(transformLocal + i));
	for (unsigned int i = 0; i < 3; ++i)
		axes.push_back(*(otherLocal + i));

	// Take into consideration the edges of each object, by crossing each local axis of one with another.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 6; j++)
		{
			Vector3 result = axes[i].Cross(axes[j]);
			if (result != Vector3(0, 0, 0))
				axes.push_back(result);
			/*if (result.x != 0 || result.y != 0 || result.z != 0)
				axes.push_back(result);*/
		}
	}
}

float PhysicsSystem::getLineOverlap(float & aMin, float & aMax, float & bMin, float & bMax)
{
	return Math::Max(0.0f, Math::Min(aMax, bMax) - Math::Max(aMin, bMin));
}




void PhysicsSystem::registerComp(Component* component)
{
	RigidbodyComponent* collider = static_cast<RigidbodyComponent*>(component);
	if (collider != nullptr)
		subscribers.push_back(collider);
}

void PhysicsSystem::removeComp(Component* component)
{
	RigidbodyComponent* collider = dynamic_cast<RigidbodyComponent*>(component);
	if (collider != nullptr)
		subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), collider));
}


//if(name == "obj0")
//	std::cout << name << ": I hit " << hit.getTransform()->getParent()->getName() << std::endl;

//if(collisions.find(collider) == collisions.end() || collisions.find(hit.getCollider()) == collisions.end())
//	collisions[collider] = hit.getCollider();

//bool isNewEntry = true;

//if (collisions.find(collider) != collisions.end())
//{
//	std::vector<ColliderComponent*>& collisionList = collisions[collider];
//	auto index = std::find(collisionList.begin(), collisionList.end(), hit.getCollider());
//	if (index != collisionList.end())
//	{
//		isNewEntry = false;
//	}
//}

//if (collisions.find(hit.getCollider()) != collisions.end())
//{
//	std::vector<ColliderComponent*>& collisionList = collisions[hit.getCollider()];
//	auto index = std::find(collisionList.begin(), collisionList.end(), collider);
//	if (index != collisionList.end())
//	{
//		isNewEntry = false;
//	}
//}



//if (isNewEntry){
//	resolveCollisions(collider, hit.getCollider(), hit, dt);
//	collisions[collider].push_back(hit.getCollider());
//}