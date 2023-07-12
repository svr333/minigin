#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"

dae::InputManager::InputManager()
{
	m_PreviousKeyboardState.reserve(10);
	m_CurrentKeyboardState.reserve(10);
}

dae::InputManager::~InputManager()
{
	for (size_t i = 0; i < m_KeyboardCommands.size(); i++)
	{
		delete m_KeyboardCommands[i].command;
	}
	for (size_t i = 0; i < m_ControllerCommands.size(); i++)
	{
		delete m_ControllerCommands[i].command;
	}
}

bool dae::InputManager::ProcessInput(float deltaTime)
{
	// keyboard
	m_PreviousKeyboardState = m_CurrentKeyboardState;
	//m_CurrentKeyboardState.clear();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			const auto button = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentKeyboardState[button] = true;
		}
		if (e.type == SDL_KEYUP)
		{
			const auto button = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentKeyboardState[button] = false;
		}

		// in case I ever need mouse input
		// if (e.type == SDL_MOUSEBUTTONDOWN)

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	// keyboard
	for (size_t i = 0; i < m_KeyboardCommands.size(); i++)
	{
		auto cmd = m_KeyboardCommands[i];

		if (cmd.sceneName != SceneManager::GetInstance().GetActiveScene()->GetName())
		{
			continue;
		}

		switch (cmd.buttonState)
		{
		case ButtonState::Up:
			if (m_PreviousKeyboardState[cmd.keyboardScancode] && !m_CurrentKeyboardState[cmd.keyboardScancode])
				cmd.command->Execute(deltaTime);
			break;
		case ButtonState::Down:
			if (!m_PreviousKeyboardState[cmd.keyboardScancode] && m_CurrentKeyboardState[cmd.keyboardScancode])
				cmd.command->Execute(deltaTime);
			break;
		case ButtonState::Pressed:
			if (m_PreviousKeyboardState[cmd.keyboardScancode] && m_CurrentKeyboardState[cmd.keyboardScancode])
				cmd.command->Execute(deltaTime);
			break;
		}
	}

	// controllers
	for (size_t i = 0; i < m_pControllers.size(); i++)
	{
		m_pControllers[i]->Update();

		for (size_t j = 0; j < m_ControllerCommands.size(); j++)
		{
			switch (m_ControllerCommands[j].buttonState)
			{
			case ButtonState::Down:
				if (m_pControllers[i]->IsDown(m_ControllerCommands[j].button))
				{
					m_ControllerCommands[j].command->Execute(deltaTime);
				}
				break;
			case ButtonState::Up:
				if (m_pControllers[i]->IsUp(m_ControllerCommands[j].button))
				{
					m_ControllerCommands[j].command->Execute(deltaTime);
				}
				break;
			case ButtonState::Pressed:
				if (m_pControllers[i]->IsPressed(m_ControllerCommands[j].button))
				{
					m_ControllerCommands[j].command->Execute(deltaTime);
				}
				break;
			}
		}
	}

	return true;
}
