#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "Camera.h"

class Camera2D : public Camera
{
public:

	Camera2D(const Vector3& position);
	Camera2D();
	~Camera2D();

	Mtx44 LookAt();
	void Init(const Vector3& pos);
	void Update(double dt);

private:
	Mtx44 transformation;
	Vector3 position;
	float rotation;

};

#endif