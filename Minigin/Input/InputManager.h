#pragma once
#include "../Services/Singleton.h"
#include <vector>
#include "Xbox360Controller.h"
#include "../Commands/Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput(float deltaTime);

		enum class ButtonState
		{
			Up = 0,
			Down = 1,
			Pressed = 2
		};

		struct KeyboardInput
		{
			SDL_Scancode keyboardScancode;
			ButtonState buttonState;
			Command* command;
		};

		struct ControllerInput
		{
			unsigned int controllerIndex;
			Xbox360Controller::ControllerButton button;
			ButtonState buttonState;
			Command* command;
		};

		void AddKeyboardCommand(Command* command, ButtonState buttonState, SDL_Scancode button)
		{
			m_KeyboardCommands.push_back(KeyboardInput{ button, buttonState, command });
		}

		void AddControllerCommand(Command* command, unsigned int ctrlIndex, ButtonState buttonState, Xbox360Controller::ControllerButton button)
		{
			m_ControllerCommands.push_back(ControllerInput{ ctrlIndex, button, buttonState, command });
		}

	private:
		std::vector<KeyboardInput> m_CurrentKeyboardState;
		std::vector<KeyboardInput> m_PreviousKeyboardState;

		std::vector<Xbox360Controller*> m_pControllers;
		std::vector<KeyboardInput> m_KeyboardCommands;
		std::vector<ControllerInput> m_ControllerCommands;
	};
}
