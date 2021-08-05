#pragma once
#include "Button.hpp"

gui::Button::Button(const sf::Vector2f& size)
	:Textbox(size, 1)
{
	action = nullptr;
	actionEvent = ActionEvent::RELEASE;
}

gui::Button::~Button()
{
}

void gui::Button::copy(const Button& button)
{
	box = button.box;
	text = button.text;
	hasText = button.hasText;
	alignment = button.alignment;

	action = nullptr;
	actionEvent = ActionEvent::RELEASE;
	original = button.original;
	highlight = button.highlight;
}

void gui::Button::setHighlightFillColor(const sf::Color& color)
{
	highlight.fillColor = color;
	if (isSelected)box.setFillColor(color);
}

void gui::Button::setHighlightOutlineColor(const sf::Color& color)
{
	highlight.outlineColor = color;
	if (isSelected)box.setOutlineColor(color);
}

void gui::Button::setHighlightOutlineThickness(float thickness)
{
	highlight.outlineThickness = thickness;
	if (isSelected)box.setOutlineThickness(thickness);
}

void gui::Button::setFillColor(const sf::Color& color)
{
	original.fillColor = color;
	if (!isSelected)box.setFillColor(color);
}

void gui::Button::setOutlineColor(const sf::Color& color)
{
	original.outlineColor = color;
	if (!isSelected)box.setOutlineColor(color);
}

void gui::Button::setOutlineThickness(float thickness)
{
	original.outlineThickness = thickness;
	if (!isSelected)box.setOutlineThickness(thickness);
}

bool gui::Button::contains(const sf::Vector2f& mousePos) const
{
	return box.getGlobalBounds().contains(mousePos);
}

gui::Entity* gui::Button::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		return this->contains(mousePos) ? this : nullptr;
	}
	return nullptr;
}

void gui::Button::activateSelection()
{
	isSelected = true;
	box.setFillColor(highlight.fillColor);
	box.setOutlineColor(highlight.outlineColor);
	box.setOutlineThickness(highlight.outlineThickness);
}
void gui::Button::deactivateSelection()
{
	isSelected = false;
	box.setFillColor(original.fillColor);
	box.setOutlineColor(original.outlineColor);
	box.setOutlineThickness(original.outlineThickness);
}

