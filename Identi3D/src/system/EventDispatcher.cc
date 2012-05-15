//
// File: EventDispatcher.cc
// ========================
//

#include <src/system/EventDispatcher.h>
#include <src/system/EventListener.h>
#include <src/identi3d/IdentiExceptions.h>
#include <algorithm>

namespace Identi3D
{

	EventDispatcher::EventDispatcher(DebugManager *debugger) : DebugFrame(debugger)
	{
	}

	bool EventDispatcher::_dispatch(const EventPacket &packet, EventResult &result)
	{
		EventListenerList::iterator iter;
		for(iter = _hooks.begin(); iter != _hooks.end(); ++iter) {
			// Execute processors for each dispatcher.
			// Messages is only valid for first procedure processing them.
			if((*iter)->processRawPacket(packet, result)) return true;
		}
		// 'false' indicates message ignorance.
		result = EventResult_Abandoned;
		return false;
	}

	bool EventDispatcher::RegisterEventListener(EventListener &listener)
	{
		// Append listener pointer to the list.
		_hooks.push_back(&listener);
		return true;
	}

	void EventDispatcher::UnregisterEventListener(EventListener &listener)
	{
		// find and delete the handle.
		_hooks.erase(std::find(_hooks.begin(), _hooks.end(), &listener));
	}

	EventResult EventDispatcher::postWindowMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		EventPacket packet;
		EventResult retval;

		switch(msg)
		{
		case WM_KEYDOWN:
			// Fill the packet information.
			packet.event_message = Event_KeyPressed;
			packet.param1 = wparam;				// WPARAM indicates key type.
			packet.param2 = lparam & 0xffff;	// Lower part of LPARAM indicates repeat times.

			// TODO: substitute this parameters with data from Timeline
			packet.time_since_last_frame = 0;
			break;
		case WM_PAINT:
			// Ignore window paint message.
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			return 0;
		case WM_DESTROY:
			// DO NOT SEND terminate MESSAGE HERE
			// WHICH MAY LEADS TO DEAD LOOP.
			PostQuitMessage(0);
			return 0;
		default:
			// Redirect the rest of unprocessed messages.
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		// Dispatch the message.
		if(_dispatch(packet, retval)) return retval;

		// Else if not processed:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	EventResult EventDispatcher::postEvent(Event e, DWORD param1, DWORD param2)
	{
		EventPacket packet;
		EventResult retval;

		// Fill EventPacket structure.
		packet.event_message = e;
		packet.param1 = param1;
		packet.param2 = param2;

		// TODO: substitute this parameters with data from Timeline
		packet.time_since_last_frame = 0;

		// Dispatch the message.
		_dispatch(packet, retval);
		return retval;
	}

};
