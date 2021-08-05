#pragma once

#include "GUIFrame.hpp"
#include "Textbox.hpp"

namespace gui {
	class Button : public Textbox {
	public:
		Button(const sf::Vector2f& size = sf::Vector2f(0, 0));
		~Button();

		void copy(const Button& button);

		void setHighlightFillColor(const sf::Color& color);
		void setHighlightOutlineColor(const sf::Color& color);
		void setHighlightOutlineThickness(float thickness);

		void setFillColor(const sf::Color& color);
		void setOutlineColor(const sf::Color& color);
		void setOutlineThickness(float thickness);

		bool contains(const sf::Vector2f& mousePos) const;
		Entity* isHit(const sf::Vector2f& mousePos);

		void activateSelection() override;
		void deactivateSelection() override;

	private:
		struct {
			sf::Color fillColor, outlineColor;
			float outlineThickness;
		} highlight, original;
	};
}
