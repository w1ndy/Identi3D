//
// File: App.cc
// ============
// 

#include "StdAfx.h"
#include "App.h"
#include <src/utils/CPU.h>

using namespace Identi3D;

App::App(void)
{
	_debugger = NULL;
	_window = NULL;
	_listener = NULL;
	_renderer = NULL;
	_device = NULL;
}

bool App::init(void)
{
	try
	{
		_listener = ntnew Listener;
		if(_listener == NULL) throw std::exception();

		_window = ntnew RenderWindow;
		if(_window == NULL) {
			throw std::exception();
		}

		SystemStartupProperties prop;
		if(!System::instance().init(L"default.conf", prop)) 
			throw std::exception();
	
		System::instance().getEventDispatcher()->RegisterEventListener(*_listener);

		_renderer = System::instance().getRenderer();
		if(!_renderer->createDefaultDevice()) {
			throw std::exception();
		}
	
		if(!_renderer->assignRenderWindow(*_window, __T("Identi3D Test"))) {
			throw std::exception();
		}

		// Test code:
#if defined (PLUGINMANAGER_TEST)
		_plugmgr = ntnew PluginManager(System::instance().getSettingManager()->getOptionTree(),
			System::instance().getDebugManager());
		if(!_plugmgr) throw std::exception();
		_plugmgr->loadAllPlugin();
#endif // PLUGINMANAGER_TEST

	} catch(...) {
		System::instance().release();
		delete _window;
		_window = NULL;
		delete _listener;
		_listener = NULL;
	}

	_device = _renderer->getDevice();
	_debugger = System::instance().getDebugManager();
	return true;
}

int App::run(void)
{
	_device->setClearColor(0.8f, 0.8f, 0.8f);
	return System::instance().start();
}

App::~App(void)
{
#if defined (PLUGINMANAGER_TEST)
	delete _plugmgr;
#endif // PLUGINMANAGER_TEST

	System::instance().release();
	delete _window;
	delete _listener;
}
