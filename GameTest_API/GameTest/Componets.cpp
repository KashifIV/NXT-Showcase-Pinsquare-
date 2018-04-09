#include "stdafx.h"
#include "Components.h"
#include <string> 
#include "App\app.h"
using std::make_pair;
using std::to_string; 

void Components::CalculateRelativePos()
{
	averageMiddle.m_x = 0; 
	averageMiddle.m_x = 0; 
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		offsetX.push_back(make_pair(_pos.m_x - _gbox.m_lines[i].m_start.m_x, _pos.m_x - _gbox.m_lines[i].m_end.m_x));
		offsetY.push_back(make_pair(_pos.m_y - _gbox.m_lines[i].m_start.m_y, _pos.m_y - _gbox.m_lines[i].m_end.m_y));
		
	}
}
void Components::UpdateOffset()
{
	for (int i = 0; i < _gbox.m_lines.size(); i++)
	{
		_gbox.m_lines[i].m_start.m_x = _pos.m_x + offsetX[i].first;
		_gbox.m_lines[i].m_end.m_x = _pos.m_x + offsetX[i].second;
		_gbox.m_lines[i].m_start.m_y = _pos.m_y + offsetY[i].first;
		_gbox.m_lines[i].m_end.m_y = _pos.m_y + offsetY[i].second;

		averageMiddle.m_x += _gbox.m_lines[i].m_start.m_x + _gbox.m_lines[i].m_end.m_x; 
		averageMiddle.m_y += _gbox.m_lines[i].m_start.m_y + _gbox.m_lines[i].m_end.m_y; 
	}
	averageMiddle.m_x /= _gbox.m_lines.size()*2;
	averageMiddle.m_y /= _gbox.m_lines.size()*2;
}
void Components::setPos(CPoint val)
{
	_pos = val; 
	UpdateOffset(); 
}
void Components::setPos(float x, float y)
{
	_pos.m_x = x; 
	_pos.m_y = y; 
	UpdateOffset();
}
void Components::_CalculateColour()
{
	cut = _maxLife / 3;
	b = _life / cut * 3;
	g = abs(cut - _life) / cut * 2;
	r = abs(cut * 2 - _life) / cut;
}
void Components::Print()
{
	for (auto& line : _gbox.m_lines)
		App::DrawLine(line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_end.m_y,r,g,b);
	App::Print(averageMiddle.m_x-10, averageMiddle.m_y, to_string(_points).c_str(),r,g,b,GLUT_BITMAP_HELVETICA_12); 
}
void Components::FadeIn()
{
	fade_in = true; 
	r = g = b = 0; 
}
void Components::_fadeIn(float deltaTime)
{
	const float speed = 1; 
	bool done = true; 
	float tempr = r, tempg = g, tempb = b; 
	_CalculateColour(); 
	if (tempr < r) {
		tempr += speed / deltaTime;
		done = false; 
	}
	if (tempg < g) {
		tempb += speed / deltaTime;
		done = false; 
	}
	if (tempb < b) {
		tempb += speed / deltaTime;
		done = false; 
	}
	r = tempr; 
	g = tempg; 
	b = tempb; 
	if (done)
		fade_in = false; 
}
void Components::ShakeObj()
{
	_CalculateColour(); 
}