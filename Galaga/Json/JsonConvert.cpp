#include "JsonConvert.h"
#include <fstream>
#include <filesystem>
#include <rapidjson.h>
#include <istreamwrapper.h>
#include <document.h>
#include <stringbuffer.h>
#include <writer.h>
#include <iostream>

using namespace rapidjson;

std::vector<dae::Enemy> dae::JsonConvert::DeserializeEnemies(const std::string& filePath)
{
	auto scores = std::vector<Enemy>{};

	// path can be non existant if running for first time
	if (!std::filesystem::exists(m_DataPath))
		return scores;

	if (std::ifstream s{ m_DataPath + filePath })
	{
		IStreamWrapper isw{ s };
		Document document;
		document.ParseStream(isw);

		if (!document.IsArray())
		{
			return scores;
		}

		for (SizeType i = 0; i < document.Size(); i++)
		{
			// skip if not an object / wrong object
			if (!document[i].IsObject() || !document[i].HasMember("X")
				|| !document[i].HasMember("Y") || !document[i].HasMember("Type"))
			{
				continue;
			}

			Enemy e{};
			e.X = document[i]["X"].GetFloat();
			e.Y = document[i]["Y"].GetFloat();
			e.Type = (EnemyType)document[i]["Type"].GetInt();

			scores.push_back(e);
		}
	}

	return scores;
}

void dae::JsonConvert::SerializeHighscores(const std::vector<Highscore>& highscores)
{
	if (std::ofstream stream{ m_HighscorePath })
	{
		stream.clear();

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);

		writer.StartArray();

		for (size_t i = 0; i < highscores.size(); i++)
		{
			writer.StartObject();

			writer.Key("Name");
			writer.String(highscores[i].Name.c_str());

			writer.Key("Score");
			writer.Int(highscores[i].Score);
			
			writer.EndObject();
		}

		writer.EndArray();

		if (!writer.IsComplete())
		{
			std::cout << "Something went wrong while writing high score\n";
		}

		stream << buffer.GetString();
	}
}

std::vector<dae::Highscore> dae::JsonConvert::DeserializeHighscores()
{
	auto scores = std::vector<Highscore>{};

	// path can be non existant if running for first time
	if (!std::filesystem::exists(m_HighscorePath))
		return scores;

	if (std::ifstream s{ m_HighscorePath })
	{
		IStreamWrapper isw{ s };
		Document document;
		document.ParseStream(isw);

		if (!document.IsArray())
		{
			return scores;
		}

		for (SizeType i = 0; i < document.Size(); i++)
		{
			// skip if not an object / wrong object
			if (!document[i].IsObject() || !document[i].HasMember("Name") || !document[i].HasMember("Score"))
			{
				continue;
			}

			Highscore hs{};
			hs.Name = document[i]["Name"].GetString();
			hs.Score = document[i]["Score"].GetInt();

			scores.push_back(hs);
		}
	}

	return scores;
}
