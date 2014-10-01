#include "s3e.h"
#include "IwDebug.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "DragDrop.h"


// ---------------------------------------------------------------

class Blob : public Draggable
{
public:
	Blob(CIwFVec2 pos, CIw2DImage*  img, CIwFVec2 siz, DropTargetList* targetList)
	{
		position = pos;
		size = siz;
		image = img;
		SetTargetList(targetList);
	}

	virtual bool IsInside(CIwFVec2 pos)
	{
		if ((pos-position).GetLength()<20)
			return true;
		return false;
	}

	void Render()
	{
		if (IsDragging())		position = GetLastPosition();

		Iw2DDrawImage(image, position - size/2, size);
	}

private:
	CIwFVec2	position;
	CIwFVec2	size;
	CIw2DImage* image;
};

// ---------------------------------------------------------------

class Box : public DropTarget
{
public:
	CIwFVec2	Position;

	Box(CIwFVec2 pos, CIw2DImage*  img, CIwFVec2 siz)
	{
		Position = pos;
		size = siz;
		image = img;
	}

	virtual bool IsInside(CIwFVec2 pos, Draggable* source)
	{
		if ((pos-Position).GetLength()<35)
		{
			Position.x = rand()%240 + 20;
			Position.y = rand()%240 + 20;
			return true;
		}
		return false;
	}

	void Render()
	{
		Iw2DDrawImage(image, Position - size/2, size);
	}

private:
	CIw2DImage* image;
	CIwFVec2	size;
};

// ---------------------------------------------------------------

CIw2DImage* targetImage=0;
CIw2DImage* blobImage=0;
DropTargetList *targets=0;
Box *t1=0, *t2=0;
Blob *b1=0, *b2=0;

void ExampleInit()
{
	gDragDropManager = new DragDropManager();

	targetImage = Iw2DCreateImage("target.png");
	blobImage = Iw2DCreateImage("blob.png");

	// Create targetList with DropTargets
	targets = new DropTargetList;
	t1 = new Box(CIwFVec2(160,40), targetImage, CIwFVec2(70,70));
	t2 = new Box(CIwFVec2(160,140), targetImage, CIwFVec2(70,70));
	targets->Add( t1);
	targets->Add( t2);

	//Create Dragables
	b1 = new Blob( CIwFVec2(40,40), blobImage, CIwFVec2(40,40), targets );
	b2 = new Blob( CIwFVec2(40,160), blobImage, CIwFVec2(40,40), targets );
	// Add draggables to manager
	gDragDropManager->Draggables.push_back(b1);
	gDragDropManager->Draggables.push_back(b2);
}

void ExampleTerminate()
{
	delete gDragDropManager;
	delete b1;
	delete b2;
	delete t1;
	delete t2;
	delete targets;
	delete blobImage;
	delete targetImage;
}

void ExampleUpdate()
{
	// Draw drop targets (Box's)
	std::list<DropTarget*>::iterator itr;
	for(itr=targets->List.begin(); itr!=targets->List.end(); ++itr)
		((Box*) (*itr))->Render();
	// draw draggable blobs
	b1->Render();
	b2->Render();

	Iw2DSurfaceShow();
}




        

