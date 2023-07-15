#include "Scene.h"
#include "../Entities/GameObject.h"
#include "../Events/EventManager.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	auto scene = object->GetScene();

	if (scene)
	{
		scene->Remove(object);
	}

	object->SetScene(this);
	m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		object->Update(deltaTime);
	}

	bool objRemoved = false;

	for (auto& object : m_Objects)
	{
		if (object->IsDestroyed())
		{
			objRemoved = true;
			EventManager::GetInstance().QueueEvent(std::make_shared<ObjectDestroyedEvent>(object.get()));
		}
	}

	auto remover = [&](std::shared_ptr<GameObject>& obj) { return obj->IsDestroyed(); };
	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), remover), m_Objects.end());
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}
