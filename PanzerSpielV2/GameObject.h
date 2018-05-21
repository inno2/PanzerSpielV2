#pragma once
#include <vector>
#include "Handle.h"
#include "Component.h"
#include <utility>


class GameObject
{
public:
	inline bool HasComponent(ComponentType type)
	{
		for each(Handle comp in m_components)
		{
			if (comp.m_type == type)
				return true;
		}
		return false;
	}

	inline bool HasComponent(ComponentType type, Handle& comp_out)
	{
		for each(Handle comp in m_components)
		{
			if (comp.m_type == type)
			{
				comp_out = comp;
				return true;
			}				
		}
		comp_out.Invalidate();
		return false;
	}

	inline Handle GetComponent(ComponentType type)
	{
		for each(Handle comp in m_components)
		{
			if (comp.m_type == type)
			{
				return comp;
			}
		}
		return Handle::GetInvalid();
	}
	
	inline void AddComponent(Handle& comp_handle)
	{
		m_components.push_back(comp_handle);
	}

	inline void RemoveComponent(Handle handle)
	{
		std::vector<Handle>::iterator comp;
		int i = 0;
		for (comp = m_components.begin(); comp != m_components.end(); i++)
		{
			if (m_components[i] == handle)
			{
				m_components.erase(comp);
				return;
			}
		}
	}

private:
	std::vector<Handle> m_components;
};