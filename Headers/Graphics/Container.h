#pragma once

#include "IDrawable.h"

class Container : public IDrawable {

public:
			Container();
			~Container();

	// IDrawable methods
	void	update(sf::Vector2i mousePosition) override;
	void	render(sf::RenderWindow& window) override;
	int		handleEvent(const std::optional<sf::Event> event) override;
	void	reset() override;
		 
	void    addWidget(IDrawable* widget);
	void	clearAll();

private:
	std::vector<IDrawable*> listWidgets;
};