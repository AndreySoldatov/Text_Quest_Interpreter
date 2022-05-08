//
// Created by Andrey on 05.05.2022.
//

#include "TextPrinter.hpp"

#include <iostream>

TextPrinter::TextPrinter(
        sf::Font const &font,
        const std::string &str,
        const sf::Color &textColor,
        sf::Vector2f const &pos,
        size_t textSize,
        size_t del) :
        m_fullString(str),
        m_textColor(textColor),
        m_pos(pos),
        m_del(del),
        m_textSize(textSize)
{
    m_text.setPosition(m_pos);
    m_text.setFont(font);
    m_text.setCharacterSize(m_textSize);
    m_text.setFillColor(m_textColor);
    m_text.setString(m_typingString);
}

bool TextPrinter::isTyping() {
    return m_isTyping;
}

void TextPrinter::draw(sf::RenderTarget &window) {
    window.draw(m_text);
}

void TextPrinter::start() {
    m_isTyping = true;
    m_clock.restart();
    m_typingString = "";
    m_text.setString(m_typingString);

    m_TypingSound.setBuffer(m_TypingSoundBuffer);
    if(m_withSound) {
        m_soundEffect.setBuffer(m_SoundBuffer);
        m_soundEffect.play();
    }
}

void TextPrinter::update() {
    if(m_isTyping && (size_t)m_clock.getElapsedTime().asMilliseconds() >= m_del) {
        m_clock.restart();
        if(m_typingString.size() < m_fullString.size()) {
            m_typingString += m_fullString[m_typingString.size()];
            m_text.setString(m_typingString);
            m_TypingSound.play();
        } else {
            m_isTyping = false;
        }
    }
}

void TextPrinter::skip() {
    m_typingString = m_fullString;
    m_text.setString(m_typingString);
}

sf::Vector2f TextPrinter::getBoundBox() {
    sf::Text tmp = m_text;
    tmp.setString(m_fullString);
    return {tmp.getGlobalBounds().width, tmp.getGlobalBounds().height};
}

void TextPrinter::move(sf::Vector2f const &delta) {
    m_pos.x += delta.x;
    m_pos.y += delta.y;

    m_text.setPosition(m_pos);
}

void TextPrinter::setPosition(sf::Vector2f const &pos) {
    m_pos = pos;
    m_text.setPosition(m_pos);
}

void TextPrinter::setString(std::string const &str) {
    m_fullString = str;
    m_typingString = "";
}

bool TextPrinter::finished() {
    return !isTyping() && m_typingString.size() == m_fullString.size();
}

sf::Vector2f TextPrinter::getPosistion() {
    return m_pos;
}

void TextPrinter::setColor(sf::Color const &col) {
    m_text.setFillColor(col);
}

void TextPrinter::setFont(sf::Font const &font) {
    m_text.setFont(font);
}

void TextPrinter::setSound(std::string const &soundPath, float volume) {
    m_SoundBuffer.loadFromFile(soundPath);
    m_soundEffect.setBuffer(m_SoundBuffer);
    m_soundEffect.setVolume(volume);
    m_withSound = true;
}

void TextPrinter::setTypingSound(std::string const &soundPath, float volume) {
    m_TypingSoundBuffer.loadFromFile(soundPath);
    m_TypingSound.setBuffer(m_TypingSoundBuffer);
    m_TypingSound.setVolume(volume);
}
