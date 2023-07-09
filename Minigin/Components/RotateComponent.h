#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class RotateComponent : public BaseComponent
	{
	public:
		RotateComponent(GameObject* pOwner, float radius = 15, float rotateSpeed = 200, bool clockwise = true);
		virtual ~RotateComponent() = default;

		RotateComponent(const RotateComponent&) = delete;
		RotateComponent(RotateComponent&&) = delete;
		RotateComponent& operator= (const RotateComponent&) = delete;
		RotateComponent& operator= (const RotateComponent&&) = delete;

		void Update(float deltaTime) override;
		void Render() const override {};

		void ToggleRotate() { m_ShouldRotate = !m_ShouldRotate; };

	private:
		float m_Angle = 0.0f;
		const float m_RotateSpeed = 200.0f;
		const float m_Radius = 15.0f;

		bool m_ShouldRotate = true;
		bool m_Clockwise = true;

		glm::vec3 m_CurrentPos{};
	};
}
