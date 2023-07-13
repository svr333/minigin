#pragma once

namespace dae
{
	enum EnemyType
	{
		Bee = 0,
		Butterfly = 1,
		Boss = 2,
	};

	struct Enemy final
	{
	public:
		float X;
		float Y;
		EnemyType Type = EnemyType::Bee;
	};
}
