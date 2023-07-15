#include "GameObject.h"
#include "../Components/BaseComponent.h"
#include <algorithm>

dae::GameObject::~GameObject()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}

	m_pComponents.clear();

	// while loop because setparent removes from the vector
	while (!m_pChildren.empty())
	{
		m_pChildren[0]->SetParent(nullptr);
	}

	// remove ourself from parent
	if (m_pParent)
	{
		SetParent(nullptr);
	}
}

void dae::GameObject::Update(float deltaTime)
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Render();
	}
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (m_pParent)
	{
		std::erase(m_pParent->m_pChildren, this);
	}

	m_pParent = pParent;

	if (pParent)
	{
		if (keepWorldPosition)
		{
			m_Transform.SetLocalPosition(m_Transform.GetLocalPosition() - GetParent()->GetTransform().GetWorldPosition());
		}

		pParent->m_pChildren.push_back(this);
	}
	else
	{
		m_Transform.SetLocalPosition(m_Transform.GetWorldPosition());
	}
}
