#include "MenuComponent.h"
#include "TextComponent.h"
#include "../Entities/Font.h"

dae::MenuComponent::MenuComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
	, m_NeedsUpdate(false)
{
}

dae::MenuComponent::~MenuComponent()
{
	// dont delete the GameObject, scene takes care of it
	for (size_t i = 0; i < m_Buttons.size(); i++)
	{
		delete m_Buttons[i].Command;
	}
}

void dae::MenuComponent::Update(float /*deltaTime*/)
{
	if (m_NeedsUpdate)
	{
		m_NeedsUpdate = false;

		for (int i = 0; i < m_Buttons.size(); i++)
		{
			auto color = i == m_ActiveButton ? m_Buttons[i].ActiveColor : m_Buttons[i].Color;
			auto text = m_Buttons[i].Obj->GetComponent<TextComponent>();

			text->SetColor(color);
		}
	}
}

void dae::MenuComponent::Render() const
{
}

std::shared_ptr<dae::GameObject> dae::MenuComponent::AddButton(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& colour, const SDL_Color& activeColour, float x, float y, Command* command)
{
	// make new text obj for each button
	auto obj = std::make_shared<GameObject>();
	
	obj->AddComponent(new TextComponent(obj.get(), text, font, colour));
	obj->GetTransform().SetLocalPosition({x, y, 0});

	m_Buttons.push_back(Button{ obj.get(), x, y, command, colour, activeColour });
	obj->SetParent(m_pOwner);

	m_NeedsUpdate = true;
	return obj;
}

void dae::MenuComponent::SelectButton(int buttonIndex)
{
	if (buttonIndex >= m_Buttons.size())
	{
		return;
	}

	m_ActiveButton = buttonIndex;
	m_NeedsUpdate = true;
}

void dae::MenuComponent::ExecuteButton(int buttonIndex)
{
	if (buttonIndex == -1)
	{
		m_Buttons[m_ActiveButton].Command->Execute(0.0f);
		return;
	}

	if (buttonIndex >= m_Buttons.size())
	{
		return;
	}

	m_Buttons[buttonIndex].Command->Execute(0.0f);
}

void dae::MenuComponent::SelectNextButton(bool previous)
{
	if (previous)
	{
		m_ActiveButton = m_ActiveButton - 1 < 0 ? (int)m_Buttons.size() - 1 : m_ActiveButton - 1;
	}
	else
	{
		m_ActiveButton = m_ActiveButton + 1 >= (int)m_Buttons.size() ? 0 : m_ActiveButton + 1;
	}

	m_NeedsUpdate = true;
}
