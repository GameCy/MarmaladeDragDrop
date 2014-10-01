#include "s3e.h"
#include "IwDebug.h"
#include "IwGx.h"
#include "Iw2D.h"
#include "PointerProxy.h"
#include "DragDrop.h"

void ExampleInit();
void ExampleTerminate();
void ExampleUpdate();


void ConsumePointerProxyEvents()
{
	PointerProxy *proxy = PointerProxy::singleton();
	// consume up/down events
	TouchInfoDeque::iterator itr;
	for(itr=proxy->ClickEvents.begin(); itr!=proxy->ClickEvents.end(); ++itr)
	{
		if (itr->active)
			gDragDropManager->BeginDragging( MouseEventInfo(itr->id, itr->x, itr->y, ButtonDown) );
		else
			gDragDropManager->EndDragging( MouseEventInfo(itr->id, itr->x, itr->y, ButtonUp) );
	}
	proxy->ClickEvents.clear();

	// also feed motion only when position changes
	for(itr=proxy->MoveEvents.begin(); itr!=proxy->MoveEvents.end(); ++itr)
	{
		if (itr->active)
			gDragDropManager->UpdateDragging( MouseEventInfo(itr->id, itr->x, itr->y, ActiveMotion) );
	}
	proxy->MoveEvents.clear();
}

void Initialize()
{
    //Initialise graphics system(s)
    IwGxInit();
	Iw2DInit();
	// register for touch callbacks with marmalade system
	PointerProxy::singleton()->RegisterCallbacks();
}

void Terminate()
{
	PointerProxy::singleton()->Unregister();
	PointerProxy::singleton()->DestroySingleton();

	Iw2DTerminate();
    IwGxTerminate();
}

int main()
{
	Initialize();

	ExampleInit();

	IwGxSetColClear(0x2f, 0x3f, 0x3f, 0xff);
    
    // Loop forever, until the user or the OS performs some action to quit the app
    while (!s3eDeviceCheckQuitRequest())
    {
        //Update the input systems
        s3eKeyboardUpdate();
        s3ePointerUpdate();

		ConsumePointerProxyEvents();

        // Clear the surface
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
        
		ExampleUpdate();

        // Standard EGL-style flush of drawing to the surface
        IwGxFlush();
        IwGxSwapBuffers();
        s3eDeviceYield(0);
    }

	ExampleTerminate();

	Terminate();    
    // Return
    return 0;
}
