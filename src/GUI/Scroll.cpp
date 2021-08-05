#include "Scroll.hpp"
#include <algorithm>

#include "Page.hpp"

gui::Scroll::Scroll()
	:Entity(3)
{
	rect.setFillColor(sf::Color(41, 41, 41));
	rect.setOutlineColor(sf::Color::Transparent);
	rect.setHighlightFillColor(sf::Color(60, 60, 60));
	rect.setHighlightOutlineColor(sf::Color::Transparent);
	
	bar.setFillColor(sf::Color(150, 150, 150, 70));
	bar.setOutlineColor(sf::Color::Transparent);
	bar.setHighlightFillColor(sf::Color(200, 200, 200, 70));
	bar.setHighlightOutlineColor(sf::Color::Transparent);

	rect.setText(false);
	bar.setText(false);

	rect.actionEvent = ActionEvent::PRESS;
	bar.actionEvent = ActionEvent::MOVE;

	scrollPos = RIGHT;
}

gui::Scroll::~Scroll()
{
}

const sf::Vector2f& gui::Scroll::getSize() const
{
	return rect.getSize();
}

sf::Vector2f gui::Scroll::getPosition() const
{
	return rect.getPosition();
}

void gui::Scroll::setPage(Page& page, ScrollPosition scrollPos)
{
	this->scrollPos = scrollPos;
	auto bound = page.getActiveRegion();
	switch (scrollPos)
	{
	case TOP: case LEFT: rect.setPosition(0, 0); break;
	case RIGHT: rect.setPosition(bound.width - 10, 0); break;
	case BOTTOM: rect.setPosition(0, bound.height - 10); break;
	}
	if (scrollPos == LEFT || scrollPos == RIGHT) {
		rect.setSize({ 10, bound.height });
		bar.setSize(sf::Vector2f(rect.getSize().x, rect.getSize().y * rect.getSize().y / page.getSize().y));
		bar.setPosition(rect.getPosition() + sf::Vector2f(0, bound.top * rect.getSize().y / page.getSize().y));
	}
	else if (scrollPos == BOTTOM || scrollPos == TOP) {
		rect.setSize({ bound.width, 10 });
		bar.setSize(sf::Vector2f(rect.getSize().x * rect.getSize().x / page.getSize().x, rect.getSize().y));
		bar.setPosition(rect.getPosition() + sf::Vector2f(bound.left * rect.getSize().x / page.getSize().x, 0));
	}
}

void gui::Scroll::move(float offsetX, float offsetY)
{
	bar.move(offsetX, offsetY);
	rect.move(offsetX, offsetY);
}

void gui::Scroll::move(const sf::Vector2f& offset)
{
	bar.move(offset);
	rect.move(offset);
}

sf::Vector2f gui::Scroll::scrollTo(const sf::Vector2f& mousePos)
{
	if (scrollPos == TOP || scrollPos == BOTTOM) {
		float oldX = bar.getPosition().x;
		float newX = mousePos.x - bar.getSize().x * 0.5f;
		newX = std::max(rect.getPosition().x, newX);
		newX = std::min(rect.getPosition().x + rect.getSize().x - bar.getSize().x, newX);

		scrollBy({ newX - oldX, 0 });
		return { newX - oldX, 0 };
	}
	else {
		float oldY = bar.getPosition().y;
		float newY = mousePos.y - bar.getSize().y * 0.5f;
		newY = std::max(rect.getPosition().y, newY);
		newY = std::min(rect.getPosition().y + rect.getSize().y - bar.getSize().y, newY);

		scrollBy({ 0, newY - oldY });
		return { 0, newY - oldY };
	}
}

void gui::Scroll::scrollBy(const sf::Vector2f& offset)
{
	bar.move(offset);
}

gui::Entity* gui::Scroll::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return bar.getGlobalBounds().contains(mousePos) ? &bar : (rect.getGlobalBounds().contains(mousePos) ? &rect : nullptr);
	}
	return nullptr;
}

void gui::Scroll::draw(sf::RenderTarget& target)
{
	if (isActive()) {
		rect.draw(target);
		bar.draw(target);
	}
}

void gui::Scroll::activateSelection()
{
}

void gui::Scroll::deactivateSelection()
{
}
