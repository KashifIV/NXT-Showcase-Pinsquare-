#pragma once
#ifndef __Components_h__
#define __Components_h__
#include "table.h"
#include <vector>

using std::vector; 
using std::pair; 
class Components
{
private: 
	//POSITION OF OBJECT
	CPoint _pos; 

	//DRAWING TABLE/OFFSETS
	CTable _gbox; 
	vector<pair<float, float>> offsetX; 
	vector<pair<float, float>> offsetY;
	//LINK OFFSETS TO GBOX
	void CalculateRelativePos();
	//PUSH GBOX POSITIONS WITH RELATION TO _POS
	void UpdateOffset();
	CPoint averageMiddle;
	int _points; 
	 //COLOUR VALUES
	float cut, r, g, b; 
	CPoint tempPos; 
	void _fadeIn(float deltaTime); 
	void _CalculateColour(); 
public: 
	//LIFE OF OBJECT
	int _maxLife; 
	int _life; 
	//IF OBJECT IS FADING IN
	bool fade_in = false; 
	//THE OBJECT THAT WILL REPLACE THIS WHEN IT DIES
	int replacer = -1;
	Components(CTable box, CPoint pos, int life, int points) : _gbox(box), _life(life), _maxLife(life), _points(points)
	{
		_CalculateColour(); 
		_pos = _gbox.m_lines[0].m_start;
		CalculateRelativePos();
		setPos(pos);
	}
	CTable& getTable() { return _gbox; }
	void setPos(CPoint val); 
	CPoint getPos() { return _pos; }
	void setPos(float x, float y);
	void Print(); 
	bool Killed() { return _life <= 0; }
	void FadeIn(); 
	void ShakeObj(); 
	int getPoints() { return _points;  }
	void AnimationController(float deltaTime)
	{
		if (fade_in)
			_fadeIn(deltaTime); 
		
	}
};

#endif __Components_h__