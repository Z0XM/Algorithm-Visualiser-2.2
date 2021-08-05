#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <functional>

namespace gui {
	class Frame;
	class Entity {
	private:
		unsigned int id;
		Frame* currFrame;

		bool active;
	protected:
		static unsigned int item_count;
		bool isSelected;

		std::function<void()> action;
	public:
		Entity(unsigned int class_id);
		virtual ~Entity();

		unsigned int getID() const;

		const Frame* getFrame() const;

		virtual Entity* isHit(const sf::Vector2f& mousePos) = 0;

		virtual void draw(sf::RenderTarget& target) = 0;

		virtual void activateSelection() = 0;
		virtual void deactivateSelection() = 0;

		void setActive();
		void setInactive();
		bool isActive();

		void callAction() const;
		bool hasAction() const;
		void setAction(std::function<void()> func);

		enum class ActionEvent{NONE, RELEASE, PRESS, MOVE } actionEvent;

		friend void setEntityFrame(Entity& entity, Frame* frame);
	};

	class Frame {
	private:
		std::map<unsigned int, Entity*> entityMap;
		static std::map<std::string, unsigned int> nameMap;

		sf::RenderWindow* window;

		Entity* mouseHoveringOn, *clicked;		
	public:
		Frame();
		~Frame();

		void setWindow(sf::RenderWindow& window);

		void addEntity(Entity& entity);
		void removeEntity(Entity& entity);
		static void setName(const Entity& entity, const std::string& name);
		static void removeName(const std::string& name);
		
		Entity* getByID(unsigned int id) const;
		Entity* getByName(const std::string& name) const;
		static unsigned int getIDByName(const std::string& name);
		static std::string getName(unsigned int id);

		void update();
		void pollEvents(sf::Event event);
		void draw();

		sf::Vector2f getMousePosition() const;
	};
}