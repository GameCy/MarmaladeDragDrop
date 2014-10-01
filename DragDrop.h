#pragma once
#include <list>
#include "IwGx.h"

class Draggable;

class DropTarget
{
public:
    virtual bool IsInside(CIwFVec2 pos, Draggable* source)=0;

protected:
	DropTarget()	{}
	~DropTarget()	{}
};

class DropTargetList
{
public:
    void	Add(DropTarget* dropTarget);
    void	Remove(DropTarget* dropTarget);

	std::list<DropTarget*>	List;
};

// -------------------------

class Draggable
{
public:
	Draggable(void)
		: isDragging(false), targets(0)
	{}
	~Draggable(void)
	{}

	virtual bool IsInside(CIwFVec2 pos)=0;
	void	SetTargetList(DropTargetList* targetList);
    bool	IsDragging();
	CIwFVec2 GetLastPosition();
	CIwFVec2 GetDragVector();

	// manual updates
	void BeginDrag(CIwFVec2 pos);
	void UpdateDrag(CIwFVec2 pos);
	void EndDrag(CIwFVec2 pos);
	void CheckDropTargets();

private:
	CIwFVec2	beginDragPos;
	CIwFVec2	lastDragPos;
	bool		isDragging;

	DropTargetList* targets;
};

typedef std::list<Draggable*>	DraggableList;

enum ButtonStates
{
    ButtonDown,
    ButtonUp,
    ActiveMotion,
    PassiveMotion,
    ButtonUndefined
};

struct MouseEventInfo
{
public:
    MouseEventInfo(int touch_id, int x, int y, ButtonStates st)
    {
        id = touch_id; X=x; Y=y; state = st;
    }

	int id;
    int X,Y;
    ButtonStates state;
};

class DragDropManager
{
public:
	DragDropManager();
	~DragDropManager();

	DraggableList Draggables;

	// handlers and delegates
	void BeginDragging(MouseEventInfo mouse);
	void UpdateDragging(MouseEventInfo mouse);
	void EndDragging(MouseEventInfo mouse);
};

extern DragDropManager* gDragDropManager;
