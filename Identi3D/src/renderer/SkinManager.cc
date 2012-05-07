//
// File: SkinManager.cc
// ====================
//

#include "SkinManager.h"

// TODO: Add implementation.

unsigned SkinManager::_skin_list_limit = 256;
unsigned SkinManager::_material_list_limit = 512;

SkinManager::SkinManager(void)
{
}

SkinManager::~SkinManager(void)
{
	SkinDataList::iterator siter;
	for(siter = _skins.begin(); siter != _skins.end(); ++siter) {
		delete (*_skins)->_mat;
		delete (*_skins);
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
									  float alpha = 1.0f)
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
	m = ntnew Material(md);
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
	s = ntnew Skin(sd);
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
	_skins.pushback(sd);
	return s;
}

Skin *SkinManager::createSkin(const Color &diffuse,
							  const Color &ambient,
							  const Color &specular,
							  const Color &emissive,
							  float specular_power,
							  float alpha = 1.0f)
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
