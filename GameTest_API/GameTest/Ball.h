#pragma once
#ifndef __Ball_h__
#define __Ball_h__
#include "table.h"
#include "Physics.h"
#include <vector>

using std::pair; 
using std::vector; 
class Ball
{
private: 
	//GENERAL POSITION
	CPoint _pos; 
	//BOX
	CTable _gbox;
	//OFFSET FROM THE BOX 
	vector< pair<float, float>>  offsetX; 
	vector< pair<float, float>>  offsetY; 
	//BIND GENERAL POSITION TO BOX
	void CalculateRelativePos(); 
	void UpdateOffset(); 
	Physics* phys;
public: 
	State state = air; 
	void setPos(CPoint val) { _pos = val; UpdateOffset(); };
	Ball(CTable box, CPoint initialPos) : _gbox(box)
	{
		_pos.m_x =(_gbox.m_lines[2].m_start.m_x- _gbox.m_lines[2].m_end.m_x)/2 + _gbox.m_lines[2].m_end.m_x;
		_pos.m_y = (_gbox.m_lines[0].m_start.m_y - _gbox.m_lines[0].m_end.m_y)/2 + _gbox.m_lines[0].m_end.m_y;
		CalculateRelativePos(); 
		setPos(initialPos); 
		phys = new Physics(1.0f, 3.0f, _pos, state); 
	}
	void Rotate(float deg); 
	void Rotation(float deltaTime,float speed); 
	//void setPos(float x, float y); 
	void UpdatePhysics(float deltaTime)
	{
		phys->UpdatePhysics(deltaTime); 
		UpdateOffset(); 
	}
	void AddForce(float x, float y)
	{
		phys->addForce(x, y); 
	}
	void Wall(CLineSegment line)
	{
		phys->Wall(line); 
	}
	void Collision(float x, float y, CLineSegment line,pair<float, float> flip = make_pair(0, 0))
	{
		phys->Collision(x, y, line,flip);
	}
	pair<float, float> getVelocity() { return phys->getVelocity(); }
	CPoint getPos() { return _pos; }
	CTable getTable() { return _gbox; }
	void Print(); 
};

#endif // !__Ball_h__
