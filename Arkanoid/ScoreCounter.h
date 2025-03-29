#pragma once
#include "IObserver.h"
#include <SFML/Graphics.hpp>
#include "Utility.h"

namespace Arkanoid
{
	struct TextStyle;
	class ListDrawableText;
	
	class ScoreCounter :
		public IObserver, public IObservable
	{
	public:
		int GetCurrentScore() const;
		void Reset();
		void SetScore(const int score);
		virtual void Notify(std::shared_ptr<IObservable> observable) override;
	private:
		int currentScore = 0;
	};

	class ScoreImage :
		public IObserver, public std::enable_shared_from_this<ScoreImage>
	{
	public:
		ScoreImage();
		void Draw(sf::RenderWindow& window) const;
		void SetScoreCounter(std::shared_ptr<ScoreCounter> counter);
		void SetPosition(const sf::Vector2f coordinates, const RelativePosition relativePosition);
		virtual void Notify(std::shared_ptr<IObservable> observable) override;
	private:
		void UpdateText(int score);
		std::unique_ptr<TextStyle> scoreTextStyle;
		std::unique_ptr<ListDrawableText> scoreText;
		RelativePosition coordinatesRelativePosition = RelativePosition::Center;
		sf::Vector2f position;
		std::weak_ptr<ScoreCounter> scoreCounter;
	};
}
