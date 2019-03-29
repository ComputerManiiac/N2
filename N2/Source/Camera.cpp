#include "Camera.h"
#include "Application.h"


Camera::Camera(const Vector3& pos) {
	Reset();
	Init(pos);
}

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos)
{
	firstMouse = true;
	lastX = 684.0f;
	lastY = 384.0f;
	yaw = -89.0f;
	pitch = 0.0f;
	sensitivity = 0.04f;

	front = Vector3(0.0f, 0.0f, 1.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	this->position = pos;

	canFreeLook = true;

	updateMouse();
}

void Camera::Reset()
{
	position.Set(1, 100, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::setFreeLook(bool state) {
	canFreeLook = state;
}

void Camera::updateMouse()
{
	double mouseX = Application::getMouseX();
	double mouseY = Application::getMouseY();

	if (firstMouse)
	{
		lastX = mouseX;
		lastY = mouseY;
		firstMouse = false;
	}

	double xoffset = (mouseX -lastX);
	// Flip since Y-coordinates range from bottom to top
	double yoffset = (lastY -  mouseY);
	lastX = mouseX;
	lastY = mouseY;


	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += (float)xoffset;
	pitch += (float)yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	float pitchRad = Math::DegreeToRadian(pitch);
	float yawRad = Math::DegreeToRadian(yaw);
	front.x = cos(pitchRad) * cos(yawRad);
	front.y = sin(pitchRad);
	front.z = cos(pitchRad) * sin(yawRad);
	front.Normalize();


	this->target = position + front;
}

void Camera::Update(double dt)
{
	float speed = 5.0 * dt;

	if (Application::isKeyPressed(GLFW_KEY_W)) {
		position += front * speed;
	}
	if (Application::isKeyPressed(GLFW_KEY_S)) {
		position -= front * speed;
	}
	if (Application::isKeyPressed(GLFW_KEY_D)) {
		position += front.Cross(up).Normalized() * speed;
	}
	if (Application::isKeyPressed(GLFW_KEY_A)) {
		position -= front.Cross(up).Normalized() * speed;
	}
	if (Application::isKeyPressed(GLFW_KEY_Q)) {
		position += up * speed;
	}
	if (Application::isKeyPressed(GLFW_KEY_E)) {
		position -= up * speed;
	}

	if (canFreeLook)
		updateMouse();
}

Mtx44 Camera::LookAt()
{
	Vector3 f = front.Normalize();
	Vector3 s = f.Cross(up).Normalize();
	Vector3 u = s.Cross(f);

	Mtx44 mat;
	if (canFreeLook) {

		mat = Mtx44(s.x, u.x, -f.x, 0,
			s.y, u.y, -f.y, 0,
			s.z, u.z, -f.z, 0,
			-s.Dot(target), -u.Dot(target), f.Dot(target), 1);
	}


	return mat;
}



void Camera::Invert()
{
	pitch = -pitch;
}

const Vector3& Camera::getFront() const {
	return front;
}

const Vector3& Camera::getRight() const {
	return front.Cross(up).Normalize();
}

const float& Camera::getYaw() const {
	return yaw;
}


const Vector3& Camera::getPos() const {
	return position;
}