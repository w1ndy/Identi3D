//
// File: PluginManager.cc
// =======================
//

#include "PluginManager.h"
#include "System.h"

namespace Identi3D
{

	PluginManager::PluginManager(OptionTree *global_conf, DebugManager *debugger)
		: DebugFrame(debugger)
	{
		if(!global_conf || 
			(_location = global_conf->getValue(L"System.PluginsLocation")).length() == 0) {
				_location = DefaultPluginLocation;
		}
	}

	PluginManager::~PluginManager(void)
	{
		DESTROYPLUGINFUNC func;
		std::vector<PluginInfo>::iterator iter;
		for(iter = _plugins.begin(); iter != _plugins.end(); ++iter) {
			iter->plugin->release(&(System::instance()));
			func = (DESTROYPLUGINFUNC)GetProcAddress(iter->handle, "DestroyPlugin");
			if(func != NULL)
				func(iter->plugin);
			FreeLibrary(iter->handle);
		}
	}

	bool PluginManager::loadAllPlugin(void)
	{
		if(_plugins.size() != 0) {
			_printMessage(__FILE__, __LINE__, E_PS_PLUGINS_ALREADY_LOADED);
			return false;
		}

		std::wifstream fin((_location + L"/plugins.lst").c_str());
		if(!fin.is_open()) {
			_printMessage(__FILE__, __LINE__, E_PS_NO_PLUGIN_LIST_FOUND, _location.c_str());
			return false;
		}

		std::wstring path;
		PluginInfo info;
		CREATEPLUGINFUNC func;
		unsigned int entries_processed = 0;
		while(!fin.eof())
		{
			std::getline(fin, path);
			if(path.length() <= 1) continue;
			info.handle = LoadLibrary(path.c_str());
			if(info.handle != NULL) {
				func = (CREATEPLUGINFUNC)GetProcAddress(info.handle, "CreatePlugin");
				if(func != NULL) {
					info.plugin = func(info.handle);
					if(info.plugin != NULL) {
						if(info.plugin->initialize(&(System::instance()))) {
							_plugins.push_back(info);
						} else FreeLibrary(info.handle);
					} else {
						FreeLibrary(info.handle);
					}
				} else {
					FreeLibrary(info.handle);
				}
			}
			entries_processed++;
		}

		_printVerboseMessage(__FILE__, __LINE__, I_PS_PLUGIN_LOAD_STATS, _plugins.size(), entries_processed);
		return true;
	}

	void PluginManager::unloadAllPlugin(void)
	{
		DESTROYPLUGINFUNC func;
		std::vector<PluginInfo>::iterator iter;
		for(iter = _plugins.begin(); iter != _plugins.end(); ++iter) {
			iter->plugin->release(&(System::instance()));
			func = (DESTROYPLUGINFUNC)GetProcAddress(iter->handle, "DestroyPlugin");
			if(func != NULL)
				func(iter->plugin);
			FreeLibrary(iter->handle);
		}
		_plugins.clear();
	}

	bool PluginManager::reloadAllPlugin(void)
	{
		unloadAllPlugin();
		return loadAllPlugin();
	}

	void PluginManager::reloadConfig(OptionTree *global_conf)
	{
		if(!global_conf || 
			(_location = global_conf->getValue(L"System.PluginsLocation")).length() == 0) {
				_location = DefaultPluginLocation;
		}
	}

	bool PluginManager::isPluginLoaded(const std::wstring &name)
	{
		std::vector<PluginInfo>::iterator iter;
		for(iter = _plugins.begin(); iter != _plugins.end(); ++iter) {
			if(name == iter->plugin->getName()) {
				return true;
			}
		}
		return false;
	}

}
