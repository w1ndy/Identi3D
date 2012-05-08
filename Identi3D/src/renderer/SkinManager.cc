//
// File: SkinManager.cc
// ====================
//

#include "SkinManager.h"

// TODO: Add implementation.

namespace Identi3D
{

	unsigned SkinManager::_skin_list_limit = 256;
	unsigned SkinManager::_material_list_limit = 512;

	SkinManager::SkinManager(void)
	{
	}

	SkinManager::~SkinManager(void)
	{
		SkinDataList::iterator siter;
		for(siter = _skins.begin(); siter != _skins.end(); ++siter) {
			delete (*siter)->mat;
			delete (*siter);
		}
		MaterialDataList::iterator miter;
		for(miter = _materials.begin(); miter != _materials.end(); ++miter) {
			delete (*miter);
		}
	}

	Material *SkinManager::createMaterial(const Color &diffuse,
										  const Color &ambient,
										  const Color &specular,
										  const Color &emissive,
										  float specular_power,
										  float alpha)
	{
		MaterialData *md;
	
		md = ntnew MaterialData;
		if(md == NULL) {
			// TODO: printMessage
			return NULL;
		}
	
		md->diffuse = diffuse;
		md->ambient = ambient;
		md->specular = specular;
		md->emissive = emissive;
		md->power = specular_power;
		md->alpha = alpha;
	
		Material *m;
		m = ntnew Material(*md);
		if(m == NULL) {
			// TODO: printMessage
			delete md;
			return NULL;
		}
	
		if(_materials.size() >= _material_list_limit) {
			compress();
			if(_materials.size() >= _material_list_limit)
				_material_list_limit *= 2;
		}
		_materials.push_back(md);
		return m;
	}

	Skin *SkinManager::createSkin(const Material &mat)
	{
		SkinData *sd;
	
		sd = ntnew SkinData;
		if(sd == NULL) {
			// TODO: printMessage
			return NULL;
		}
	
		sd->mat = ntnew Material(mat);
		if(sd->mat == NULL) {
			// TODO: printMessage
			delete sd;
			return NULL;
		}
	
		Skin *s;
		s = ntnew Skin(*sd);
		if(s == NULL) {
			// TODO: printMessage
			delete sd->mat;
			delete sd;
			return NULL;
		}
	
		if(_skins.size() >= _skin_list_limit) {
			compress();
			if(_skins.size() >= _skin_list_limit)
				_skin_list_limit *= 2;
		}
		_skins.push_back(sd);
		return s;
	}

	Skin *SkinManager::createSkin(const Color &diffuse,
								  const Color &ambient,
								  const Color &specular,
								  const Color &emissive,
								  float specular_power,
								  float alpha)
	{
		Material *m = createMaterial(diffuse, ambient, specular, emissive, specular_power, alpha);
		if(m == NULL) {
			// TODO: printMessage
			return NULL;
		}
		Skin *s = createSkin(*m);
		if(s == NULL) {
			// TODO: printMessage
			delete m;
			return NULL;
		}
		delete m;
		return s;
	}

	void SkinManager::releaseMaterial(Material &mat)
	{
		SkinDataList::iterator siter;
		for(siter = _skins.begin(); siter != _skins.end(); ++siter) {
			if((*siter)->mat->_data == mat._data) {
				if((*siter)->ref_count != 0) {
					// TODO: print a warning: object is still being referenced.
				}
				delete (*siter);
				_skins.erase(siter);
			} else if((*siter)->ref_count == 0) {
				delete (*siter);
				_skins.erase(siter);
			}
		}
		MaterialDataList::iterator miter;
		for(miter = _materials.begin(); miter != _materials.end(); ++miter) {
			if((*miter) == mat._data) {
				if(--mat._data->ref_count != 0) {
					// TODO: print a warning: object is still being referenced.
				}
				delete (*miter);
				_materials.erase(miter);
				break;
			} else if((*miter)->ref_count == 0) {
				delete (*miter);
				_materials.erase(miter);
			}
		}
		delete &mat;
	}

	void SkinManager::releaseSkin(Skin &s)
	{
		SkinDataList::iterator iter;
		for(iter = _skins.begin(); iter != _skins.end(); ++iter) {
			if((*iter) == s._data) {
				if(--s._data->ref_count != 0) {
					// TODO: print a warning: object is still being referenced.
				}
				delete (*iter);
				_skins.erase(iter);
				break;
			} else if((*iter)->ref_count == 0) {
				delete (*iter);
				_skins.erase(iter);
			}
		}
		delete &s;
	}

	void SkinManager::compress(void)
	{
		SkinDataList::iterator siter;
		for(siter = _skins.begin(); siter != _skins.end(); ++siter) {
			if((*siter)->ref_count == 0) {
				delete (*siter);
				_skins.erase(siter);
			}
		}
		if(_skins.size() < _skin_list_limit / 2) _skin_list_limit /= 2;
	
		MaterialDataList::iterator miter;
		for(miter = _materials.begin(); miter != _materials.end(); ++miter) {
			if((*miter)->ref_count == 0) {
				delete (*miter);
				_materials.erase(miter);
			}
		}
		if(_materials.size() < _material_list_limit / 2) _material_list_limit /= 2;
	}

	void SkinManager::resetSkins(void)
	{
		SkinDataList::iterator iter;
		for(iter = _skins.begin(); iter != _skins.end(); ++iter) {
			if((*iter)->ref_count != 0) {
				// TODO: print a warning: object is still being referenced.
			}
			delete (*iter);
			_skins.erase(iter);
		}
	}

	void SkinManager::reset(void)
	{
		resetSkins();
		MaterialDataList::iterator iter;
		for(iter = _materials.begin(); iter != _materials.end(); ++iter) {
			if((*iter)->ref_count != 0) {
				// TODO: print a warning: object is still being referenced.
			}
			delete (*iter);
			_materials.erase(iter);
		}
	}

}