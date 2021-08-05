#pragma once
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Page.hpp"
#include "GUIFrame.hpp"

namespace gui {
	class Scroll : public Entity{
	private:
		Button bar, rect;

	public:
		Scroll();
		~Scroll();

		const sf::Vector2f& getSize() const;

		enum ScrollPosition {LEFT, RIGHT, TOP, BOTTOM};
	
		sf::Vector2f getPosition() const;

		void setPage(Page& page, ScrollPosition scrollPos = RIGHT);

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);
		sf::Vector2f scrollTo(const sf::Vector2f& mousePos);
		void scrollBy(const sf::Vector2f& offset);

		Entity* isHit(const sf::Vector2f& mousePos);

		void draw(sf::RenderTarget& target);

		void activateSelection() override;
		void deactivateSelection() override;

		friend void setScrollAction(Page* page, int scrollPos);

		void setAction(std::function<void()> func) = delete;

	private:
		ScrollPosition scrollPos;
	};
}
