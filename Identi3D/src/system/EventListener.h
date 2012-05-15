//
// File: EventListener.h
// =====================
// Listener of system event.
//

#ifndef IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H
#define IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H

#include <src/identi3d/General.h>

#define EventResult_OK			0x00000000
#define EventResult_Abandoned	0x00000001
#define EventResult_Error		0x80000001
#define EventResult_FatalError	0x8000FFFF

namespace Identi3D
{

	typedef DWORD EventResult;

	class EventListener
	{
	private:
		EventResult _last_result;

	protected:
		//// Message Processors ////

		/*
		 * To render objects on screen.
		 */
		virtual void onRendering(UINT timeSinceLastFrame);

		/*
		 * To process pressed key.
		 */
		virtual void onKeyPressed(KeyType key, UINT repeat_times, UINT timeSinceLastFrame);

		/*
		 * To terminate program. (Generally do not change the procedure.)
		 */
		virtual void onTerminate(void);

		////////////////////////////

	public:
		EventListener(void) : _last_result(EventResult_OK) {} ;
		virtual ~EventListener(void) {} ;

		/*
		 * Process raw packet dispatched from system.
		 */
		bool processRawPacket(const EventPacket &packet, EventResult &result);

		/*
		 * Set last result of message processing.
		 */
		void setLastResult(EventResult result = EventResult_OK)
		{
			_last_result = result;
		}

		/*
		 * Get last result of message processing.
		 */
		long getLastResult(void) const { return _last_result; }
	};

};

#endif // IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H