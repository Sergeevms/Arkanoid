#pragma once
#include "IGameObject.h"
namespace Arkanoid
{
	enum class BlockType;

	class IBlockObject : public virtual IGameObject
	{
	public:
		virtual bool IsBroken() const = 0;
		virtual bool IsBreakable() const = 0;
		virtual bool IsBallReboundable() const = 0;
		virtual int GetScore() const = 0;
		virtual BlockType GetBlockType() const = 0;
	};
}

