//
// Created by Andrey on 10.05.2022.
//

#pragma once

#include <SFML/Audio.hpp>

class MusicPlayer {
    std::string m_musicPath;
    sf::Music m_musicSource;
    float m_maxVolume{100};
    float m_volume{};
    bool m_isPlaying{false};
    bool m_fadingIn{};
    bool m_fadingOut{};

public:
    MusicPlayer() = default;
    MusicPlayer(std::string const &musicPath);

    void setPath(std::string const &musicPath);

    bool isSame(std::string const &musicPath);

    void setVolume(float volume);

    void fadeIn(float maxVolume);
    void fadeOut();

    void update(float dt);
};


