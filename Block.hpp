//
// Created by Andrey on 05.05.2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

#include "TextPrinter.hpp"
#include "InputField.hpp"

class Block {
    float scroll_speed{};
    std::vector<TextPrinter> m_textPrinters;
    TextPrinter m_message;
    InputField m_inputField;
    sf::Music m_music;
    sf::Color m_backgroundColor;
    bool m_started{};

public:
    explicit Block(std::vector<TextPrinter> textPrinters, std::string const &musicPath = "", sf::Color const &bgColor = sf::Color::Black);

    void start();

    void processInput(sf::Event const &event, float dt);

    const std::wstring &getString();

    void message(std::wstring const &str);

    void draw(sf::RenderTarget &window);

    bool isTyping();
};


