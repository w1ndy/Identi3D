//
// File: SettingManager.h
// ======================
// Load and save configuration on I/O level.
//

#ifndef IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H
#define IDENTI3D_SRC_UTILS_SETTINGSMANAGER_H

#include <src/identi3d/General.h>
#include <src/utils/DebugManager.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

	class __declspec(dllexport) SettingManager : public DebugFrame
	{
		friend class System;

	private:
		OptionTree _tree;
		std::wstring _conf_path;

	private:
		SettingManager(SettingManager &mgr);
		SettingManager &operator=(SettingManager &rhs);

	private:
		void saveElementRecursively(const OptionElement *elem, std::wofstream &fout);

	public:
		SettingManager(DebugManager *debugger = NULL) 
			: DebugFrame(debugger), _tree(debugger) {} ;
		~SettingManager(void);

		/*
		 * Load configuration from file.
		 */
		bool load(const std::wstring &path);

		/*
		 * Reload configuration from current file.
		 */
		bool reload(void);

		/*
		 * Write configuration to current file.
		 */
		bool save(void);

		/*
		 * Get the handle of global OptionTree.
		 */
		inline OptionTree *getOptionTree(void)
		{
			return &_tree;
		}

		/*
		 * Set a debug manager.
		 */
		void setDebugManager(DebugManager *debugger = NULL)
		{
			this->DebugFrame::setDebugManager(debugger);
			_tree.setDebugManager(debugger);
		}
	};

};

#endif // IDENTI3D_SRC_UTILS_SETTINGMANAGER_H