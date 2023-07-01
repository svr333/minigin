#pragma once
namespace dae
{
	class Command abstract
	{
	public:
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute(float deltaTime) = 0;

	protected:
		explicit Command() = default;
	};
}
