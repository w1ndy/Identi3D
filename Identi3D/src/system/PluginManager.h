//
// File: PluginManager.h
// =====================
// Provide plugins management
//

#ifndef IDENTI3D_SRC_SYSTEM_PLUGINMANAGER_H
#define IDENTI3D_SRC_SYSTEM_PLUGINMANAGER_H

#include "PluginTemplate.h"

#include <src/identi3d/General.h>
#include <src/utils/DebugFrame.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	static const wchar_t *DefaultPluginLocation = L"plugins";

	struct PluginInfo
	{
		Plugin *plugin;
		HMODULE handle;
	};

	class PluginManager : public DebugFrame
	{
	private:
		std::vector<PluginInfo> _plugins;
		std::wstring _location;

	public:
		PluginManager(OptionTree *global_conf, DebugManager *debugger = NULL);
		~PluginManager(void);

		/*
		 * Load all plugins under the location.
		 */
		bool loadAllPlugin(void);

		/*
		 * Unload all initialized plugins.
		 */
		void unloadAllPlugin(void);

		/*
		 * Reload plugins' location from global configuration.
		 */
		void reloadConfig(OptionTree *global_conf);

		/*
		 * Reload all plugins in current location.
		 */
		bool reloadAllPlugin(void);

		/*
		 * If specified plugin is loaded.
		 */
		bool isPluginLoaded(const std::wstring &name);
	};

}

#endif // IDENTI3D_SRC_SYSTEM_PLUGINMANAGER_H
