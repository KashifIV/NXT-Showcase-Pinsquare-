#pragma once
#ifndef __Physics_h__
#define __Physics_h__
#include "table.h"
#define gravity 9.8f 
using std::pair; 
using std::make_pair; 
enum State {air, ground};
class Physics
{
private: 
	float _mass;
	float _drag; 
	pair<float, float> _velocity; 
	pair<float, float> _acceleration; 
	CPoint *obj; 
	State *state; 
public: 
	Physics(float mass, float drag, CPoint &pos, State &s) : _mass(mass), _drag(drag) {
		_velocity = make_pair(0, 0); 
		_acceleration = make_pair(0, 0); 
		obj = &pos; 
		state = &s; 
	}
	//UPDATE THE EFFECTS OF PHYSICS ONTO THE POSITION OF THE OBJECT
	void UpdatePhysics(float deltaTime); 
	//ADD A FORCE ONTO THE OBJECT
	void addForce(float x, float y); 
	void Wall(CLineSegment line); 
	pair<float, float> getVelocity(){ return _velocity; }
	//CALCULATE THE COLLISIONS
	void Collision(float dirX, float dirY, CLineSegment line, pair<float, float> flip = make_pair(0, 0));
	Physics(); 
};
#endif // !__Physics_h__

