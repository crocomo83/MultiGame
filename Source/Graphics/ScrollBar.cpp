#include "../../Headers/Graphics/ScrollBar.h"

ScrollBar::ScrollBar(sf::Vector2f upPosition_, float height_, float thickness_)
	: upPosition(upPosition_)
	, height(height_)
	, thickness(thickness_)
{
	// Global ellipse
	sf::RectangleShape outerRectangle({ thickness, height });
	outerRectangle.setPosition({ upPosition.x - thickness / 2, upPosition.y });
	outerRectangle.setFillColor(sf::Color(150, 150, 150));
	globalEllipse.rectangle = outerRectangle;

	sf::CircleShape upCircle(thickness / 2);
	upCircle.setPosition(upPosition.x - thickness/2, upPosition.y - thickness/2);
	upCircle.setFillColor(sf::Color(150, 150, 150));
	globalEllipse.upCircle = upCircle;

	sf::CircleShape downCircle(thickness / 2);
	downCircle.setPosition({ upPosition.x - thickness / 2, upPosition.y - thickness / 2 + height });
	downCircle.setFillColor(sf::Color(150, 150, 150));
	globalEllipse.downCircle = downCircle;

	// Inner ellipse
	sf::RectangleShape outerRectangle2({ thickness, height });
	outerRectangle2.setPosition({ upPosition.x - thickness / 2, upPosition.y });
	outerRectangle2.setFillColor(sf::Color::Black);
	innerEllipse.rectangle = outerRectangle2;

	sf::CircleShape upCircle2(thickness / 2);
	upCircle2.setPosition(outerRectangle2.getPosition() - sf::Vector2f(0, thickness / 2));
	upCircle2.setFillColor(sf::Color::Black);
	innerEllipse.upCircle = upCircle2;

	sf::CircleShape downCircle2(thickness / 2);
	downCircle2.setPosition(upCircle2.getPosition() + sf::Vector2f(0, height));
	downCircle2.setFillColor(sf::Color::Black);
	innerEllipse.downCircle = downCircle2;
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::update(sf::Vector2i mousePosition)
{
}

void ScrollBar::render(sf::RenderWindow& window)
{
	window.draw(globalEllipse.rectangle);
	window.draw(globalEllipse.upCircle);
	window.draw(globalEllipse.downCircle);

	window.draw(innerEllipse.rectangle);
	window.draw(innerEllipse.upCircle);
	window.draw(innerEllipse.downCircle);
}

int ScrollBar::handleEvent(const sf::Event& event)
{
	return 0;
}

void ScrollBar::reset()
{
}

void ScrollBar::setHeightInnerEllipsePercent(float percent) {
	sf::Vector2f size = innerEllipse.rectangle.getSize();
	innerEllipse.rectangle.setSize({ size.x, percent * height / 100 });
	innerEllipse.upCircle.setPosition(innerEllipse.rectangle.getPosition() - sf::Vector2f(0, thickness / 2));
	innerEllipse.downCircle.setPosition(innerEllipse.upCircle.getPosition() + sf::Vector2f(0, innerEllipse.rectangle.getSize().y));
}

void ScrollBar::setPositionInnerEllipsePercent(float percent) {
	innerEllipse.rectangle.setPosition({ upPosition.x - thickness / 2, upPosition.y + percent * height});
	innerEllipse.upCircle.setPosition(innerEllipse.rectangle.getPosition() - sf::Vector2f(0, thickness / 2));
	innerEllipse.downCircle.setPosition(innerEllipse.upCircle.getPosition() + sf::Vector2f(0, innerEllipse.rectangle.getSize().y));
}