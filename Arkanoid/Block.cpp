#include "Block.h"
#include "Application.h"
#include "Utility.h"

namespace Arkanoid
{
	Block::Block(const sf::Vector2f& position) : GameObject("platform.png")
	{
		Settings* settings = Application::GetSettings();
		sprite.setColor(settings->blockColors[BlockType::Simple]);
		SetScaleBySize(sprite, settings->blockSize);
		SetOriginByRelative(sprite, relativePositions.at(RelativePosition::Center));
		sprite.setPosition(position);
	}

	bool Block::GetCollision(Collidable* object) const
	{
		auto gameObject = dynamic_cast<GameObject*>(object);
		assert(gameObject);
		sf::FloatRect objectRect = gameObject->GetRect();
		return GetRect().intersects(gameObject->GetRect());
	}

	bool Block::IsBroken()
	{
		return HitCount <= 0;
	}

	bool Block::IsBallReboundable()
	{
		return true;
	}

	void Block::OnHit()
	{
		--HitCount;
	}

	UnbreakbleBlock::UnbreakbleBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(Application::GetSettings()->blockColors[BlockType::Unbreackble]);
	}

	void UnbreakbleBlock::OnHit()
	{
		
	}

	std::shared_ptr<Block> CreateRandomBlock(sf::Vector2f position)
	{
		Settings* settings = Application::GetSettings();
		BlockType newBlockType = settings->availiableBlockTypes[rand() % settings->availiableBlockTypes.size()];
		switch (newBlockType)
		{
		case Arkanoid::BlockType::Simple:
		{
			return std::make_shared<Block>(position);
			break;
		}
		case Arkanoid::BlockType::SmoothDestroyable:
		{
			return std::make_shared<SmoothDestroyableBlock>(position);
			break;
		}
		case Arkanoid::BlockType::Unbreackble:
		{
			return std::make_shared<UnbreakbleBlock>(position);
			break;
		}
		case Arkanoid::BlockType::MultiHit:
		{
			return std::make_shared<MultiHitBlock>(position);
			break;
		}
		case Arkanoid::BlockType::Glass:
		{
			return std::make_shared<GlassBlock>(position);
			break;
		}
		default:
		{
			return nullptr;
			break;
		}
		}
	}

	SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(Application::GetSettings()->blockColors[BlockType::SmoothDestroyable]);
	}

	bool SmoothDestroyableBlock::GetCollision(Collidable* object) const
	{
		if (currentTime > 0.f)
		{
			return false;
		}
		return Block::GetCollision(object);
	}

	void SmoothDestroyableBlock::Update(const float deltaTime)
	{
		UpdateTimer(deltaTime);
	}

	void SmoothDestroyableBlock::OnHit()
	{
		StartTimer(Application::GetSettings()->smoothBlockDestroyTime);
	}

	void SmoothDestroyableBlock::FinalAction()
	{
		--HitCount;
	}

	void SmoothDestroyableBlock::UpdateAction(float deltaTime)
	{
		ChangeSpriteOpacity(sprite, static_cast<sf::Uint8> (255 * currentTime / delayDuration));
	}

	MultiHitBlock::MultiHitBlock(const sf::Vector2f& position) : Block(position)
	{
		HitCount = Application::GetSettings()->multiHitBlockCount;
		sprite.setColor(Application::GetSettings()->blockColors[BlockType::MultiHit]);
	}

	void MultiHitBlock::OnHit()
	{
		--HitCount;
		ChangeSpriteOpacity(sprite, 255 * HitCount / Application::GetSettings()->multiHitBlockCount);
	}

	GlassBlock::GlassBlock(const sf::Vector2f& position) : Block(position)
	{
		sprite.setColor(Application::GetSettings()->blockColors[BlockType::Glass]);
	}

	bool GlassBlock::CheckCollision(Collidable* object)
	{
		if (GetCollision(object))
		{
			OnHit();
			return true;
		}
		return false;
	}

	bool GlassBlock::IsBallReboundable()
	{
		return false;
	}
}