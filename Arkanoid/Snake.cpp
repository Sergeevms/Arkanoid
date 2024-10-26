#include "Snake.h"
#include "Apple.h"
#include "Settings.h"
#include "PlayingState.h"

namespace SnakeGame
{

	SnakeNode::SnakeNode(const sf::Vector2i& mapCell, const sf::Texture& texture, const Direction direction, const bool isMovingEnabled) :
		MapObject(mapCell, texture), currentDirection(direction), isMoving(isMovingEnabled)
	{

	}

	void SnakeNode::Update(const float deltaTime, const float speedModifire = 1.f)
	{
		if (isMoving)
		{
			const float movementDistance = deltaTime * Settings::GetSettings()->GetMovementSpeed() * speedModifire;
			sf::Vector2f movement = directionVectorsF.at(currentDirection) * movementDistance;
			screenCoordinates += movement;
			sprite.setPosition(screenCoordinates.x, screenCoordinates.y);
		}
	}

	void SnakeNode::SetDirection(Direction newDirection)
	{
		currentDirection = newDirection;
		SetVisualDirection(newDirection);
	}

	void SnakeNode::SetVisualDirection(Direction direction)
	{
		sprite.setRotation(directionToRotation.at(direction));
	}

	void SnakeNode::SetCellPositionIfMoving(const sf::Vector2i& newCell)
	{
		if (isMoving)
		{
			mapCoordinates.x = newCell.x;
			mapCoordinates.y = newCell.y;
		}
	}

	void SnakeNode::UpdateScreenPositionByCell()
	{
		Settings* settings = Settings::GetSettings(); 
		sf::Vector2f mapCellF(mapCoordinates);
		screenCoordinates = (mapCellF + relativePositions.at(RelativePosition::Center)) * settings->tileSize;
		sprite.setPosition(screenCoordinates.x, screenCoordinates.y);
	}

	void SnakeNode::SetMovingEnabledState(bool isEnabled)
	{
		isMoving = isEnabled;
	}

	Direction SnakeNode::GetDirection() const
	{
		return currentDirection;
	}

	Snake::Snake(PlayingState* currentState, Map* currentMap): playingState(currentState),
		newDirection(Direction::None), map(currentMap)
	{
		Settings* settings = Settings::GetSettings();
		LoadTexture("SnakeHead.png", headTexture);
		LoadTexture("SnakeBody.png", bodyTexture);
	}

	void Snake::Update(const float deltaTime)
	{		
		timeTillNextCell -= deltaTime;
		Settings* settings = Settings::GetSettings();

		sf::Color snakeColor = sf::Color::White;

		//handle special states
		if (timeTillDisorientedFall > 0.f)
		{
			timeTillDisorientedFall -= deltaTime;
			snakeColor *= settings->disorientAppleColor;
		}

		if (timeTillPoisonedFall > 0.f)
		{
			timeTillPoisonedFall -= deltaTime;
			snakeColor *= settings->poisionedAppleColor;
		}

		for (auto& node : nodes)
		{
			node.get()->SetColor(snakeColor);
		}

		if (timeTillNextCell > 0.f)
		{
			for (auto& node : nodes)
			{
				node->Update(deltaTime, timeTillPoisonedFall > 0.f ? settings->poisonedSpeedModifire : 1.f);
			}
		}
		else
		{
			timeTillNextCell = settings->GetTimeOnCell() / (timeTillPoisonedFall > 0.f ? settings->poisonedSpeedModifire : 1.f);
			nodes.back()->SetMovingEnabledState(true);
			std::shared_ptr<SnakeNode> head = nodes.front();
			Direction nextNodeDirection = newDirection;
			sf::Vector2i cellToCheck = head->GetCellPosition() + directionVectorsI.at(nextNodeDirection);
			if (playingState->CheckColition(cellToCheck))
			{				
				for (auto node = nodes.begin(); node != nodes.end(); ++node)
				{
					map->RemoveMapObject((*node));
					sf::Vector2i currentCell = (*node)->GetCellPosition();
					Direction currentDirection = (*node)->GetDirection();
					(*node)->UpdateScreenPositionByCell();
					(*node)->SetCellPositionIfMoving(currentCell + directionVectorsI.at(nextNodeDirection));
					(*node)->SetDirection(nextNodeDirection);
					nextNodeDirection = currentDirection;
					map->EmplaceMapObject((*node));
					(*node)->Update(deltaTime, timeTillPoisonedFall > 0.f ? settings->poisonedSpeedModifire : 1.f);
				}
			}
			else
			{
				head->SetDirection(newDirection);
			}
		}
	}

	void Snake::SetNewDirection(Direction direction)
	{
		Direction possibleDirection = timeTillDisorientedFall > 0.f ? OppossiteDirection(direction) : direction;
		if (possibleDirection != OppossiteDirection(nodes.front()->GetDirection()))
		{
			newDirection = possibleDirection;
		}
	}

	void Snake::LoadFromCharMap(const std::vector<std::string>& charMap, const sf::Vector2i& headPosition)
	{
		nodes.clear();
		sf::Vector2i currentCell = headPosition;
		std::vector<std::vector<bool>> addedToSnake;
		addedToSnake.reserve(charMap.size());
		for (auto& row : charMap)
		{
			addedToSnake.push_back(std::vector<bool>(row.size(), false));
		}
		addedToSnake[currentCell.y][currentCell.x] = true;
		nodes.push_back(std::make_shared<SnakeNode>(currentCell, headTexture, Direction::None));

		//Adding body from map
		while (AddNextBodyFromMap(addedToSnake, charMap, currentCell))
		{
			currentCell = nodes.back()->GetCellPosition();
		}

		//Select new direction
		Direction possibleDirection = ((nodes.begin())->get())->GetDirection();
		if (charMap[headPosition.y + (directionVectorsI.at(possibleDirection).y)][headPosition.x + (directionVectorsI.at(possibleDirection).x)] == 'E' ||
			charMap[headPosition.y + (directionVectorsI.at(possibleDirection).y)][headPosition.x + (directionVectorsI.at(possibleDirection).x)] == 'A')
		{
			newDirection = possibleDirection;
		}
		else
		{
			for (auto& curDir : directionVectorsI)
			{
				if (charMap[headPosition.y + (curDir.second.y)][headPosition.x + (curDir.second.x)] == 'E' ||
					charMap[headPosition.y + (curDir.second.y)][headPosition.x + (curDir.second.x)] == 'A')
				{
					newDirection = curDir.first;
				}
			}
		}
		nodes.front()->SetVisualDirection(newDirection);

		for (auto& node : nodes)
		{
			map->EmplaceMapObject((node));
		}
	}

	void Snake::GenerateSnake(Map* currentMap)
	{
		sf::Vector2i currentNode = currentMap->GetRandomEmptyCell();
		nodes.push_back(std::make_shared<SnakeNode>(currentNode, headTexture, Direction::None));
		map->EmplaceMapObject(nodes.back());

		Direction previosDirection = Direction::None;

		//Choosing head direction
		for (const auto& direction : directionVectorsI)
		{
			if (map->GetObject(currentNode + direction.second) == nullptr || dynamic_cast<Apple*>(map->GetObject(currentNode + direction.second)))
			{
				newDirection = direction.first;
				previosDirection = direction.first;
				break;
			}
		}		

		//Adding snake body nodes while can or default snake size reached
		for (int i = 1; i < Settings::GetSettings()->defaultSnakeSize; ++i)
		{
			for (const auto& direction : directionVectorsI)
			{
				if (direction.first != previosDirection)
				{
					sf::Vector2i newNodeCell = currentNode + direction.second;
					if (map->GetObject(newNodeCell) == nullptr)
					{
						nodes.back().get()->SetDirection(OppossiteDirection(direction.first));
						previosDirection = OppossiteDirection(direction.first);
						nodes.push_back(std::make_shared<SnakeNode>(newNodeCell, bodyTexture, Direction::None));
						map->EmplaceMapObject(nodes.back());
						currentNode = newNodeCell;
						break;
					}
				}
			}
		}
		nodes.front()->SetVisualDirection(newDirection);
	}

	void Snake::AddNewBody()
	{
		std::shared_ptr<SnakeNode> lastNode = nodes.back();
		nodes.push_back(std::make_shared<SnakeNode>(lastNode->GetCellPosition(), bodyTexture, lastNode->GetDirection(), false));
	}

	void Snake::GetDisoriented()
	{
		timeTillDisorientedFall = Settings::GetSettings()->disorientationTime;
	}

	void Snake::GetPoisioned()
	{
		Settings* settings = Settings::GetSettings();
		timeTillPoisonedFall = settings->poisonedCellCount * (settings->GetTimeOnCell() / settings->poisonedSpeedModifire);
		timeTillNextCell = settings->GetTimeOnCell() / settings->poisonedSpeedModifire;
	}

	bool Snake::IsPoisioned() const
	{
		return timeTillPoisonedFall > 0.f;
	}

	bool Snake::AddNextBodyFromMap(std::vector<std::vector<bool>>& addedCells, const std::vector<std::string>& charMap, const sf::Vector2i& currentCell)
	{
		for (auto& curDir : directionVectorsI)
		{
			sf::Vector2i checkingCell = currentCell + curDir.second;

			if (map->ValidCell(checkingCell) &&
				charMap[checkingCell.y][checkingCell.x] == 'B' &&
				addedCells[checkingCell.y][checkingCell.x] == false)
			{				
				nodes.back()->SetDirection(OppossiteDirection(curDir.first));
				nodes.push_back(std::make_shared<SnakeNode>(checkingCell, bodyTexture, Direction::None));
				addedCells[checkingCell.y][checkingCell.x] = true;
				return true;
			}
		}
		return false;
	}
}