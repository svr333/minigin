#include "Xbox360Controller.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

using namespace dae;

class Xbox360Controller::Xbox360ControllerImpl
{
public:
	Xbox360ControllerImpl(unsigned int controllerIndex);
	~Xbox360ControllerImpl() = default;
	Xbox360ControllerImpl(const Xbox360ControllerImpl& other) = delete;
	Xbox360ControllerImpl(Xbox360ControllerImpl&& other) = delete;
	Xbox360ControllerImpl& operator=(const Xbox360ControllerImpl& other) = delete;
	Xbox360ControllerImpl& operator=(Xbox360ControllerImpl&& other) = delete;

	void Update();

	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};

	WORD m_ButtonsPressedThisFrame;
	WORD m_ButtonsReleasedThisFrame;

	unsigned int m_ControllerIndex = 0;
};

dae::Xbox360Controller::Xbox360Controller(unsigned int controllerIndex)
{
	m_pImpl = new Xbox360ControllerImpl(controllerIndex);
}

void dae::Xbox360Controller::Update()
{
	m_pImpl->Update();
}

bool dae::Xbox360Controller::IsDown(ControllerButton button)
{
	return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::Xbox360Controller::IsUp(ControllerButton button)
{
	return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::Xbox360Controller::IsPressed(ControllerButton button)
{
	return m_pImpl->IsPressed(static_cast<unsigned int>(button));
}

Xbox360Controller::Xbox360ControllerImpl::Xbox360ControllerImpl(unsigned int controllerIndex)
	: m_ControllerIndex { controllerIndex }
{
	ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
}

void Xbox360Controller::Xbox360ControllerImpl::Update()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Xbox360Controller::Xbox360ControllerImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool Xbox360Controller::Xbox360ControllerImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool Xbox360Controller::Xbox360ControllerImpl::IsPressed(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}
