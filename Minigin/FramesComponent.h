#pragma once
#include "BaseComponent.h"

namespace dae
{
	class FramesComponent final : public BaseComponent
	{
	public:
		FramesComponent(GameObject* pOwner);
		virtual ~FramesComponent() = default;
		FramesComponent(const FramesComponent& other) = delete;
		FramesComponent(FramesComponent&& other) = delete;
		FramesComponent& operator=(const FramesComponent& other) = delete;
		FramesComponent& operator=(FramesComponent&& other) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

	private:
		float m_TextTimer = 0.0f;
		const float m_MaxTextTime = 1.0f;
		int m_FPS = 0;
	};
}
