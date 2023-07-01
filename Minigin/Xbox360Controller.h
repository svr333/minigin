#pragma once
namespace dae
{
	class Xbox360Controller final
	{
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000,
		};

		Xbox360Controller(unsigned int controllerIndex);
		~Xbox360Controller() = default;

		void Update();

		bool IsDown(ControllerButton button);
		bool IsUp(ControllerButton button);
		bool IsPressed(ControllerButton button);

	private:
		// class declaration has to be inside this class
		class Xbox360ControllerImpl;
		Xbox360ControllerImpl* m_pImpl;
	};
}
