//
// Created by Andrey on 05.05.2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class InputField {
    sf::Text m_text;
    std::wstring m_str;
    sf::Clock m_cl;
    sf::RectangleShape m_cursor;
    sf::Color m_textColor;
    sf::Vector2f m_pos;
    sf::Sound m_TypingSound;
    sf::SoundBuffer m_TypingSoundBuffer;
    bool m_active{false};

public:
    explicit InputField(
            sf::Vector2f const &pos,
            sf::Font const &font,
            size_t textSize = 64,
            sf::Color const &textColor = sf::Color::White
            );

    const std::wstring &getString() const;

    void processInput(sf::Event const &event);

    void draw(sf::RenderTarget &window);

    void setActive(bool active);

    bool isActive();

    void clear();

    void setColor(sf::Color const &col);

    void setFont(sf::Font const &font);

    void setTypingSound(std::string const &soundPath, float volume = 100);

    sf::Vector2f getBoundBox();

    void move(sf::Vector2f const &delta);
    void setPosition(sf::Vector2f const &pos);
};


