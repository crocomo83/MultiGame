#include "../../Headers/Graphics/Container.h"

Container::Container()
{
}

Container::~Container()
{
	for (IDrawable* widget : listWidgets) {
		delete widget;
	}
}

void Container::update(sf::Vector2i mousePosition)
{
	for (IDrawable* widget : listWidgets) {
		widget->update(mousePosition);
	}
}

void Container::render(sf::RenderWindow& window)
{
	for (IDrawable* widget : listWidgets) {
		widget->render(window);
	}
}

int Container::handleEvent(const std::optional<sf::Event> event)
{
	for (IDrawable* widget : listWidgets) {
		widget->handleEvent(event);
	}
	return 0;
}

void Container::reset()
{
	for (IDrawable* widget : listWidgets) {
		widget->reset();
	}
}

void Container::addWidget(IDrawable* widget) {
	listWidgets.push_back(widget);
}

void Container::clearAll()
{
	listWidgets.clear();
}