#pragma once
namespace Arkanoid
{
	class IDelayedAction
	{
	public:
		void StartTimer(float delayDuration);
	protected:
		virtual void UpdateTimer(float deltaTime);
		virtual void FinalAction() = 0;
		virtual void UpdateAction(float deltaTime) = 0;
		float delayDuration = 0.f;
		float currentTime = 0.f;
	};
}

