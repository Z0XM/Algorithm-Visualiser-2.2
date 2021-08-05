#pragma once
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "Slider.hpp"
#include "GUIFrame.hpp"

namespace gui {
	class Scroll;
	class Page : public Entity{
	private:
		sf::Vector2f activePos;
		sf::Vector2f maxSize;
		sf::FloatRect activeRegion;

		std::map<unsigned int, Entity*> entityMap;

		Scroll* connectedScroll[4];

		sf::Color fillColor, outlineColor;

		void limitActiveRegion();
	public:
		Page(const sf::Vector2f& size = sf::Vector2f(0, 0));
		~Page();

		void addEntity(Entity& entity);
		void removeEntity(const Entity& entity);
		void setName(const Entity& entity, const std::string& name);

		Entity* getByID(unsigned int id) const;
		Entity* getByName(const std::string& name) const;
		std::string getName(unsigned int id);

		sf::Vector2f getMousePosition() const;

		void setFillColor(sf::Color color);
		void setOutineColor(sf::Color color);

		void setScroll(int scrollPos);
		void removeScroll(int scrollPos);

		void setActiveRegion(const sf::FloatRect& region);
		sf::FloatRect getActiveRegion() const;
		void moveActiveRegion(float offsetX, float offsetY);
		void moveActiveRegion(const sf::Vector2f& offset);

		void setSize(const sf::Vector2f& size);
		sf::Vector2f getSize() const;

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		sf::Vector2f getPosition() const;

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		Entity* isHit(const sf::Vector2f& mousePos);

		virtual void activateSelection() override;
		virtual void deactivateSelection() override;

		void setAction(std::function<void()> func) = delete;

		void draw(sf::RenderTarget&);

		friend void setScrollAction(Page* page, int scrollPos);
	};
}