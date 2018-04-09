//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <string> 
#include <vector>
#include <fstream>
//------------------------------------------------------------------------
#include "app\app.h"
#include "table.h"
#include "Ball.h"
#include "Flipper.h"
#include "Components.h"
using namespace std;

//INITIALIZATION
CTable outline; 
Ball* ball;
Flipper* flip1;
Flipper* flip2;

//STORE ALL GAME CTABLES
vector<CTable*> objects;

//STORE ALL COLLISION OBJECTS
vector<Components> items;

// CURRENT SCORE
int SCORE = 0;

//PAUSE PHYSICS AND MOVEMENT
bool start = false;

//STATES OF THE GAME
enum gameState {gameOver, random, level,selection,win};
gameState gstate = selection;

//STORE LEVELS
const int LEVELS = 7; 
int currentLevel = 1; 

//STORE DATA OF COLLISION OBJECTS
struct itemData
{
	string file; 
	float offsetX, offsetY; 
	int points, health; 
};
vector<itemData> availItems;

//READ TABLE FORMATS AND RETURN THE RESPECTIVE CTABLE
CTable ReadTable(string filename, bool rev = false)
{
	//input stream
	ifstream in(filename);
	int version;
	in >> version;
	CTable temp;
	while (!in.eof())
	{
		CLineSegment line;
		int type;
		in >> type;
		line.m_type = static_cast<LineType>(type);

		in >> line.m_start.m_x;
		in >> line.m_start.m_y;
		in >> line.m_end.m_x;
		in >> line.m_end.m_y;
		if (rev)
		{
			line.m_start.m_x *= -1;
			line.m_end.m_x *= -1;
		}
		temp.m_lines.push_back(line);
	}
	return temp;
}

//ADD UNIQUE COLLISION OBJECT TYPE 
void LoadItems(string fileName)
{
	ifstream in(fileName.c_str());
	while (!in.eof())
	{
		itemData a; 
		in >> a.file; 
		in >> a.offsetX; 
		in >> a.offsetY; 
		in >> a.points; 
		in >> a.health; 
		availItems.push_back(a); 
	}
}

//ADD A NEW COLLISION OBJECT TO THE SCREEN
void addItems(string file, float x, float y, int points = 0, int health = 50,  int index = -1, int rep = -1)
{
	CPoint in;
	CTable ct = ReadTable(file.c_str());
	in.m_x = x;
	in.m_y = y;
	Components *a = new Components(ct, in, health, points);
	a->replacer = rep; 
	if (index < 0) {
		items.push_back(*a);
		objects.push_back(&a->getTable());
	}
	else
	{
		items[index] = *a; 
		objects[index + 3] = &items[index].getTable(); 
	}
	//delete a; 
}

//REMOVE OR REPLACE ANY OBJECTS WHICH HAVE LOST ALL HEALTH
void CleanUp()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].Killed())
		{
			if (gstate == random){
				int index = rand() % availItems.size(); 
				addItems(availItems[index].file, items[i].getPos().m_x + availItems[index].offsetX, items[i].getPos().m_y + availItems[index].offsetY, availItems[index].points, availItems[index].health, i);
				items[i].FadeIn();
			}
			else
			{
				if (items[i].replacer >= 0)
				{
					addItems(availItems[items[i].replacer].file, items[i].getPos().m_x + availItems[items[i].replacer].offsetX, items[i].getPos().m_y + availItems[items[i].replacer].offsetY, availItems[items[i].replacer].points, items[i]._maxLife, i);
					items[i].FadeIn();
				}
				else {
					items.erase(items.begin() + i);
					objects.erase(objects.begin() + i + 3);
				}
			}
		}
	}
}
//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	LoadItems("Itemlist.txt");
	outline = ReadTable("outerBoarder.txt", false);
	objects.push_back(&outline);

	// CREATE AN INSTANCE OF THE BALL
	CPoint in;
	in.m_x = APP_INIT_WINDOW_WIDTH - 300;
	in.m_y = 170;
	ball = new Ball(ReadTable("ball.txt", false), in);

	//CREATE AN INSTANCE OF LEFT FLIPPER
	in.m_x = APP_INIT_WINDOW_WIDTH - 515;
	in.m_y = 170;
	flip1 = new Flipper(in, ReadTable("wack.txt", false));

	//CREATE AN INSTANCE OF THE RIGHT FLIPPER
	in.m_x = APP_INIT_WINDOW_WIDTH - 105;
	flip2 = new Flipper(in, ReadTable("wack.txt", true));

	//ADD TO CONTACT OBJECTS
	objects.push_back(&flip1->getTable());
	objects.push_back(&flip2->getTable());
}

//INITIAL ITEMS ADDED TO SCREEN IN RANDOM STATE
void LoadInitialRandom()
{
	addItems("hex.txt", APP_INIT_WINDOW_WIDTH - 355, 420, availItems[0].points, availItems[0].health);
	addItems("hex.txt", APP_INIT_WINDOW_WIDTH - 450, 320, availItems[0].points, availItems[0].health);
	addItems("hex.txt", APP_INIT_WINDOW_WIDTH - 250, 320, availItems[0].points, availItems[0].health);

	addItems("line.txt", APP_INIT_WINDOW_WIDTH - 305, 680, availItems[1].points, availItems[1].health);
	addItems("line.txt", APP_INIT_WINDOW_WIDTH - 400, 680, availItems[1].points, availItems[1].health);
	addItems("line.txt", APP_INIT_WINDOW_WIDTH - 200, 680, availItems[1].points, availItems[1].health);

}

//COLLISION DETECTION
bool Collision(float deltaTime)
{
	CTable b = ball->getTable();
	bool contact = false;
	int count = 0;
	for (auto obj : objects)
	{
		for (int i = 0; i < obj->m_lines.size(); i++)
		{
			for (int j = 0; j < b.m_lines.size(); j++) {
				if (obj->m_lines[i].IsOnLine(b.m_lines[j], 11))
				{
					//STORE SLOPE OF THE LINE THE BALL HIT
					float dx = obj->m_lines[i].m_end.m_x - obj->m_lines[i].m_start.m_x;
					float dy = obj->m_lines[i].m_end.m_y - obj->m_lines[i].m_start.m_y;

					//REDUCE CALCULATION FOR VARIOUS SURFACES
					if (obj->m_lines[i].m_type == eLine_Flipper)
					{
						ball->state = ground;
						if (dy < dx)
							ball->Collision(dx, dy, obj->m_lines[i], flip1->getVelocity(ball->getPos()));
						else
							ball->Collision(dx, dy, obj->m_lines[i], flip2->getVelocity(ball->getPos()));
					}
					else if (obj->m_lines[i].m_type == eLine_Wall)
					{
						if (i == 1)
							gstate = gameOver;
						ball->Wall(obj->m_lines[i]);
					}
					else
						ball->Collision(dx, dy, obj->m_lines[i]);

					//CHECK STATS IF THE OBJECT WAS A COLLISION OBJECT
					if (count > 2)
					{
						items[count - 3]._life--;
						items[count - 3].ShakeObj();
						SCORE += items[count - 3].getPoints();
					}
					contact = true;
				}
			}
		}
		if (count > 2)
			items[count - 3].AnimationController(deltaTime);
		count++;
	}

	if (contact)
		return true;
	ball->state = air;
	return false;
}

//DRAW PASSED IN CTABLE
void DrawCTable(CTable val)
{
	for (auto& line : val.m_lines)
		App::DrawLine(line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_end.m_y);
}

//LOAD ALL VALUES AND STATISTICS FOR THIS LEVEL
void LoadLevel(int index)
{ 
	string file = "Level" + to_string(index) + ".txt";
	ifstream in(file);
	string bounds;
	in >> bounds;
	outline = ReadTable(bounds);
	objects[0] = &outline; 
	while (!in.eof())
	{
		int i, p,h,r; 
		in >> i; 
		float x, y; 
		in >> x >> y >> p >> h>> r; 
		addItems(availItems[i].file, APP_INIT_WINDOW_WIDTH - x + availItems[i].offsetX, y + availItems[i].offsetY, p,h,-1,r); 
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//CHOOSE GAME MODE TYPE
	if (gstate == selection)
	{
		if (App::IsKeyPressed('L')) {
			gstate = level;
			LoadLevel(currentLevel); 
		}
		else if (App::IsKeyPressed('R')) {
			LoadInitialRandom(); 
			gstate = random;
		}
	}
	if (gstate == level || gstate == random || gstate == win || gstate == gameOver)
	{
		if (App::IsKeyPressed('K'))
		{
			CPoint in;
			in.m_x = APP_INIT_WINDOW_WIDTH - 300;
			in.m_y = 170;
			ball->setPos(in);
			items.clear();
			objects.erase(objects.begin() + 3, objects.end());
			currentLevel = 1;
			gstate = selection;
			outline = ReadTable("outerBoarder.txt"); 
			SCORE = 0; 
		}
	}
	//GAME UPDATES
	if (gstate == level || gstate == random ) {
		if (App::IsKeyPressed('W') && !start) {
			ball->AddForce(0, -gravity);
			start = true; 
		}
		flip1->Flip(App::IsKeyPressed('A'), true);
		flip2->Flip(App::IsKeyPressed('D'), false);
		if (start) {
			Collision(deltaTime);
			ball->Rotation(deltaTime, 1);
			CleanUp();
			ball->UpdatePhysics(deltaTime);
			if (items.size() == 0)
			{
				start = false; 
				if (currentLevel + 1 <= LEVELS) {
					currentLevel++;
					CPoint in;
					in.m_x = APP_INIT_WINDOW_WIDTH - 300;
					in.m_y = 170;
					ball->setPos(in); 
					LoadLevel(currentLevel); 
				}
				else
					gstate = win; 
			}
		}
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	// DRAW CONTACT OBJECTS
	for (auto& draw : items)
		draw.Print();
	//DRAW BOARDER/FLIPPER/BALL
	DrawCTable(outline);
	ball->Print();
	flip1->Print();
	flip2->Print();

	//STATISTICS AND PROMPTS FOR USER
	App::Print(200, 700, "PINSQUARE", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
	if (gstate == level || gstate == random) {
		if (!start) {
			App::Print(200, 500, "PRESS W TO START", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
			App::Print(200, 480, "USE (A) & (B) FOR FLIPPERS", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		}
		else {
			App::Print(200, 500, "SCORE:", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
			App::Print(200, 450, (to_string(SCORE)).c_str(), 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		}
	}
	else if (gstate == selection)
	{
		App::Print(200, 500, "Which Game Mode?", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(200, 450, "Level Mode (L)", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
		App::Print(200, 400, "Random Mode (R)", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);

	}
	if (gstate == gameOver)
	{
		App::Print(200, 650, "GAME OVER", 1, 0, 0, GLUT_BITMAP_HELVETICA_18);
		App::Print(50, 630, "PRESS (K) TO RETURN TO SELECTION SCREEN", 1, 1, 1, GLUT_BITMAP_HELVETICA_18);
	}
	else if (gstate == win)
		App::Print(200, 650, "CONGRATS YOU WIN", 0, 1, 0, GLUT_BITMAP_HELVETICA_18); 
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}