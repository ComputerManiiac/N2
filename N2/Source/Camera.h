#ifndef CAMERA_H
#define CAMERA_H


#include "Camera.h"
#include "Vector3.h"
#include "Mtx44.h"

class Camera
{
public:

	Camera(const Vector3& pos);
	Camera();
	~Camera();


	virtual Mtx44 LookAt();
	virtual void Init(const Vector3& pos);
	virtual void Update(double dt);

	void Reset();
	void Invert();
	void setFreeLook(bool state);

	const Vector3& getRight() const;
	const Vector3& getFront() const;
	const float& getYaw() const;
	const Vector3& getPos() const;

private:
	Vector3 front;
	Vector3 target;
	Vector3 up;

	bool firstMouse;
	bool canFreeLook;
	double lastX = 0.0;
	double lastY = 0.0;
	float sensitivity = 0.08f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	void updateMouse();

protected:


	Vector3 position;
	
};

#endif