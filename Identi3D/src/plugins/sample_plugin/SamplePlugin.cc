//
// File: SamplePlugin.cc
// =====================
//

#include "SamplePlugin.h"

#include <src/system/System.h>
#include <src/utils/DebugManager.h>

namespace Identi3D
{

	SamplePlugin::SamplePlugin(void)
	{
	}

	SamplePlugin::~SamplePlugin(void)
	{
	}

	bool SamplePlugin::initialize(System *syshandle)
	{
		DebugManager *debugger = syshandle->getDebugManager();
		if(debugger != NULL) {
			debugger->print(__FILE__, __LINE__, true, "Sample plugin loaded.");
		}
		return true;
	}

	void SamplePlugin::release(System *syshandle)
	{
		DebugManager *debugger = syshandle->getDebugManager();
		if(debugger != NULL) {
			debugger->print(__FILE__, __LINE__, true, "Sample plugin unloaded.");
		}
	}

}
