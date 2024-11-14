#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <unordered_map>

namespace Arkanoid
{
	enum class SoundType
	{
		OnKeyHit,
		OnLose,
		OnBallHit,
		OnSessionStart
	};

	enum class GameState;
	class BaseState;

	class Game
	{
	public:
		Game();

		bool IsGameShuttingDown() const;
		void Update(const float deltaTime, const std::vector<sf::Event>& inputEvents);
		void Draw(sf::RenderWindow&) const;
		void SwitchToState(GameState newState);
		void ShutDown();
		void PlaySound(const SoundType sound);
		void SetLastSessionScore(const int score);
		int GetLastSessionScore() const;
		BaseState* GetState() const;

	private:
		bool isShuttingDown{ false };
		int lastSessionScore{ 0 };
		sf::Music backGroundMusic;
		std::vector<std::unique_ptr<BaseState>> stateStack;
		std::vector<std::unique_ptr<sf::SoundBuffer>> soundBuffers;
		std::unordered_map<SoundType, sf::Sound> sounds;
		void LoadSound(const SoundType type, std::string fileName);
	};
}