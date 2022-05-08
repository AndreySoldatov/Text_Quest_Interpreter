//
// Created by Andrey on 05.05.2022.
//

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class TextPrinter {
    std::string m_fullString;
    std::string m_typingString;
    sf::Text m_text;
    sf::Color m_textColor;
    sf::Clock m_clock;
    sf::Vector2f m_pos;
    size_t m_del{};
    size_t m_textSize{};
    bool m_isTyping{false};
    sf::Sound m_soundEffect;
    sf::SoundBuffer m_SoundBuffer;
    sf::Sound m_TypingSound;
    sf::SoundBuffer m_TypingSoundBuffer;
    bool m_withSound{false};

public:
    TextPrinter() = delete;
    explicit TextPrinter(
            sf::Font const &font,
            std::string const &str = "This is a default sentence.\nNew line!",
            sf::Color const &textColor = sf::Color::White,
            sf::Vector2f const &pos = {100, 100},
            size_t textSize = 32,
            size_t del = 50);

    void start();

    bool isTyping();
    bool finished();

    void draw(sf::RenderTarget &window);

    void skip();

    void setSound(std::string const &soundPath, float volume = 100);
    void setTypingSound(std::string const &soundPath, float volume = 100);

    void update();

    void setString(std::string const &str);

    sf::Vector2f getBoundBox();

    void move(sf::Vector2f const &delta);
    void setPosition(sf::Vector2f const &pos);

    void setColor(sf::Color const &col);

    void setFont(sf::Font const &font);

    sf::Vector2f getPosistion();
};


