#pragma once

#include "Textbox.hpp"
#include "GUIFrame.hpp"
#include <SFML/Graphics.hpp>

namespace gui {
	class Slider : public Textbox {
	private:
		sf::RectangleShape bar;
		sf::Color barColor, barHighlight;
		float offset;

		void updateBar();
	public:
		Slider(const sf::Vector2f& size = sf::Vector2f(0, 0));
		~Slider();

		void copy(const Slider& slider);

		void setHigh();
		void setLow();
		float getMappedOffset()const;

		void setBarFillColor(sf::Color color);
		void setBarHighlightColor(sf::Color color);

		void setSize(const sf::Vector2f& size);
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		sf::Vector2f getPosition() const;
		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		void reset();
		void moveSliderBar(const sf::Vector2f& mousePos);
		Entity* isHit(const sf::Vector2f& mousePos);

		void activateSelection() override;
		void deactivateSelection() override;
		
		void draw(sf::RenderTarget& target) override;
	};
}