#pragma once
#include "BaseComponent.h"
#include <vector>
#include "../Commands/Command.h"
#include "TextComponent.h"
#include "../Entities/GameObject.h"

namespace dae
{
	class MenuComponent : public BaseComponent
	{
	public:
		struct Button
		{
			GameObject* Obj;
			float X;
			float Y;
			Command* Command;
			SDL_Color Color;
			SDL_Color ActiveColor;
		};

		MenuComponent(GameObject* pOwner);
		~MenuComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		std::shared_ptr<GameObject> AddButton(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color, const SDL_Color& activeColor, float x, float y, Command* command);
		void SelectButton(int buttonIndex);
		void ExecuteButton(int buttonIndex = -1);
		void SelectNextButton(bool previous = false);

	private:
		std::vector<Button> m_Buttons;
		int m_ActiveButton = 0;

		bool m_NeedsUpdate;
	};
}
