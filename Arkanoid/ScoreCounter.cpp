#include "ScoreCounter.h"
#include "IBlockObject.h"
#include "Utility.h"
#include "IListDrawable.h"

namespace Arkanoid
{
	void ScoreCounter::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto block = std::dynamic_pointer_cast<IBlockObject>(observable))
		{
			currentScore += block->GetScore();
			Emit();
		}
	}

	int Arkanoid::ScoreCounter::GetCurrentScore() const
	{
		return currentScore;
	}

	void ScoreCounter::Reset()
	{
		SetScore(0);
	}

	void ScoreCounter::SetScore(const int score)
	{
		currentScore = score;
		Emit();
	}

	ScoreImage::ScoreImage()
	{
		scoreTextStyle = std::make_unique<TextStyle>();
		scoreTextStyle->Init("Roboto-Regular.ttf", sf::Color::Green, sf::Text::Bold, 30);
		scoreText = std::make_unique<ListDrawableText>();
		scoreText->SetStyle(scoreTextStyle.get());
	}

	void ScoreImage::Draw(sf::RenderWindow& window) const
	{
		scoreText->Draw(window);
	}

	void ScoreImage::SetScoreCounter(std::shared_ptr<ScoreCounter> counter)
	{
		scoreCounter = counter;
		counter->AddObserver(weak_from_this());
		UpdateText(counter->GetCurrentScore());
	}

	void ScoreImage::SetPosition(const sf::Vector2f coordinates, const RelativePosition relativePosition)
	{
		position = coordinates;
		coordinatesRelativePosition = relativePosition;
		if (auto counter = scoreCounter.lock())
		{
			UpdateText(counter->GetCurrentScore());
		}
	}

	void ScoreImage::Notify(std::shared_ptr<IObservable> observable)
	{
		if (auto counter = std::dynamic_pointer_cast<ScoreCounter> (observable); counter == scoreCounter.lock())
		{
			UpdateText(counter->GetCurrentScore());
		}
	}

	void ScoreImage::UpdateText(int score)
	{
		scoreText->setString(L"Очки:\n" + std::to_wstring(score));
		scoreText->PresetPosition(position, coordinatesRelativePosition);
	}
}
