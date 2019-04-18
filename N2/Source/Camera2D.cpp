#include "Camera2D.h"
#include "Application.h"

Camera2D::Camera2D(const Vector3& position){
	Init(position);
}

Camera2D::Camera2D()
{
}


Camera2D::~Camera2D()
{
}

Mtx44 Camera2D::LookAt()
{
	return transformation;
}

void Camera2D::Init(const Vector3& position)
{
	rotation = 0;
	this->position = position;
}

void Camera2D::Update(double dt)
{

	float speed = 100.0 * dt;

	//int horizontalInput = (-Application::isKeyPressed(GLFW_KEY_A) + Application::isKeyPressed(GLFW_KEY_D));
	//int verticalInput = (-Application::isKeyPressed(GLFW_KEY_W) + Application::isKeyPressed(GLFW_KEY_S));

	//position.x += horizontalInput * speed;
	//position.y += verticalInput * speed;

	Mtx44 translation;
	translation.SetToTranslation(-position.x, position.y, position.z);

	Mtx44 rot;
	rot.SetToRotation(rotation, 0, 0, 1);
	transformation = translation * rot;

}
