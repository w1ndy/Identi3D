//
// File: PluginTemplate.cc
// =======================
//

#include "SamplePlugin.h"

namespace Identi3D
{

	LPPLUGIN CreatePlugin(HMODULE hPluginLibrary)
	{
		return ntnew SamplePlugin();
	}

	void DestroyPlugin(LPPLUGIN hPlugin)
	{
		delete hPlugin;
	}

}
