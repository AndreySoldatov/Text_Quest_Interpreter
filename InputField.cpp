//
// Created by Andrey on 05.05.2022.
//

#include "InputField.hpp"

InputField::InputField(
        sf::Vector2f const &pos,
        sf::Font const &font,
        size_t textSize,
        const sf::Color &textColor) :
        m_textColor(textColor),
        m_pos(pos) {
    m_text.setPosition(m_pos);
    m_text.setFont(font);
    m_text.setCharacterSize(textSize);
    m_text.setFillColor(m_textColor);
    m_text.setString(m_str);

    m_cursor.setPosition(pos);
    m_cursor.setSize(sf::Vector2f(0.05f * textSize, textSize));
    m_cursor.setFillColor(textColor);
}

const std::wstring &InputField::getString() const {
    return m_str;
}

void InputField::processInput(sf::Event const &event) {
    if(event.type == sf::Event::TextEntered && m_active) {
        if(event.text.unicode >= 32 && m_str.size() < 30) {
            m_str += (wchar_t)event.text.unicode;
            m_TypingSound.play();
        }
        else if(event.text.unicode == 8 && !m_str.empty()) {
            m_str.pop_back();
            m_TypingSound.play();
        }
        m_text.setString(m_str);
        m_cursor.setPosition(sf::Vector2f(m_pos.x + m_text.getGlobalBounds().width, m_pos.y));
    }
}

void InputField::draw(sf::RenderTarget &window) {
    if(m_active) {
        window.draw(m_text);
        if (m_cl.getElapsedTime().asMilliseconds() % 1000 < 500)
            window.draw(m_cursor);
    }
}

void InputField::move(sf::Vector2f const &delta) {
    m_pos.x += delta.x;
    m_pos.y += delta.y;

    m_text.setPosition(m_pos);
    m_cursor.setPosition(sf::Vector2f(m_pos.x + m_text.getGlobalBounds().width, m_pos.y));
}

void InputField::setPosition(sf::Vector2f const &pos) {
    m_pos = pos;

    m_text.setPosition(m_pos);
    m_cursor.setPosition(sf::Vector2f(m_pos.x + m_text.getGlobalBounds().width, m_pos.y));
}

bool InputField::isActive() {
    return m_active;
}

void InputField::setActive(bool active) {
    m_active = active;
    m_TypingSound.setBuffer(m_TypingSoundBuffer);
}

sf::Vector2f InputField::getBoundBox() {
    return {m_text.getGlobalBounds().width, m_text.getGlobalBounds().height};
}

void InputField::clear() {
    m_str = L"";
    m_text.setString(m_str);
}

void InputField::setColor(sf::Color const &col) {
    m_text.setFillColor(col);
    m_cursor.setFillColor(col);
}

void InputField::setFont(sf::Font const &font) {
    m_text.setFont(font);
}

void InputField::setTypingSound(std::string const &soundPath, float volume) {
    m_TypingSoundBuffer.loadFromFile(soundPath);
    m_TypingSound.setBuffer(m_TypingSoundBuffer);
    m_TypingSound.setVolume(volume);
}
