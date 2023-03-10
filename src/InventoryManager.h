#pragma once
#ifndef __INVENTORY_MANAGER__
#define __INVENTORY_MANAGER__

#include <string>
#include <map>

class InventoryManager
{
public:

	static InventoryManager& Instance()
	{
		static InventoryManager instance; // Magic statics.
		return instance;
	}

	InventoryManager();

	~InventoryManager();

	//setter
	void SetInventory(const std::string& key, bool hasItem);

	//getter
	bool GetInventory(const std::string& key) const;

	std::map < std::string, bool > GetInventoryMap();

private:

	std::map < std::string, bool > m_inventory;


};

#endif
