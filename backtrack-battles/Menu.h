#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "GameState.h"
#include "GameSolver.h"
#include "Game.h"
const int MENU_WIDTH = 600;
const int MENU_HEIGHT = 400;
class Menu {
private:
    sf::RenderWindow window;
    shared_ptr<sf::Font> font;
    int selectedTokenCount;

    sf::RectangleShape startButton;
    optional<sf::Text> startButtonText;

    sf::RectangleShape minusButton;
    optional<sf::Text> minusButtonText;

    sf::RectangleShape plusButton;
    optional<sf::Text> plusButtonText;

    optional<sf::Text> tokenText;

public:
    Menu() : window(sf::VideoMode(sf::Vector2u(MENU_WIDTH, MENU_HEIGHT)), "Start Menu"), selectedTokenCount(3) {
        font = make_shared<sf::Font>();
        if (!font->openFromFile("Arial.ttf")) {
            cerr << "Failed to load font\n";
        }

        startButton.setSize(sf::Vector2f(120, 50));
        startButton.setPosition(sf::Vector2f(240, 170));
        startButton.setFillColor(sf::Color(100, 200, 100));

        startButtonText = sf::Text(*font, "Start", 20);
        startButtonText->setFillColor(sf::Color::Black);
        startButtonText->setPosition(sf::Vector2f(265, 180));

        // Minus Button
        minusButton.setSize(sf::Vector2f(40, 40));
        minusButton.setPosition(sf::Vector2f(200, 100));
        minusButton.setFillColor(sf::Color(200, 100, 100));

        minusButtonText = sf::Text(*font, "-", 30);
        minusButtonText->setFillColor(sf::Color::Black);
        minusButtonText->setPosition(sf::Vector2f(215, 100));

        // Plus Button
        plusButton.setSize(sf::Vector2f(40, 40));
        plusButton.setPosition(sf::Vector2f(360, 100));
        plusButton.setFillColor(sf::Color(100, 200, 100));

        plusButtonText = sf::Text(*font, "+", 30);
        plusButtonText->setFillColor(sf::Color::Black);
        plusButtonText->setPosition(sf::Vector2f(370, 100));

        // Token Count Text
        tokenText = sf::Text(*font, "Tokens: 3", 24);
        tokenText->setFillColor(sf::Color::Black);
        tokenText->setPosition(sf::Vector2f(250, 50));
    }

    void run() {
        while (window.isOpen()) {
            while (optional event = window.pollEvent()) {
                if (!event) continue;

                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (event->is<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);

                    if (isMouseOver(mouse, minusButton) && selectedTokenCount > 3) {
                        selectedTokenCount--;
                        updateTokenText();
                    }

                    if (isMouseOver(mouse, plusButton) && selectedTokenCount < 10) {
                        selectedTokenCount++;
                        updateTokenText();
                    }

                    if (isMouseOver(mouse, startButton)) {
                        Game game(selectedTokenCount);
                        game.run();
                    }
                }
            }

            window.clear(sf::Color::White);

            if (tokenText) window.draw(*tokenText);
            window.draw(minusButton);
            if (minusButtonText) window.draw(*minusButtonText);
            window.draw(plusButton);
            if (plusButtonText) window.draw(*plusButtonText);
            window.draw(startButton);
            if (startButtonText) window.draw(*startButtonText);

            window.display();
        }
    }

private:
    bool isMouseOver(const sf::Vector2i& mouse, const sf::RectangleShape& shape) {
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse));
    }

    void updateTokenText() {
        if (tokenText) tokenText->setString("Tokens: " + to_string(selectedTokenCount));
        if (startButtonText) startButtonText->setString("Start (" + to_string(selectedTokenCount) + ")");
    }
};