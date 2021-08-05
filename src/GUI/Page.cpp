#include "Page.hpp"
#include "Scroll.hpp"

gui::Page::Page(const sf::Vector2f& size)
	:Entity(4)
{
	maxSize = size;
	activeRegion = sf::FloatRect(0, 0, size.x, size.y);

	action = nullptr;
	actionEvent = ActionEvent::NONE;

	for (int i = 0; i < 4; i++) {
		connectedScroll[i] = nullptr;
	}

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color::Transparent;
}

gui::Page::~Page()
{
	for (int i = 0; i < 4; i++) {
		delete connectedScroll[i];
	}
}

void gui::Page::addEntity(Entity& entity)
{
	entityMap.insert(std::make_pair(entity.getID(), &entity));
}

void gui::Page::removeEntity(const Entity& entity)
{
	std::string name = getName(entity.getID());
	if (name != "")Frame::removeName(name);
	entityMap.erase(entity.getID());
}

void gui::Page::setName(const Entity& entity, const std::string& name)
{
	Frame::setName(entity, name);
}

gui::Entity* gui::Page::getByID(unsigned int id) const
{
	return entityMap.at(id);
}

gui::Entity* gui::Page::getByName(const std::string& name) const
{
	return entityMap.at(Frame::getIDByName(name));
}

std::string gui::Page::getName(unsigned int id)
{
	return Frame::getName(id);
}

sf::Vector2f gui::Page::getMousePosition() const
{
	return getFrame()->getMousePosition() - activePos;
}

void gui::Page::setFillColor(sf::Color color)
{
	fillColor = color;
}

void gui::Page::setOutineColor(sf::Color color)
{
	outlineColor = color;
}

void gui::setScrollAction(Page* page, int scrollPos)
{
	Scroll::ScrollPosition comp;
	if (scrollPos == Scroll::LEFT)comp = Scroll::RIGHT;
	else if (scrollPos == Scroll::TOP)comp = Scroll::BOTTOM;
	else if (scrollPos == Scroll::RIGHT)comp = Scroll::LEFT;
	else if (scrollPos == Scroll::BOTTOM)comp = Scroll::TOP;

	page->connectedScroll[scrollPos]->bar.setAction([page, scrollPos, comp]() {
		sf::Vector2f offset = page->connectedScroll[scrollPos]->scrollTo(page->getMousePosition());
		if(page->connectedScroll[comp] != nullptr)page->connectedScroll[comp]->scrollBy(offset);
		page->moveActiveRegion({
			offset.x * page->getSize().x / page->connectedScroll[scrollPos]->rect.getSize().x,
			offset.y * page->getSize().y / page->connectedScroll[scrollPos]->rect.getSize().y
			});
	});
	page->connectedScroll[scrollPos]->rect.setAction([page, scrollPos, comp]() {
		sf::Vector2f offset = page->connectedScroll[scrollPos]->scrollTo(page->getMousePosition());
		if (page->connectedScroll[comp] != nullptr)page->connectedScroll[comp]->scrollBy(offset);
		page->moveActiveRegion({
			offset.x * page->getSize().x / page->connectedScroll[scrollPos]->rect.getSize().x,
			offset.y * page->getSize().y / page->connectedScroll[scrollPos]->rect.getSize().y
			});
	});
}

void gui::Page::setScroll(int scrollPos)
{
	if (connectedScroll[scrollPos] == nullptr) connectedScroll[scrollPos] = new Scroll();

	setScrollAction(this, scrollPos);

	connectedScroll[scrollPos]->setPage(*this, (Scroll::ScrollPosition)scrollPos);
}

void gui::Page::removeScroll(int scrollPos)
{
	delete connectedScroll[scrollPos];
	connectedScroll[scrollPos] = nullptr;
}

void gui::Page::limitActiveRegion()
{
	activeRegion.left = std::max(0.f, activeRegion.left);
	activeRegion.left = std::min(maxSize.x, activeRegion.left);

	activeRegion.top = std::max(0.f, activeRegion.top);
	activeRegion.top = std::min(maxSize.y, activeRegion.top);

	activeRegion.width = std::max(0.f, activeRegion.width);
	if (activeRegion.left + activeRegion.width >= maxSize.x)activeRegion.width = maxSize.x - activeRegion.left;
	activeRegion.height = std::max(0.f, activeRegion.height);
	if (activeRegion.top + activeRegion.height >= maxSize.y)activeRegion.height = maxSize.y - activeRegion.top;
}

void gui::Page::setActiveRegion(const sf::FloatRect& region)
{
	activeRegion = region;
	limitActiveRegion();
}

sf::FloatRect gui::Page::getActiveRegion() const
{
	return activeRegion;
}

void gui::Page::moveActiveRegion(float offsetX, float offsetY)
{
	activeRegion.left += offsetX;
	activeRegion.top += offsetY;
	limitActiveRegion();
}

void gui::Page::moveActiveRegion(const sf::Vector2f& offset)
{
	activeRegion.left += offset.x;
	activeRegion.top += offset.y;
	limitActiveRegion();
}

void gui::Page::setSize(const sf::Vector2f& size)
{
	maxSize = size;
}

sf::Vector2f gui::Page::getSize() const
{
	return maxSize;
}

void gui::Page::setPosition(float x, float y)
{
	activePos.x = x;
	activePos.y = y;
}

void gui::Page::setPosition(const sf::Vector2f& position)
{
	activePos = position;
}

sf::Vector2f gui::Page::getPosition() const
{
	return activePos;
}

void gui::Page::move(float offsetX, float offsetY)
{
	activePos.x += offsetX;
	activePos.y += offsetY;
}

void gui::Page::move(const sf::Vector2f& offset)
{
	activePos += offset;
}

gui::Entity* gui::Page::isHit(const sf::Vector2f& mousePos)
{
	if (isActive()) {
		sf::Vector2f mPos = mousePos - activePos;
		if (mPos.x < 0 || mPos.x >= activeRegion.width || mPos.y < 0 || mPos.y >= activeRegion.height)return nullptr;

		mPos += sf::Vector2f(activeRegion.left, activeRegion.top);

		Entity* entity = nullptr;

		for (int i = 0; i < 4 && entity == nullptr; i++) {
			if (connectedScroll[i] != nullptr)entity = connectedScroll[i]->isHit(mousePos - activePos);
		}

		for (auto it = entityMap.begin(); it != entityMap.end() && entity == nullptr; it++) {
			entity = it->second->isHit(mPos);
		}
		return entity;
	}
	return nullptr;
}

void gui::Page::activateSelection()
{
}

void gui::Page::deactivateSelection()
{
}

void gui::Page::draw(sf::RenderTarget& target)
{
	if (isActive()) {
		sf::Vertex v[4] = {
			{{activePos.x, activePos.y}, outlineColor},
			{{activePos.x + activeRegion.width, activePos.y}, outlineColor},
			{{activePos.x + activeRegion.width, activePos.y + activeRegion.height}, outlineColor},
			{{activePos.x, activePos.y + activeRegion.height}, outlineColor}
		};

		target.draw(v, 4, sf::LinesStrip);

		v[0].color = v[1].color = v[2].color = v[3].color = fillColor;

		target.draw(v, 4, sf::Quads);

		sf::RenderTexture rt;
		rt.create(activeRegion.width, activeRegion.height);
		rt.clear(sf::Color::Transparent);

		for (auto it = entityMap.begin(); it != entityMap.end(); it++) {
			it->second->draw(rt);
		}
		rt.display();
		sf::Sprite s;
		s.setTexture(rt.getTexture());
		s.setTextureRect(sf::IntRect(activeRegion));
		s.setPosition(activePos);
		target.draw(s);
		
		for (int i = 0; i < 4; i++) {
			if (connectedScroll[i] == nullptr)continue;
			connectedScroll[i]->move(activePos);
			connectedScroll[i]->draw(target);
			connectedScroll[i]->move(-activePos);
		}
	}
}
