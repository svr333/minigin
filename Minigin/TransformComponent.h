#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	class TransformComponent : public BaseComponent
	{
	public:
		TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) = delete;
		TransformComponent& operator= (const TransformComponent&) = delete;
		TransformComponent& operator= (const TransformComponent&&) = delete;

		void Update(float) override {};
		void Render() const override {};

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetRotation() const { return m_Rotation; };
		const glm::vec3& GetScale() const { return m_Scale; };

		void SetPositionDirty();
		void SetLocalPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; };
		void SetScale(const glm::vec3& scale) { m_Scale = scale; };

	private:
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};
		bool m_IsDirty = true;

		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{};
	};
}
