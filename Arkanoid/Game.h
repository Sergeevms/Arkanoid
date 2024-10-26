#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <unordered_map>

namespace SnakeGame
{
	enum class SoundType
	{
		OnKeyHit,
		OnLose,
		OnSnakeHit,
		OnSessionStart
	};

	class BaseState;
	enum class GameState;

	class Game
	{
	public:
		static Game* GetGame()
		{
			if (game == nullptr)
			{
				game = new Game();
			}
			return game;
		};

		Game(Game&) = delete;

		bool IsGameShuttingDown() const;
		void Update(const float deltaTime, const std::vector<sf::Event>& inputEvents);
		void Draw(sf::RenderWindow&) const;
		void SwitchToState(GameState newState);
		void ShutDown();
		void SwitchMusicPlaying(bool playing);
		void PlaySound(const SoundType sound);
		void SetLastSessionScore(const int score);
		int GetLastSessionScore();

	private:
		Game();
		static Game* game;
		bool isShuttingDown{ false };
		int lastSessionScore{ 0 };
		sf::Music backGroundMusic;
		std::vector<std::unique_ptr<BaseState>> stateStack;
		std::vector<std::unique_ptr<sf::SoundBuffer>> soundBuffers;
		std::unordered_map<SoundType, sf::Sound> sounds;
		void loadSound(const SoundType type, std::string fileName);
	};
}