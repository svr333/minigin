#pragma once

namespace dae
{
	class GameObject;

	enum EnemyType
	{
		Bee = 0,
		Butterfly = 1,
		Boss = 2,
		Fighter = 3,
	};

	struct Enemy final
	{
	public:
		float X;
		float Y;
		EnemyType Type = EnemyType::Bee;
	};
}
