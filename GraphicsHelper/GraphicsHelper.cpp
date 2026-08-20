#include "GraphicsHelper.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

sf::Text createText(const sf::Font& font, const std::string& text, float size,
                    const float xPos, const float yPos, sf::Color color) {
    sf::Text words(font, text, size);
    sf::FloatRect bounds = words.getLocalBounds();
    words.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    words.setPosition({xPos, yPos});
    words.setFillColor(color);
    
    return words;
}

sf::RectangleShape createButton(sf::Vector2f size, sf::Vector2f position, float thickness,
                                sf::Color fillColor, sf::Color outLineColor) {
    sf::RectangleShape menuButton(size);
    menuButton.setPosition(position);
    menuButton.setFillColor(fillColor);
    menuButton.setOutlineColor(outLineColor);
    menuButton.setOutlineThickness(thickness);
    return menuButton;
}

void recenterText(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
}
