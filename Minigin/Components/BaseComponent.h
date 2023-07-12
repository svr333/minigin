#pragma once

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) = delete;
		BaseComponent& operator= (const BaseComponent&) = delete;
		BaseComponent& operator= (const BaseComponent&&) = delete;

		virtual void Update(float deltaTime) = 0;
		virtual void Render() const = 0;

	protected:
		BaseComponent(GameObject* pOwner) { m_pOwner = pOwner; };
		GameObject* m_pOwner;
	};
}
