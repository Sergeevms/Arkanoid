#include "IDelayedAction.h"

namespace Arkanoid
{
	void IDelayedAction::StartTimer(float delayDuration)
	{
		IDelayedAction::delayDuration = delayDuration;
		currentTime = delayDuration;
	}

	void IDelayedAction::UpdateTimer(float deltaTime)
	{
		if (currentTime > 0.f)
		{
			currentTime -= deltaTime;
			UpdateAction(deltaTime);
			if (currentTime <= 0.f)
			{
				FinalAction();
			}
		}
	}
}