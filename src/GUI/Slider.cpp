#include "Slider.hpp"

#include <algorithm>

gui::Slider::Slider(const sf::Vector2f& size)
	:Textbox(size, 2)
{
	offset = 0;
	bar.setSize({size.y * 0.25f, size.x * 0.25f});
	bar.setOrigin(0, bar.getSize().y * 0.5f);
	setBarFillColor(sf::Color::Red);
	setBarHighlightColor(sf::Color(255, 165, 0));

	actionEvent = ActionEvent::MOVE;
	action = [this]() {
		moveSliderBar(getFrame()->getMousePosition());
	};
}

gui::Slider::~Slider()
{
}

void gui::Slider::copy(const Slider& slider)
{
	bar = slider.bar;
	barColor = slider.barColor;
	barHighlight = slider.barHighlight;

	box = slider.box;
	text = slider.text;
	hasText = slider.hasText;
	alignment = slider.alignment;
}

void gui::Slider::setHigh()
{
	bar.setPosition(box.getPosition().x + box.getSize().x - bar.getSize().x, bar.getPosition().y);
}

void gui::Slider::setLow()
{
	bar.setPosition(box.getPosition().x, bar.getPosition().y);
}

float gui::Slider::getMappedOffset() const 
{
	return offset / (box.getSize().x - bar.getSize().x);
}

void gui::Slider::setBarFillColor(sf::Color color)
{
	barColor = color;
	if(!isSelected)bar.setFillColor(color);
}

void gui::Slider::setBarHighlightColor(sf::Color color)
{
	barHighlight = color;
	if (isSelected)bar.setFillColor(color);
}

void gui::Slider::updateBar()
{
	bar.setPosition(getPosition() + sf::Vector2f(offset, getSize().y * 0.5f));
}

void gui::Slider::setSize(const sf::Vector2f& size)
{
	box.setSize(size);
	bar.setSize({ size.y * 0.25f, size.x * 0.25f });
	bar.setOrigin(0, bar.getSize().y * 0.5f);
	updateBar();
}

void gui::Slider::setPosition(float x, float y)
{
	box.setPosition(x, y);
	updateText();
	updateBar();
}

void gui::Slider::setPosition(const sf::Vector2f& position)
{
	box.setPosition(position.x, position.y);
	updateText();
	updateBar();
}

sf::Vector2f gui::Slider::getPosition() const
{
	return box.getPosition();
}

void gui::Slider::move(float offsetX, float offsetY)
{
	box.move(offsetX, offsetY);
	text.move(offsetX, offsetY);
	bar.move(offsetX, offsetY);
}

void gui::Slider::move(const sf::Vector2f& offset)
{
	box.move(offset);
	text.move(offset);
	bar.move(offset);
}

gui::Entity* gui::Slider::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return box.getGlobalBounds().contains(mousePos) ? this : nullptr;
	}
	return nullptr;
}

void gui::Slider::reset()
{
	offset = 0;
	updateBar();
}

void gui::Slider::moveSliderBar(const sf::Vector2f& mousePos)
{
	sf::Vector2f mPos = mousePos;
	float x = box.getPosition().x;

	mPos.x = std::max(mPos.x, x);
	mPos.x = std::min(mPos.x, x + box.getSize().x - bar.getSize().x);
	offset = mPos.x - x;

	updateBar();
}

void gui::Slider::activateSelection()
{
	isSelected = true;
	bar.setFillColor(barHighlight);
}

void gui::Slider::deactivateSelection()
{
	isSelected = false;
	bar.setFillColor(barColor);
}


void gui::Slider::draw(sf::RenderTarget& target)
{
	if (isActive()) {
		float halfWidth = 1.5f;
		sf::Vertex midLine[4] = {
			{{box.getPosition().x, bar.getPosition().y - halfWidth}, sf::Color::Green},
			{{box.getPosition().x + box.getSize().x * 0.6f, bar.getPosition().y - halfWidth}, sf::Color::Yellow },
			{{box.getPosition().x + box.getSize().x * 0.6f, bar.getPosition().y + halfWidth}, sf::Color::Yellow },
			{{box.getPosition().x, bar.getPosition().y + halfWidth}, sf::Color::Green}
		};
		target.draw(midLine, 4, sf::Quads);

		midLine[0] = { {box.getPosition().x + box.getSize().x * 0.3f, bar.getPosition().y - halfWidth }, sf::Color::Yellow };
		midLine[1] = { {box.getPosition().x + box.getSize().x, bar.getPosition().y - halfWidth}, sf::Color::Red };
		midLine[2] = { {box.getPosition().x + box.getSize().x, bar.getPosition().y + halfWidth}, sf::Color::Red };
		midLine[3] = { {box.getPosition().x + box.getSize().x * 0.3f, bar.getPosition().y + halfWidth}, sf::Color::Yellow };
		target.draw(midLine, 4, sf::Quads);
		target.draw(box);
		target.draw(text);
		target.draw(bar);
	}
}
