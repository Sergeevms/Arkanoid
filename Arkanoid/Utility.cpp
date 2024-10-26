#include <assert.h>
#include "Utility.h"
#include "Settings.h"

namespace SnakeGame
{
    void TextStyle::Init(const std::string fontName, const sf::Color newColor, const sf::Text::Style newTextStyle, const unsigned int newSize)
    {
        Settings* settings = Settings::GetSettings();
#ifdef _DEBUG
        assert(font.loadFromFile(settings->fontPath + fontName));
#else
        font.loadFromFile(settings->fontPath + fontName);
#endif // DEBUG
        color = newColor;
        textStyle = newTextStyle;
        characterSize = newSize;
    }

    Direction SnakeGame::OppossiteDirection(Direction direction)
    {
        switch (direction)
        {
        case Direction::Left:
        {
            return Direction::Rigth;
            break;
        }
        case Direction::Rigth:
        {
            return Direction::Left;
            break;
        }
        case Direction::Up:
        {
            return Direction::Down;
            break;
        }
        case Direction::Down:
        {
            return Direction::Up;
            break;
        }
        default:
            return Direction::None;
        }
    }

    void LoadTexture(std::string const& fileName, sf::Texture& texture)
    {
#ifndef NDEBUG
        assert(texture.loadFromFile(Settings::GetSettings()->texturePath + fileName));
#else 
        texture.loadFromFile(Settings::GetSettings()->texturePath + fileName);
#endif
    }

    RelativePosition RelativePositionByOrientationAndAlignment(const Orientation orientation, const Alignment alignment)
    {
        RelativePosition relativeOrigin = RelativePosition::TopLeft;
        switch (alignment)
        {
        case Alignment::Min:
        {
            relativeOrigin = RelativePosition::TopLeft;
            break;
        }
        case Alignment::Middle:
        {
            relativeOrigin = orientation == Orientation::Vertical ? RelativePosition::TopMiddle : RelativePosition::MiddleLeft;
            break;
        }
        case Alignment::Max:
        {
            relativeOrigin = orientation == Orientation::Vertical ? RelativePosition::TopRight : RelativePosition::BottomLeft;
            break;
        }
        }
        return relativeOrigin;
    }
}