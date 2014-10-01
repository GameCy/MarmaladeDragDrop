#include "PointerProxy.h"
#include "s3eSurface.h"


PointerProxy* gpPointerProxy=0;

PointerProxy* PointerProxy::singleton()
{
	if (!gpPointerProxy)
	{
		gpPointerProxy = new PointerProxy();
		gpPointerProxy->Touches = new TouchInfo[MAX_TOUCHES];
	}
	return gpPointerProxy;
}

void PointerProxy::DestroySingleton()
{
	if (gpPointerProxy)	
	{
		if (gpPointerProxy->Touches)
			delete gpPointerProxy->Touches;
		delete gpPointerProxy;
		gpPointerProxy = 0;
	}
}

//Find an active touch with the specified id, or allocate a free one from the list
TouchInfo* PointerProxy::GetTouch(int32 id)
{
    TouchInfo* pInActive = NULL;

    for (uint32 i = 0; i < MAX_TOUCHES; i++)
    {
        if (id == Touches[i].id)
            return &Touches[i];
        if (!Touches[i].active)
            pInActive = &Touches[i];
    }

    //Return first inactive touch
    if (pInActive)
    {
        pInActive->id = id;
        return pInActive;
    }

    return NULL;
}


void PointerProxy::MultiTouchButtonCB(s3ePointerTouchEvent* ev)
{
    TouchInfo* touch = singleton()->GetTouch(ev->m_TouchID);
    if (touch)
    {
		touch->active = ev->m_Pressed != 0;
        touch->x = ev->m_x;
        touch->y = ev->m_y;
		singleton()->ClickEvents.push_back( TouchInfo(touch));
    }
}

void PointerProxy::MultiTouchMotionCB(s3ePointerTouchMotionEvent* ev)
{
    TouchInfo* touch = singleton()->GetTouch(ev->m_TouchID);
    if (touch)
    {
        touch->x = ev->m_x;
        touch->y = ev->m_y;
		singleton()->MoveEvents.push_back( TouchInfo(touch) );
    }
}

void PointerProxy::SingleTouchButtonCB(s3ePointerEvent* ev)
{
	TouchInfo *touch = &singleton()->Touches[0];
	touch->active = (ev->m_Pressed != 0);
    touch->x = ev->m_x;
    touch->y = ev->m_y;

	singleton()->ClickEvents.push_back( TouchInfo(touch) );
    //sprintf(g_TouchEventMsg, "`x666666Touch %s", event->m_Pressed ? "PRESSED" : "RELEASED");
}

void PointerProxy::SingleTouchMotionCB(s3ePointerMotionEvent* ev)
{
	TouchInfo *touch = &singleton()->Touches[0];
    touch->x = ev->m_x;
    touch->y = ev->m_y;

	uint32 siz  = singleton()->MoveEvents.size();
	singleton()->MoveEvents.push_back( TouchInfo(touch) );
}

void PointerProxy::RegisterCallbacks()
{
    // An OS which supports multi-touch will return TOUCH_EVENTs if they are registered for.
    // An OS which does not will only raise standard pointer events.
    singleton()->bMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;

    if (singleton()->bMultiTouch)
    {
        //Register for multi touch events on platforms where the functionality is available.
        s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB, NULL);
        s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, NULL);
    }
    else
    {
        //Register for standard pointer events
        s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchButtonCB, NULL);
        s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCB, NULL);
    }
}

void PointerProxy::Unregister()
{
    //Clear up
    if (singleton()->bMultiTouch)
    {
        s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB);
        s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB);
    }
    else
    {
        s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)SingleTouchButtonCB);
        s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)SingleTouchMotionCB);
    }
}

