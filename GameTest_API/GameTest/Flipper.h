#pragma once
#ifndef __Flipper_h__
#define __Flipper_h__
#include "table.h"
#include <vector>

using std::vector; 
using std::pair; 
using std::make_pair; 
class Flipper
{
private: 
	//POSITION AND DRAWING BOX
	CPoint _pos; 
	CTable _gbox; 
	//THE BOX OF THE PREVIOUS FRAME
	CTable _prevbox; 
	vector<pair<float, float>> offsetX; 
	vector<pair<float, float>> offsetY; 
	//ROTATE/MOVE THE BOX FOR FLIPPING
	void Rotate(float deg);
	void UpdateOffset(); 
	//ANGLE THE BOX IS AT
	int _deg = 0; 
	void CalculateRelativePos(); 
public: 
	Flipper(CPoint pos, CTable box) : _gbox(box), _prevbox(box) 
	{
		_pos = _gbox.m_lines[0].m_start; 
		CalculateRelativePos(); 
		setPos(pos); 
	}
	void setPos(float x, float y); 
	void setPos(CPoint val); 
	void Flip(bool up, bool left); 
	void Print(); 
	//GET THE VELOCITY OF THE MOVEMENT OF THE FLIPPER AT THE POSITION OF THE BALL
	pair<float, float> getVelocity(CPoint ball); 
	CTable& getTable() { return _gbox; }
};
#endif // !__Flipper_h__

