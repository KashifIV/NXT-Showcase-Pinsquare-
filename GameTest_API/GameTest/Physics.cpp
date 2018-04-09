#include "stdafx.h"
#include "table.h"
#include "Physics.h"
#include "App\app.h"
#include <math.h>

const float terminalVelocity = 80.0f;
void Physics::UpdatePhysics(float deltaTime)
{
	if (abs(_velocity.first) < terminalVelocity)
	{
		_velocity.first += _acceleration.first / deltaTime;
	}
	if (abs(_velocity.second) < terminalVelocity)
	{
		_velocity.second += _acceleration.second / deltaTime;
	}

	/*if (abs(_velocity.first) < 0.4f && *state == ground)
		_velocity.first = 0;
	if (abs(_velocity.second) < 0.4f && *state == ground)
		_velocity.second = 0; */
	if (abs(_velocity.first) > terminalVelocity) {
		if (_velocity.first < 0)
			_velocity.first = -terminalVelocity;
		else
			_velocity.first = terminalVelocity;
	}
	else if (abs(_velocity.second) > terminalVelocity)
	{
		if (_velocity.second < 0)
			_velocity.second = -terminalVelocity;
		else
			_velocity.second = terminalVelocity;
	}
	if (obj->m_x + _velocity.first / deltaTime > 480 && obj->m_x + _velocity.first / deltaTime < 950)
		obj->m_x += _velocity.first/deltaTime;
	obj->m_y += _velocity.second/deltaTime;
}
void Physics::addForce(float x, float y)
{
	_acceleration.first += x / _mass;
	_acceleration.second += y / _mass;
}
void Physics::Wall(CLineSegment line)
{
	if (obj->m_x <= line.m_start.m_x && obj->m_x <= line.m_end.m_x)
		_velocity.first = -abs(_velocity.first);
	else if(obj->m_x > line.m_start.m_x && obj->m_x > line.m_end.m_x)
		_velocity.first = abs(_velocity.first);
	if (obj->m_y <= line.m_start.m_y && obj->m_y <= line.m_end.m_y)
		_velocity.second = -abs(_velocity.second);

}
void Physics::Collision(float dirX, float dirY, CLineSegment line, pair<float, float> flip)
{
	if (abs(flip.first) > 0)
	{
		_velocity.first = abs(flip.first)*3;
		_velocity.second = abs(flip.second)*3;
		if (line.m_end.m_x - line.m_start.m_x < 0)
			_velocity.first = -abs(_velocity.first);
		else
			_velocity.first = abs(_velocity.first);

	}
	else if (*state == ground)
	{
		_velocity.first = dirX;
		_velocity.second = dirY;
	}
	else {
		if (dirX >= dirY)
		{
			//App::PlaySound("hitX.wav"); 
			_velocity.first += dirY / dirX;
			_velocity.first *= 1.2;
			if (obj->m_x <= line.m_start.m_x || obj->m_x <= line.m_end.m_x)
				_velocity.first = abs(_velocity.first)*-1;
			else
				_velocity.first = abs(_velocity.first);
		}
		else if (dirY > dirX)
		{
			//App::PlaySound("hitY.wav");
			_velocity.second += dirX / dirY;
			_velocity.second *= 1.2;
			//_velocity.first += dirX / dirY*0.004f;
			if (obj->m_y < line.m_start.m_y || obj->m_y < line.m_end.m_y)
				_velocity.second = abs(_velocity.second)*-1;
			else
				_velocity.second = abs(_velocity.second);
		}
		/*if (obj->m_x <= line.m_start.m_x || obj->m_x <= line.m_end.m_x)
			_velocity.first = abs(_velocity.first)*-1;
		else
			_velocity.first = abs(_velocity.first);
		if (obj->m_y < line.m_start.m_y || obj->m_y < line.m_end.m_y)
			_velocity.second = abs(_velocity.second)*-1;
		else
			_velocity.second = abs(_velocity.second);*/
	}
	//_velocity.first *= dirX; 
	//_velocity.second *= dirY; 
}