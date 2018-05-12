#include "PlayInfo.h"
#include "Role.h"

PlayInfo::PlayInfo()
{
}

PlayInfo::~PlayInfo()
{
}

void PlayInfo::SynInfo(Role *obj)
{

	obj->_roleMaxHealth = _roleMaxHealth;
	obj->_roleCurrHealth = _roleCurrHealth;
	obj->_roleMaxEnergy = _roleMaxEnergy;
	obj->_roleCurrEnergy = _roleCurrEnergy;
	obj->_roleAttackValue = _roleAttackValue;
	obj->_roleDefenseValue = _roleDefenseValue;
	obj->_roleSpeed = _roleSpeed;
	obj->_roledirection = _roledirection;
	
	obj->_role_x = _role_x;
	obj->_role_y = _role_y;
}
