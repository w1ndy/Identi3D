//
// File: System.cc
// ===============
//

#include "System.h"
#include "EventDispatcher.h"
#include "SettingsWindow.h"

#include <src/renderer/SkinManager.h>
#include <src/renderer/Renderer.h>

#include <src/utils/DebugManager.h>
#include <src/utils/SettingManager.h>

#define _LOG_DEBUG_MESSAGE(verbose, msg, ...) \
	{ \
		if(_debugger) \
			_debugger->print(__FILE__, __LINE__, verbose, msg, __VA_ARGS__); \
	}

#define _LOG_ERROR(msg, ...) _LOG_DEBUG_MESSAGE(false, msg, __VA_ARGS__)
#define _LOG_INFO(msg, ...) _LOG_DEBUG_MESSAGE(true, msg, __VA_ARGS__)

#define _ERROR_MSGBOX(msg) MessageBoxA(NULL, msg, "Error", MB_ICONERROR | MB_OK)
#define _WARNING_MSGBOX(msg) MessageBoxA(NULL, msg, "Warning", MB_ICONWARNING | MB_OK)

#define _DELETE_AND_SET_NULL(ptr) \
	{ \
		delete ptr; \
		ptr = NULL; \
	}

namespace Identi3D
{

	System::System(void)
		: _confmgr(NULL), _dispatcher(NULL), _skinmgr(NULL), _renderer(NULL), 
		  _state(SystemState_NotInitialized)
	{
		// Initialize debugger.
		_debugger = ntnew DebugManager();
		if(_debugger == NULL) {
			_WARNING_MSGBOX(W_SYSTEM_CREATE_DEBUGGER_FAILURE);
		}
	}

	System::~System(void)
	{
		release();
		delete _debugger;
	}

	bool System::init(const wchar_t *config_name, const SystemStartupProperties &prop)
	{
		// Already initialized.
		if(_state != SystemState_NotInitialized) return true;

		try
		{
			_dispatcher = ntnew EventDispatcher(_debugger);
			if(_dispatcher == NULL) {
				throw std::runtime_error(E_SYSTEM_CREATE_DISPATCHER_FAILURE);
			}

			_skinmgr = ntnew SkinManager();
			if(_skinmgr == NULL) {
				throw std::runtime_error(E_SYSTEM_CREATE_SKINMGR_FAILURE);
			}

			_confmgr = ntnew SettingManager(_debugger);
			if(_confmgr == NULL) {
				throw std::runtime_error(E_SYSTEM_CREATE_CONFMGR_FAILURE);
			}

			// Load configuration.
			if(config_name) {
				_LOG_INFO(I_SYSTEM_LOADING_CONFIGURATION, config_name);

				if(_confmgr->load(config_name)) {
					// Load successfully.
					_LOG_INFO(I_SYSTEM_CONFIGURATION_LOAD_SUCCESS);
				} else {
					_LOG_ERROR(W_SYSTEM_CONFIGURATION_LOAD_FAILURE);
					_WARNING_MSGBOX(W_SYSTEM_CONFIGURATION_LOAD_FAILURE);
					if(prop.disallow_fallback_config) {
						// If fallback configuration is not acceptable.
						throw std::runtime_error(E_FATAL_ERROR);
					} else {
						// Set default options.
						_confmgr->getOptionTree()->clean();
					}
				}
			}
		
			if(prop.show_config_dialog) {
				switch(SettingsWindow::show(*_confmgr->getOptionTree()))
				{
				case SettingsWindowResult_Modified:
					_confmgr->save();
					break;
				case SettingsWindowResult_NoModification:
					break;
				case SettingsWindowResult_Cancelled:
				default:
					throw std::runtime_error(W_SYSTEM_USERS_CANCELLED);
				}
			}

			if(NULL == createRenderer()) {
				throw std::runtime_error(E_SYSTEM_CREATE_RENDERER_FAILURE);
			}
		} catch(std::exception &e) {
			_LOG_ERROR(e.what());
			_ERROR_MSGBOX(e.what());
			releaseRenderer();
			_DELETE_AND_SET_NULL(_confmgr);
			_DELETE_AND_SET_NULL(_skinmgr);
			_DELETE_AND_SET_NULL(_dispatcher);
			return false;
		}

		_state = SystemState_Idle;
		_LOG_INFO(I_SYSTEM_CREATE_SUCCESS);

		return true;
	}

	bool System::release()
	{
		// System not initialized.
		if(_state == SystemState_NotInitialized) return true;
		

		// Stop listener.
		if(_state == SystemState_Listening) kill();

		// Kill renderer.
		releaseRenderer();
		
		// Release modules
		_DELETE_AND_SET_NULL(_confmgr);
		_DELETE_AND_SET_NULL(_skinmgr);
		_DELETE_AND_SET_NULL(_dispatcher);
		
		_state = SystemState_NotInitialized;
		_LOG_INFO(I_SYSTEM_RELEASED);

		return S_OK;
	}

	Renderer *System::createRenderer(void)
	{
		if(_renderer) delete _renderer;
		_renderer = ntnew Renderer(_debugger);
		if(_renderer == NULL) {
			if(_debugger)
				_debugger->print(__FILE__, __LINE__, false, E_SYSTEM_CREATE_RENDERER_FAILURE);
			MessageBoxA(NULL, E_SYSTEM_CREATE_RENDERER_FAILURE, "Error", MB_ICONERROR | MB_OK);
			return NULL;
		}

		return _renderer;
	}

	void System::releaseRenderer(void)
	{
		delete _renderer;
		_renderer = NULL;
	}
	
	int System::start(void)
	{
		MSG msg;

		if(_state != SystemState_Idle) return -255;

		_state = SystemState_Listening;

		while(true) 
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(msg.message == WM_QUIT) break;

			if(_renderer->getDevice()->isRunning()) {
				if(!_renderer->getDevice()->startRendering(true, true, true)) {
					kill();
				} else {
					_dispatcher->postEvent(Event_Rendering);
					_renderer->getDevice()->endRendering();
				}
			}
		}
		
		_state = SystemState_Idle;
		return static_cast<int>(msg.wParam);
	}

	void System::kill(void)
	{
		if(isInitialized()) {
			_dispatcher->postEvent(Event_Terminate);
		}
		//if(_renderer)
		//	_renderer->releaseRenderWindow();
	}

};
