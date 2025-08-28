#include "Vehicle.h"

Vehicle::Vehicle(glm::vec3 position, float yaw, float speed) : m_Position(position), m_Yaw(yaw), m_Speed(speed), m_Throttle(0.0f), m_Brake(0.0f), m_Steering(0.0f)
{

}