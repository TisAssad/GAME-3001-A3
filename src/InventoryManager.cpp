#include "InventoryManager.h"

InventoryManager::InventoryManager() = default;

InventoryManager::~InventoryManager() = default;

void InventoryManager::SetInventory(const std::string& key, bool hasItem)
{
	if (m_inventory.count(key) && !m_inventory.empty()) // checking if that inventory item already exists
	{
		m_inventory.at(key) = hasItem;
		return;
	}
	m_inventory.emplace(key, hasItem);

}

bool InventoryManager::GetInventory(const std::string& key) const
{
	return m_inventory.at(key);
}

std::map<std::string, bool> InventoryManager::GetInventoryMap()
{
	return m_inventory;
}
