#pragma once

#include "IDrawable.h"

class ScrollBar : public IDrawable {

public:
	ScrollBar(sf::Vector2f upPosition_, float height_, float thickness_ = 10.f);
	~ScrollBar();

	// IDrawable methods
	void	update(sf::Vector2i mousePosition) override;
	void	render(sf::RenderWindow& window) override;
	int		handleEvent(const sf::Event& event) override;
	void	reset() override;

	void	setHeightInnerEllipsePercent(float percent);
	void	setPositionInnerEllipsePercent(float percent);

private:
	struct Ellipse {
		sf::RectangleShape rectangle;
		sf::CircleShape upCircle;
		sf::CircleShape downCircle;
	};

	Ellipse globalEllipse;
	Ellipse innerEllipse;

	sf::Vector2f upPosition;
	float height;
	float thickness;
};