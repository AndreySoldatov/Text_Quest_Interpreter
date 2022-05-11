//
// Created by Andrey on 11.05.2022.
//

#include <iostream>
#include "ColorFader.hpp"

ColorFader::ColorFader(sf::Color const &color) :
m_color{(float)color.r, (float)color.g, (float)color.b},
m_targetColor{(float)color.r, (float)color.g, (float)color.b}
{}

void ColorFader::setColor(sf::Color const &color) {
    m_targetColor = std::array<float, 3>{(float)color.r, (float)color.g, (float)color.b};
}

void ColorFader::update(float dt) {
    m_color[0] += dt * ((float)m_targetColor[0] - (float)m_color[0]);
    m_color[1] += dt * ((float)m_targetColor[1] - (float)m_color[1]);
    m_color[2] += dt * ((float)m_targetColor[2] - (float)m_color[2]);
}

sf::Color ColorFader::getColor() const {
    return {(sf::Uint8)m_color[0], (sf::Uint8)m_color[1], (sf::Uint8)m_color[2]};
}
