#pragma once

#include "Transform.h"
#include "../Components/TransformComponent.h"

#include <memory>
#include <vector>
#include <algorithm>

namespace dae
{
	class Texture2D;
	class BaseComponent;
	class Scene;

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
		bool HasComponent() { return GetComponent<T>(); };
		int GetComponentCount() { return (int)m_pComponents.size(); };

		void AddComponent(BaseComponent* comp) { m_pComponents.push_back(comp); };
		void RemoveComponent(BaseComponent* comp) { m_pComponents.erase(std::find(m_pComponents.begin(), m_pComponents.end(), comp)); };

		void SetScene(Scene* scene) { m_AttachedScene = scene; };
		Scene* GetScene() { return m_AttachedScene; };

		GameObject* GetParent() { return m_pParent; };
		void SetParent(GameObject* pParent, bool keepWorldPosition = true);
		const std::vector<GameObject*>& GetChildren() { return m_pChildren; };

		void MarkDestroy() { m_IsDestroyed = true; };
		bool IsDestroyed() { return m_IsDestroyed; };

		void SetVisible(bool isVisible) { m_ShouldRender = isVisible; };
		bool IsVisible() { return m_ShouldRender; };

	private:
		TransformComponent m_Transform = TransformComponent(this);
		std::vector<BaseComponent*> m_pComponents;

		Scene* m_AttachedScene = nullptr;
		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_pChildren{};

		bool m_IsDestroyed = false;
		
		// could have added this as a component but this seemed easier
		bool m_ShouldRender = true;
	};
}
