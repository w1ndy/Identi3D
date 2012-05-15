//
// File: SettingsManager.cc
// ========================
//

#include <src/utils/SettingsManager.h>
#include <src/utils/DebugManager.h>
#include <src/identi3d/IdentiExceptions.h>

namespace Identi3D
{

	SettingsManager::~SettingsManager(void)
	{
		if(_tree.getStatus()) save();
	}

	bool SettingsManager::load(const std::wstring &path)
	{
		std::wifstream fin;
		int correct, total;
		std::wstring name, value;
		
		_conf_path = path;
		try{
			_tree.clean();
			correct = 0, total = 0;

			fin.open(path);
			if(!fin.is_open()) throw std::runtime_error(E_FILE_OPEN_FAILURE);

			// TODO: parse the file in a more elegant way.
			while(!fin.eof()) {
				fin >> name >> value;
				if(name.length() == 0 || value.length() == 0) continue;
				if(_tree.addElement(name, value)) correct++;
				total++;
			}

			fin.close();
			_printVerboseMessage(__FILE__, __LINE__, I_SETTINGS_LOAD_COMPLETED, path, correct, total);
		} catch(std::exception &e) {
			_printMessage(__FILE__, __LINE__, e.what());
			_tree.clean();
			fin.close();
			return false;
		}
		_tree.resetStatus();
		return true;
	}

	bool SettingsManager::reload(void)
	{
		return load(_conf_path);
	}

	bool SettingsManager::save(void)
	{
		std::wofstream fout;

		fout.open(_conf_path);
		if(!fout.is_open()) {
			_printMessage(__FILE__, __LINE__, E_FILE_OPEN_FAILURE);
			fout.close();
			return false;
		}

		saveElementRecursively(NULL, fout);
		fout.close();
		_tree.resetStatus();
		return true;
	}

	void SettingsManager::saveElementRecursively(const OptionElement *elem, std::wofstream &fout)
	{
		OptionIterator iter((elem == NULL) ? _tree.getRootIterator() : elem->child);
		
		while(!iter.end()) {
			if(iter->child) {
				saveElementRecursively(iter.get(), fout);
			} else {
				fout << iter->name << L" " << iter->value << std::endl;
			}
			++iter;
		}
	}

};
