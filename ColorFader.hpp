//
// Created by Andrey on 11.05.2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <array>

class ColorFader {
    std::array<float, 3> m_color{0, 0, 0};
    std::array<float, 3> m_targetColor{0, 0, 0};

public:
    ColorFader() = default;
    explicit ColorFader(sf::Color const &color);

    void setColor(sf::Color const &color);

    void update(float dt);

    [[nodiscard]] sf::Color getColor() const;
};


