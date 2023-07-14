#pragma once
#include <Components/BaseComponent.h>
#include <string>
#include <vector>

namespace dae
{
	struct Enemy;

	class LevelComponent final : public BaseComponent
	{
	public:
		LevelComponent(GameObject* pOwner, const std::string& levelName);
		void InitializeEnemies();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

	private:
		std::string m_LevelName;

		// theres probably a better way to do it without having 2 separate values but this gives more control
		bool m_CurrentHorizontalDirectionLeft = true;
		bool m_CurrentVerticalDirectionUp = true;

		float m_CurrentHorizontalSwingTime = 0.0f;
		float m_CurrentVerticalSwingTime = 0.0f;

		const float m_MaxHorizontalSwingTime = 1.7f;
		const float m_MaxVerticalSwingTime = 0.7f;

		const float m_MaxHorizontalSwingDistance = 80.0f;
		const float m_VerticalSwingDistance = 15.0f;
	};
}
