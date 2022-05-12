//
// Created by Andrey on 10.05.2022.
//

#include <iostream>
#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer(std::string const &musicPath) {
    m_musicPath = musicPath;
    m_musicSource.openFromFile(m_musicPath);
    m_volume = 0.0f;
    m_musicSource.setVolume(m_volume);
    m_musicSource.setLoop(true);
}

bool MusicPlayer::setPath(std::string const &musicPath) {
    m_musicPath = musicPath;
    bool res = m_musicSource.openFromFile(m_musicPath);
    m_musicSource.setVolume(m_volume);
    m_musicSource.setLoop(true);
    return res;
}

void MusicPlayer::fadeIn(float maxVolume) {
    m_maxVolume = maxVolume;
    m_musicSource.play();
    m_fadingIn = true;
    m_fadingOut = false;
    m_isPlaying = false;
}

void MusicPlayer::fadeOut() {
    m_fadingOut = true;
    m_fadingIn = false;
    m_isPlaying = false;
}

void MusicPlayer::update(float dt) {
    if(m_fadingIn) {
        if (m_volume < m_maxVolume) {
            m_volume += dt * 15;
        } else {
            m_volume = m_maxVolume;
            m_fadingIn = false;
            m_isPlaying = true;
        }
    }
    if(m_fadingOut) {
        if (m_volume > 0.0f) {
            m_volume -= dt * 15;
        } else {
            m_volume = 0.0;
            m_fadingOut = false;
            m_musicSource.stop();
        }
    }
    m_volume = std::max(std::min(m_maxVolume, m_volume), 0.0f);
    m_musicSource.setVolume(m_volume);
    //std::cout << m_maxVolume << ", " << m_volume << "\n";
}

bool MusicPlayer::isSame(std::string const &musicPath) {
    return musicPath == m_musicPath;
}

void MusicPlayer::setVolume(float volume) {
    m_maxVolume = volume;
    m_volume = m_maxVolume;
    m_musicSource.setVolume(volume);
}
