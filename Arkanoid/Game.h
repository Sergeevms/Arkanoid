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
		void StartGame();
		void PauseGame();
		void WinGame();
		void LooseGame();
		void UpdateGame(const float deltaTime, sf::RenderWindow& window);
		void ToMainMenu();
		void ShowRecords();
		void LoadNextLevel();
		void Shutdown();

		void PlaySoundOnKeyHit();
		void PlaySoundOnBallHit();
		void PlaySoundOnSessionStart();

		Game();
		void SetLastSessionScore(const int score);
		int GetLastSessionScore() const;
		BaseState* GetState() const;

	private:
		void Update(const float deltaTime);
		void HandleInputEvents(sf::RenderWindow& window);
		void Draw(sf::RenderWindow&) const;
		void SwitchToState(GameState newState);
		bool IsGameShuttingDown() const;
		void PlaySound(const SoundType sound);
		int lastSessionScore{ 0 };
		sf::Music backGroundMusic;
		std::vector<std::shared_ptr<BaseState>> stateStack;
		std::vector<std::unique_ptr<sf::SoundBuffer>> soundBuffers;
		std::unordered_map<SoundType, sf::Sound> sounds;
		void LoadSound(const SoundType type, std::string fileName);
	};
}