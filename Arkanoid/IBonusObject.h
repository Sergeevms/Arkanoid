#pragma once
#include "IGameObject.h"
namespace Arkanoid
{
	enum class BonusType;

	class IBonusObject :
		public virtual IGameObject
	{
	public:
		virtual void Apply(std::shared_ptr<IGameObject>& object) = 0;
		virtual BonusType GetType() = 0;
		virtual bool IsActivated() = 0;
		virtual bool IsToBeDestroyed() = 0;
	protected:
		virtual void OnActivation(std::shared_ptr<IGameObject>& object) = 0;
		virtual void OnEnding(std::shared_ptr<IGameObject>& object) const = 0;
	};
}

