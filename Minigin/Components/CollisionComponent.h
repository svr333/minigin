#pragma once
#include "BaseComponent.h"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class CollisionComponent : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* pOwner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void SetBorderDimensions(float width, float height);

	private:
		bool CheckCollision(std::vector<std::shared_ptr<GameObject>>& m_Objects);

		glm::fvec2 m_Size {};
	};
}
