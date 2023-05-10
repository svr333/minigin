#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "TransformComponent.h"
#include <algorithm>

namespace dae
{
	class Texture2D;
	class BaseComponent;

	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(float deltaTime);
		virtual void Render() const;

		TransformComponent& GetTransform() { return m_Transform; };

		template <typename T>
		T* GetComponent() const
		{
			for (size_t i = 0; i < m_pComponents.size(); i++)
			{
				auto cast = dynamic_cast<T*>(m_pComponents[i]);

				if (cast)
				{
					return cast;
				}
			}

			return nullptr;
			
			// am dumb
			//auto isValid = [](std::shared_ptr<T> comp) { return dynamic_pointer_cast<T>(comp); };
			//return std::find_if(m_pComponents.begin(), m_pComponents.end(), isValid);
		};

		template <typename T>
		bool HasComponent() { return GetComponent(); };

		void AddComponent(BaseComponent* comp) { m_pComponents.push_back(comp); };
		void RemoveComponent(BaseComponent* comp) { m_pComponents.erase(std::find(m_pComponents.begin(), m_pComponents.end(), comp)); };

		GameObject* GetParent() { return m_pParent; };
		void SetParent(GameObject* pParent, bool keepWorldPosition = true);
		const std::vector<GameObject*>& GetChildren() { return m_pChildren; };

	private:
		TransformComponent m_Transform = TransformComponent(this);
		std::vector<BaseComponent*> m_pComponents;

		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_pChildren{};
	};
}
