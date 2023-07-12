#pragma once
#include "../Services/Singleton.h"
#include "Xbox360Controller.h"
#include "../Commands/Command.h"
#include <vector>
#include <unordered_map>
#include <string>

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
			std::string sceneName;
		};

		struct ControllerInput
		{
			unsigned int controllerIndex;
			Xbox360Controller::ControllerButton button;
			ButtonState buttonState;
			Command* command;
			std::string sceneName;
		};

		void AddKeyboardCommand(Command* command, ButtonState buttonState, SDL_Scancode button, const std::string& sceneName)
		{
			m_KeyboardCommands.push_back(KeyboardInput{ button, buttonState, command, sceneName });
		}

		void AddControllerCommand(Command* command, unsigned int ctrlIndex, ButtonState buttonState, Xbox360Controller::ControllerButton button, const std::string& sceneName)
		{
			m_ControllerCommands.push_back(ControllerInput{ ctrlIndex, button, buttonState, command, sceneName });
		}

	private:
		std::unordered_map<int, bool> m_CurrentKeyboardState;
		std::unordered_map<int, bool> m_PreviousKeyboardState;

		std::vector<Xbox360Controller*> m_pControllers;
		std::vector<KeyboardInput> m_KeyboardCommands;
		std::vector<ControllerInput> m_ControllerCommands;
	};
}
