#pragma once
#include <Services/Singleton.h>
#include <string>
#include <vector>
#include "../Entities/Enemy.h"
#include "../Entities/Highscore.h"

namespace dae
{
	class JsonConvert final : public Singleton<JsonConvert>
	{
	public:
		std::vector<Enemy> DeserializeEnemies(const std::string& filePath);

		void SerializeHighscores(const std::vector<Highscore>& highscores);
		std::vector<Highscore> DeserializeHighscores();

	private:
		std::string m_DataPath = "../Data/Json/";
		std::string m_HighscorePath = m_DataPath + "Highscores.json";
	};
}
