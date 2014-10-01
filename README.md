MarmaladeDragDrop
=================

A simple implementation of a drag and drop system, with a working example for the marmalade game engine.

The included classes are:
 Dragable
 DropTarget
 DragDropManager
 
 DraggableList
 DropTargetList
 
 -------------------
 
 How it works:
  1. Derive the object you want to drag from Dragable:
      class Gem : public Dragable {}

  2. The object that accepts dropped objects should be derived from the DropTarget class:
      class Basket : public DropTarget {}

  3. Create a DragDropManager instance, and in your main loop call its event handlers on mouse events:
    gDragDropManager = new DragDropManager;
    ...
    MainLoop:
      if (MouseButtonDown) gDragDropManager->BeginDragging(MouseEventInfo( .. x,y,.....))
      if (MouseButtonMove) gDragDropManager->UpdateDragging(MouseEventInfo( ........))
      if (MouseButtonUp)   gDragDropManager->EndDragging(MouseEventInfo( .. x,y,.....))

  4. Create your DropTarget objects and add them to a DropTargetList 
  DropTargetList* targets = new DropTargetList;
	t1 = new Basket(...);
	t2 = new Basket(...);
	targets->Add( t1);
	targets->Add( t2);

  5. Create some Dragable objects, let them know which targets they can drop onto, and add them to the DragDropManager:
  gem1 = new Gem( targets, pos, image, etc...);
  gem2 = new Gem( targets, pos, image, etc...);
	gDragDropManager->Draggables.push_back( gem1 );
	gDragDropManager->Draggables.push_back( gem2);

  5. Imlement the IsInside methods of Dragable and DropTarget, such that they return true whenever the clicked point is inside your objects geometry.

  6. Of course make sure you render your objects on screen.


