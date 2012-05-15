//
// File: EventListener.cc
// ======================
//

#include "EventListener.h"
#include "EventDispatcher.h"
#include "System.h"

#include <src/renderer/Renderer.h>
#include <src/identi3d/Basetypes.h>

#define hSystem System::instance()

namespace Identi3D
{

	bool EventListener::processRawPacket(const EventPacket &packet, EventResult &result)
	{
		try
		{
			switch(packet.event_message)
			{
			case Event_Rendering:
				onRendering(packet.time_since_last_frame);
				result = getLastResult();
				return true;
			case Event_KeyPressed:
				onKeyPressed((KeyType)(packet.param1), (UINT)(packet.param2), 
					packet.time_since_last_frame);
				result = getLastResult();
				return true;
			case Event_Terminate:
				onTerminate();
				return true;
			}
		} catch(...) {
			setLastResult(EventResult_FatalError);
			return false;
		}
		return false;
	}

	void EventListener::onRendering(UINT timeSinceLastFrame)
	{
		setLastResult(EventResult_OK);
	}

	void EventListener::onKeyPressed(KeyType key, UINT repeat_times, UINT timeSinceLastFrame)
	{
		switch(key)
		{
		case KeyType_Escape:
			hSystem.kill();
			break;
		}
		setLastResult(EventResult_OK);
	}

	void EventListener::onTerminate(void)
	{
		if(hSystem.isInitialized())
			hSystem.getRenderer()->releaseRenderWindow();
		setLastResult(EventResult_OK);
	}

};
