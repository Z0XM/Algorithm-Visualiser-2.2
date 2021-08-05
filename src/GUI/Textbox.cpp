#include "Textbox.hpp"

namespace gui {
	Textbox::Textbox(const sf::Vector2f size, int class_id)
		: Entity(class_id), hasText(true), alignment(TextAlign::CENTER)
	{
		this->setSize(size);
		action = nullptr;
		actionEvent = ActionEvent::NONE;
	}
	Textbox::Textbox(const sf::Vector2f& size)
		:Entity(5), hasText(true), alignment(TextAlign::CENTER)
	{
		this->setSize(size);
		action = nullptr;
		actionEvent = ActionEvent::NONE;
	}

	Textbox::~Textbox()
	{

	}

	void Textbox::copy(Textbox& textbox)
	{
		box = textbox.box;
		text = textbox.text;
		hasText = textbox.hasText;
		alignment = textbox.alignment;
	}

	void Textbox::updateText()
	{
		if (hasText) {
			text.setPosition(
				box.getPosition().x + (box.getSize().x - text.getGlobalBounds().width) * 0.5f * (int)alignment,
				box.getPosition().y + box.getSize().y * 0.5f - text.getGlobalBounds().height
			);
		}
	}

	void Textbox::setSize(const sf::Vector2f& size)
	{
		box.setSize(size);
	}

	const sf::Vector2f& Textbox::getSize() const
	{
		return box.getSize();
	}

	void Textbox::setPosition(float x, float y)
	{
		box.setPosition(x, y);
		updateText();
	}

	void Textbox::setPosition(const sf::Vector2f& position)
	{
		box.setPosition(position.x, position.y);
		updateText();
	}

	sf::Vector2f Textbox::getPosition() const
	{
		return box.getPosition();
	}

	void Textbox::move(float offsetX, float offsetY)
	{
		box.move(offsetX, offsetY);
		text.move(offsetX, offsetY);
	}

	void Textbox::move(const sf::Vector2f& offset)
	{
		box.move(offset);
		text.move(offset);
	}

	void Textbox::setFillColor(const sf::Color& color)
	{
		box.setFillColor(color);
	}

	void Textbox::setOutlineColor(const sf::Color& color)
	{
		box.setOutlineColor(color);
	}

	void Textbox::setOutlineThickness(float thickness)
	{
		box.setOutlineThickness(thickness);
	}

	const sf::Color& Textbox::getFillColor() const
	{
		return box.getFillColor();
	}

	const sf::Color& Textbox::getOutlineColor() const
	{
		return box.getOutlineColor();
	}

	float Textbox::getOutlineThickness() const
	{
		return box.getOutlineThickness();
	}

	void Textbox::alignText(const TextAlign& align)
	{
		alignment = align;
		updateText();
	}

	sf::FloatRect Textbox::getGlobalBounds() const
	{
		return box.getGlobalBounds();
	}

	void Textbox::setString(const std::string& string)
	{
		text.setString(string);
		updateText();
	}

	std::string Textbox::getString() const
	{
		return text.getString();
	}

	void Textbox::setFont(const sf::Font& font)
	{
		text.setFont(font);
	}

	void Textbox::setCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	void Textbox::setLineSpacing(float spacingFactor)
	{
		text.setLineSpacing(spacingFactor);
	}

	void Textbox::setLetterSpacing(float spacingFactor)
	{
		text.setLetterSpacing(spacingFactor);
	}

	const sf::Font* Textbox::getFont() const
	{
		return text.getFont();
	}

	unsigned int Textbox::getCharacterSize() const
	{
		return text.getCharacterSize();
	}

	float Textbox::getLetterSpacing() const
	{
		return text.getLetterSpacing();
	}

	float Textbox::getLineSpacing() const
	{
		return text.getLineSpacing();
	}

	void Textbox::setTextFillColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void Textbox::setTextOutlineColor(const sf::Color& color)
	{
		text.setOutlineColor(color);
	}

	void Textbox::setTextOutlineThickness(float thickness)
	{
		text.setOutlineThickness(thickness);
	}

	const sf::Color& Textbox::getTextFillColor() const
	{
		return text.getFillColor();
	}

	const sf::Color& Textbox::getTextOutlineColor() const
	{
		return text.getOutlineColor();
	}

	float Textbox::getTextOutlineThickness() const
	{
		return text.getOutlineThickness();
	}

	void Textbox::setStyle(sf::Uint32 style)
	{
		text.setStyle(style);
	}

	void Textbox::setText(bool hasText)
	{
		this->hasText = hasText;
	}
	Entity* Textbox::isHit(const sf::Vector2f& mousePos)
	{
		return nullptr;
	}
	void Textbox::draw(sf::RenderTarget& target)
	{
		if (isActive()) {
			target.draw(box);
			target.draw(text);
		}
	}
	void Textbox::activateSelection()
	{
	}
	void Textbox::deactivateSelection()
	{
	}
}
