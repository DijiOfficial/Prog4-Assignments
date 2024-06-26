#include "Scene.h"

#include <algorithm>
#include <stdexcept>

diji::Scene::~Scene()
{
	m_ObjectsUPtrMap.clear();
};

diji::GameObject* diji::Scene::CreateGameObject(const std::string& name)
{
	if (m_ObjectsUPtrMap.find(name) != m_ObjectsUPtrMap.end())
		throw std::runtime_error("A GameObject with the given name already exists.");
	
	m_ObjectsUPtrMap[name] = (std::make_unique<GameObject>());
	return m_ObjectsUPtrMap.at(name).get();
}

void diji::Scene::Remove(GameObject* object)
{
	for (auto it = m_ObjectsUPtrMap.begin(); it != m_ObjectsUPtrMap.end(); ++it)
	{
		if (it->second.get() == object)
		{
			m_ObjectsUPtrMap.erase(it);
			break;
		}
	}
}

void diji::Scene::Remove(const std::string& name)
{
	auto it = m_ObjectsUPtrMap.find(name);
	if (it != m_ObjectsUPtrMap.end())
	{
		m_ObjectsUPtrMap.erase(it);
	}
}

void diji::Scene::RemoveAll()
{
	m_ObjectsUPtrMap.clear();
}

diji::GameObject* diji::Scene::GetGameObject(const std::string& name) const
{
	auto it = m_ObjectsUPtrMap.find(name);
	
	return it != m_ObjectsUPtrMap.end() ? it->second.get() : nullptr;
}

std::string diji::Scene::GetGameObjectName(const GameObject* object) const
{
	for (const auto& pair : m_ObjectsUPtrMap)
	{
		if (pair.second.get() == object)
		{
			return pair.first;
		}
	}
	throw std::runtime_error("GameObject not found in the scene.");
}

void diji::Scene::Update()
{
	for(auto& object : m_ObjectsUPtrMap)
	{
		object.second->Update();
	}
}

void diji::Scene::FixedUpdate()
{
	for (auto& object : m_ObjectsUPtrMap)
	{
		object.second->FixedUpdate();
	}
}

void diji::Scene::Init()
{
	for (const auto& object : m_ObjectsUPtrMap)
	{
		object.second->Init();
	}
}

void diji::Scene::Render() const
{
	for (const auto& object : m_ObjectsUPtrMap)
	{
		object.second->Render();
	}
}

void diji::Scene::LateUpdate()
{
	for (auto& object : m_ObjectsUPtrMap)
	{
		object.second->LateUpdate();
	}
}
