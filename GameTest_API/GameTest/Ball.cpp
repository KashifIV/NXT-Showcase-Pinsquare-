#include "stdafx.h"
#include "Ball.h"
#include "table.h"
#include "App\app.h"
#include "Physics.h"
#include <vector> 

using std::vector; 
using std::make_pair; 
void Ball::CalculateRelativePos()
{
	
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		offsetX.push_back(make_pair(_pos.m_x - _gbox.m_lines[i].m_start.m_x, _pos.m_x - _gbox.m_lines[i].m_end.m_x)); 
		offsetY.push_back(make_pair(_pos.m_y - _gbox.m_lines[i].m_start.m_y, _pos.m_y - _gbox.m_lines[i].m_end.m_y)); 
	}
}
void Ball::UpdateOffset()
{
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		_gbox.m_lines[i].m_start.m_x = _pos.m_x + offsetX[i].first; 
		_gbox.m_lines[i].m_end.m_x = _pos.m_x + offsetX[i].second;
		_gbox.m_lines[i].m_start.m_y = _pos.m_y + offsetY[i].first;
		_gbox.m_lines[i].m_end.m_y = _pos.m_y + offsetY[i].second;
	}
}
void Ball::Rotate(float deg)
{
	//float a = deg*PI / 180;
	float a = deg; 
	int counter = 0; 
	for (auto& point : _gbox.m_lines)
	{
		float x0 = (point.m_start.m_x - _pos.m_x) *cos(a) + _pos.m_x - (point.m_start.m_y - _pos.m_y)*sin(a);
		float y0 = (point.m_start.m_y - _pos.m_y)*cos(a) + (point.m_start.m_x - _pos.m_x)*sin(a) + _pos.m_y;

		float x1 = (point.m_end.m_x - _pos.m_x) *cos(a) + _pos.m_x - (point.m_end.m_y - _pos.m_y)*sin(a);
		float y1 = (point.m_end.m_y - _pos.m_y)*cos(a) + (point.m_end.m_x - _pos.m_x)*sin(a) + _pos.m_y;

		point.m_start.m_x = x0;
		point.m_start.m_y = y0;
		point.m_end.m_x = x1;
		point.m_end.m_y = y1;

		offsetX[counter] = make_pair(_pos.m_x - _gbox.m_lines[counter].m_start.m_x, _pos.m_x - _gbox.m_lines[counter].m_end.m_x);
		offsetY[counter] = make_pair(_pos.m_y - _gbox.m_lines[counter].m_start.m_y, _pos.m_y - _gbox.m_lines[counter].m_end.m_y);
		counter++; 
	}
}
void Ball::Print()
{
	for (auto& line : _gbox.m_lines)
		App::DrawLine(line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_end.m_y,1,1,1);
}
void Ball::Rotation(float deltaTime, float speed)
{
	pair<float, float> vel = phys->getVelocity();
	if (vel.first != 0 || vel.second != 0) {
		if (vel.first > 0 || vel.second > 0)
			Rotate(speed*deltaTime/200.0f);
		else
			Rotate(-speed*deltaTime/200.0f);
	}
}