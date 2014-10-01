#pragma once
#include "s3ePointer.h"
#include <deque>

#define MAX_TOUCHES 10

struct TouchInfo
{
	TouchInfo() 
		: x(0), y(0), active(false), id(0) 
	{}

	TouchInfo(TouchInfo *other)
	{
		x = other->x;
		y = other->y;
		active = other->active;
		id = other->id;
	}

    int32 x;
    int32 y;
    bool active;
    int32 id; // touch's unique identifier
};

typedef std::deque<TouchInfo>	TouchInfoDeque;

class PointerProxy
{
public:
	static PointerProxy* singleton();
	static void RegisterCallbacks();
	static void Unregister();
	static void DestroySingleton();

	TouchInfo		*Touches;
	TouchInfoDeque	ClickEvents; // to be consumed and cleared, should be: lock controlled access
	TouchInfoDeque	MoveEvents; // to be consumed and cleared, should be: lock controlled access

private:
	bool bMultiTouch;
	TouchInfo* GetTouch(int32 id);

	static void MultiTouchButtonCB(s3ePointerTouchEvent* ev);
	static void MultiTouchMotionCB(s3ePointerTouchMotionEvent* ev);
	static void SingleTouchButtonCB(s3ePointerEvent* ev);
	static void SingleTouchMotionCB(s3ePointerMotionEvent* ev);

};

extern PointerProxy* gpPointerProxy;