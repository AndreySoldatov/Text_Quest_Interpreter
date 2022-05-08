//
// Created by Andrey on 05.05.2022.
//

#include "Block.hpp"

Block::Block(
        std::vector<TextPrinter> textPrinters,
        const std::string &musicPath,
        const sf::Color &bgColor) :
        m_textPrinters(std::move(textPrinters)),
        m_backgroundColor(bgColor)
{
    m_music.openFromFile(musicPath);
}

void Block::start() {

}

void Block::processInput(sf::Event const &event, float dt) {

}

const std::wstring &Block::getString() {
    return <#initializer#>;
}

void Block::message(std::wstring const &str) {

}

void Block::draw(sf::RenderTarget &window) {

}

bool Block::isTyping() {
    return false;
}
