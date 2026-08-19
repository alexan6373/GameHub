#ifndef GRAPHICSHELPER_H
#define GRAPHICSHELPER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

sf::Text createText(const sf::Font& font, const std::string& text, float size,
                    const float xPos, const float yPos,
                    sf::Color color = sf::Color::Black);

sf::RectangleShape createButton(sf::Vector2f size, sf::Vector2f position, float thickness,
                                sf::Color fillColor = sf::Color::Transparent,
                                sf::Color outLineColor = sf::Color::Black);

#endif
