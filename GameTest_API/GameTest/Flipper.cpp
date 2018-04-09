#include "stdafx.h"
#include "Flipper.h"
#include "table.h"
#include "App\app.h"
#include <string>

using namespace std; 
void Flipper::CalculateRelativePos()
{
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		offsetX.push_back(make_pair(_gbox.m_lines[i].m_start.m_x - _pos.m_x,_gbox.m_lines[i].m_end.m_x - _pos.m_x));
		offsetY.push_back(make_pair(_gbox.m_lines[i].m_start.m_y - _pos.m_y, _gbox.m_lines[i].m_end.m_y - _pos.m_y));
	}
}
void Flipper::Rotate(float deg)
{
	float a = deg*PI / 180;
	_prevbox = _gbox; 
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

	}
}
void Flipper::Flip(bool up, bool left)
{
	/*if (!App::IsSoundPlaying("flip.wav"))
		App::PlaySound("flip.wav");*/
	int dir = 1; 
	if (!left)
		dir *= -1; 
	if (up && _deg < 40 && _deg >= 0) {
		Rotate(4.0f*dir);
		_deg+= 4;
	}
	else if (_deg > 0 && !up) {
		Rotate(-4.0f*dir);
		_deg-= 4; 
	}
}
void Flipper::UpdateOffset()
{
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		_gbox.m_lines[i].m_start.m_x = _pos.m_x + offsetX[i].first;
		_gbox.m_lines[i].m_end.m_x = _pos.m_x + offsetX[i].second;
		_gbox.m_lines[i].m_start.m_y = _pos.m_y + offsetY[i].first;
		_gbox.m_lines[i].m_end.m_y = _pos.m_y + offsetY[i].second;
	}
}
void Flipper::setPos(float x, float y)
{
	_pos.m_x = x; 
	_pos.m_x = y; 
	UpdateOffset(); 
}
void Flipper :: setPos(CPoint val)
{
	_pos = val; 
	UpdateOffset(); 

}
void Flipper::Print()
{
	for (auto& line : _gbox.m_lines)
		App::DrawLine(line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_end.m_y);
}
pair<float, float> Flipper::getVelocity(CPoint ball)
{
	float dx = _gbox.m_lines[2].m_end.m_x - _prevbox.m_lines[2].m_start.m_x;
	float dy = _gbox.m_lines[2].m_end.m_y - _prevbox.m_lines[2].m_start.m_y;
	float angle = atan(dy / dx);
	float displaceX = ball.m_x - _gbox.m_lines[2].m_start.m_x;
	float displaceY = ball.m_y - _gbox.m_lines[2].m_start.m_y;
	float total = sqrt(displaceX*displaceX + displaceY*displaceY);
	float prevX = displaceX - cos(angle)*total;
	float prevY = displaceX - sin(angle)*total;
	float ansX = displaceX - prevX;
	float ansY = displaceY - prevY;
	//return make_pair(0, 0);
	return make_pair(ansX, ansY);
}