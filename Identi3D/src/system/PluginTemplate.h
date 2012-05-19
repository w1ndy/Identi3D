//
// File: PluginTemplate.h
// ======================
// A template for plugin interface.
//

#ifndef IDENTI3D_SRC_SYSTEM_PLUGINTEMPLATE_H
#define IDENTI3D_SRC_SYSTEM_PLUGINTEMPLATE_H

#include <src/identi3d/General.h>

namespace Identi3D
{

	class Plugin
	{
	public:
		Plugin(void) {};
		virtual ~Plugin(void) {};

		/*
		 * Initialize plugin.
		 */
		virtual bool initialize(System *syshandle) = 0;

		/*
		 * Release plugin.
		 */
		virtual void release(System *syshandle) = 0;

		/*
		 * Get the name of plugin.
		 */
		virtual const wchar_t *getName(void) = 0;
	};

	extern "C" {
		typedef class Plugin *LPPLUGIN;

		LPPLUGIN CreatePlugin(HMODULE hPluginLibrary);
		void DestroyPlugin(LPPLUGIN hPlugin);

		typedef LPPLUGIN (*CREATEPLUGINFUNC)(HMODULE hPluginLibrary);
		typedef void (*DESTROYPLUGINFUNC)(LPPLUGIN hPlugin);
	}

}

#endif // IDENTI3D_SRC_SYSTEM_PLUGINTEMPLATE_H