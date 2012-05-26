//
// File: SamplePlugin.h
// ====================
// Provide a plugin sample for Identi3D.
//

#ifndef IDENTI3D_SRC_PLUGINS_SAMPLEPLUGIN_SAMPLEPLUGIN_H
#define IDENTI3D_SRC_PLUGINS_SAMPLEPLUGIN_SAMPLEPLUGIN_H

#include <src/identi3d/General.h>
#include <src/system/PluginTemplate.h>

namespace Identi3D
{

	class SamplePlugin : public Plugin
	{
	public:
		SamplePlugin(void);
		~SamplePlugin(void);

		/*
		 * Initialize plugin.
		 */
		bool initialize(System *syshandle);

		/*
		 * Release plugin.
		 */
		void release(System *syshandle);

		/*
		 * Get the name of plugin.
		 */
		const wchar_t *getName(void) { return L"SamplePlugin"; }
	};

}

#endif // IDENTI3D_SRC_PLUGINS_SAMPLEPLUGIN_SAMPLEPLUGIN_H
