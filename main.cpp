#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <regex>
#include <array>
#include <filesystem>

#include "json.hpp"
#include "TextPrinter.hpp"
#include "InputField.hpp"

using namespace nlohmann;

void fillBlock(
        std::string const &id, json &js,
        std::vector<TextPrinter> &printers,
        sf::Music &mus,
        sf::Color &backgroundColor,
        InputField &input,
        TextPrinter &message,
        std::vector<std::regex> &reg,
        std::array<sf::Font, 4> &fonts,
        sf::Vector2f &pos,
        sf::Shader &shader,
        std::filesystem::path const &programPath,
        std::filesystem::path const &gamePath) {
    mus.stop();
    if(js[id].contains("music")) {
        mus.openFromFile(gamePath.string() + "\\music\\" + (std::string)js[id]["music"]["file"]);
        if(js[id]["music"].contains("volume")) {
            mus.setVolume(js[id]["music"]["volume"]);
        }
        mus.setLoop(true);
        mus.play();
    }

    if(js[id].contains("font")) {
        if(js[id]["font"].contains("regular")) {
            fonts[0].loadFromFile(gamePath.string() + "\\fonts\\" + (std::string)js[id]["font"]["regular"]);
        }
        if(js[id]["font"].contains("italic")) {
            fonts[1].loadFromFile(gamePath.string() + "\\fonts\\" + (std::string)js[id]["font"]["italic"]);
        }
        if(js[id]["font"].contains("bold")) {
            fonts[2].loadFromFile(gamePath.string() + "\\fonts\\" + (std::string)js[id]["font"]["bold"]);
        }
        if(js[id]["font"].contains("boldItalic")) {
            fonts[3].loadFromFile(gamePath.string() + "\\fonts\\" + (std::string)js[id]["font"]["boldItalic"]);
        }
    } else {
        fonts[0].loadFromFile((programPath / "fonts\\regular.ttf").string());
        fonts[1].loadFromFile((programPath / "fonts\\italic.ttf").string());
        fonts[2].loadFromFile((programPath / "fonts\\bold.ttf").string());
        fonts[3].loadFromFile((programPath / "fonts\\boldItalic.ttf").string());
    }

    if(js[id].contains("shader")) {
        std::ifstream ifs(gamePath.string() + "\\shaders\\" + (std::string)js[id]["shader"]);
        std::stringstream ss;
        ss << ifs.rdbuf();
        shader.loadFromMemory(ss.str(), sf::Shader::Fragment);
    } else {
        std::ifstream ifs(programPath.string() + "\\shader\\default.glsl");
        std::stringstream ss;
        ss << ifs.rdbuf();
        shader.loadFromMemory(ss.str(), sf::Shader::Fragment);
    }

    if(js[id].contains("backgroundColor")) {
        backgroundColor.r = js[id]["backgroundColor"][0];
        backgroundColor.g = js[id]["backgroundColor"][1];
        backgroundColor.b = js[id]["backgroundColor"][2];
    } else {
        backgroundColor = sf::Color::Black;
    }

    if(js[id].contains("textColor")) {
        sf::Color c{js[id]["textColor"][0], js[id]["textColor"][1], js[id]["textColor"][2]};
        input.setColor(c);
        message.setColor(c);
    } else {
        input.setColor(sf::Color::White);
        message.setColor(sf::Color::White);
    }

    printers.clear();
    for(auto &line : js[id]["lines"]) {
        size_t fontIndex{};
        if(line.contains("font")) {
            if(line["font"] == "regular") fontIndex = 0;
            if(line["font"] == "italic") fontIndex = 1;
            if(line["font"] == "bold") fontIndex = 2;
            if(line["font"] == "boldItalic") fontIndex = 3;
        }
        printers.emplace_back(TextPrinter(
                fonts[fontIndex],
                line["string"],
                (line.contains("textColor") ? sf::Color(line["textColor"][0], line["textColor"][1], line["textColor"][2]) : sf::Color::White),
                pos,
                (line.contains("textSize") ? (float)line["textSize"] : 32.0f),
                (line.contains("del") ? (float)line["del"] : 30)
        ));
        if(line.contains("sound")) {
            float volume{100};
            if(line["sound"].contains("volume")) {
                volume = line["sound"]["volume"];
            }
            printers[printers.size() - 1].setSound(gamePath.string() + "\\sounds\\" + (std::string)line["sound"]["file"], volume);
        }

        float typingVolume{100};
        std::string typingSoundPath;
        if(line.contains("typeSound")) {
            typingSoundPath = gamePath.string() + "\\sounds\\" + (std::string)line["typeSound"]["file"];
            if(line["typeSound"].contains("volume")) {
                typingVolume = (float)line["typeSound"]["volume"];
            }
        } else {
            typingSoundPath = programPath.string() + "\\sounds\\typeSound.ogg";
        }
        printers[printers.size() - 1].setTypingSound(typingSoundPath, typingVolume);

        pos.y += printers[printers.size() - 1].getBoundBox().y + 40.0;
    }

    reg.clear();
    for (size_t i = 0; i < js[id]["links"].size(); ++i) {
        std::string regStr;
        for (size_t j = 0; j < js[id]["links"][i]["keys"].size(); j++) {
            regStr.append(js[id]["links"][i]["keys"][j]);
            if(j < js[id]["links"][i]["keys"].size() - 1) {
                regStr += "|";
            }
        }
        reg.emplace_back(std::regex(regStr, std::regex_constants::icase));
    }

    std::ofstream saveFile(gamePath.string() + "\\save\\saveFile", std::ios::trunc);
    saveFile << id;
    saveFile.close();
}

int main([[maybe_unused]]int argc, char** argv) {
    std::filesystem::path programPath = argv[0];
    programPath = programPath.parent_path();
    std::filesystem::path gamePath = argv[1];
    gamePath = gamePath.parent_path();

    std::cout << programPath.string() << std::endl;

    std::ifstream ifs(argv[1]);
    json js;
    ifs >> js;

    sf::Clock deltaTimeClock;
    float oldTime{};
    float dt;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode(1600, 900), "", sf::Style::None, settings);
    window.setFramerateLimit(60);

    std::array<sf::Font, 4> fonts;
    fonts[0].loadFromFile((programPath / "fonts\\regular.ttf").string());
    fonts[1].loadFromFile((programPath / "fonts\\italic.ttf").string());
    fonts[2].loadFromFile((programPath / "fonts\\bold.ttf").string());
    fonts[3].loadFromFile((programPath / "fonts\\boldItalic.ttf").string());

    sf::Vector2f pos{100, 100};
    sf::Vector2f inpPos{800, 900 - 4 * 32};

    size_t currentPrinter{0};
    float scrollSpeed{};

    std::vector<TextPrinter> printers;
//    printers.emplace_back(TextPrinter(font, "First line.\nLololo Same text", sf::Color::White, pos));
//    pos.y += printers[0].getBoundBox().y + 40.0;
//    printers.emplace_back(TextPrinter(font, "This is second line (angry >:( ).", sf::Color::Red, pos, 64));
//    pos.y += printers[1].getBoundBox().y + 40.0;
//    printers.emplace_back(TextPrinter(font, "This is third line Yellow.", sf::Color::Yellow, pos, 24));
    std::string currentId = "start";

    std::ifstream saveFile(gamePath.string() + "\\save\\saveFile");
    if(saveFile.good()) {
        std::stringstream ss;
        ss << saveFile.rdbuf();
        currentId = ss.str();
    }

    sf::Music mus;
    sf::Color backgroundColor = sf::Color::Black;
    std::vector<std::regex> regs;

    std::vector<sf::SoundBuffer> buffers;

    InputField input(inpPos, fonts[0], 32);
    input.setTypingSound(programPath.string() + "\\sounds\\inputSound.ogg");

    TextPrinter message(fonts[0], "", sf::Color::White, {inpPos.x, inpPos.y + 2 * 32});
    message.setTypingSound(programPath.string() + "\\sounds\\typeSound.ogg");
    message.start();

    sf::Shader shader;

    fillBlock(currentId, js, printers, mus, backgroundColor, input, message, regs, fonts, pos, shader, programPath, gamePath);

    sf::RectangleShape pad;
    pad.setSize({1600, 32 * 5});
    pad.setPosition({0, 900 - 32 * 5});
    pad.setFillColor(backgroundColor);

    sf::RenderTexture texture;
    texture.create(1600, 900);

    shader.setUniform("scale", sf::Vector2f{1600, 900});

    while (window.isOpen()) {
        dt = deltaTimeClock.getElapsedTime().asSeconds() - oldTime;
        oldTime += dt;

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                window.close();

            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter && currentPrinter < printers.size()) {
//                std::cout << "printer " << currentPrinter + 1 << " is " << (printers[currentPrinter].isTyping() ? "" : "not") << " typing";
//                std::cout << " and " << (printers[currentPrinter].finished() ? "" : "not") << " finished.\n\n";
                if(!printers[currentPrinter].isTyping() && !printers[currentPrinter].finished()) {
                    printers[currentPrinter].start();
                } else if(printers[currentPrinter].isTyping()) {
                    printers[currentPrinter].skip();
                    if(currentPrinter < printers.size() - 1)
                        currentPrinter++;
                } else if(printers[currentPrinter].finished() && currentPrinter < printers.size() - 1) {
                    printers[++currentPrinter].start();
                } if(input.isActive()) {
                    std::string str(input.getString().begin(), input.getString().end());
                    bool found{false};
                    for (size_t i = 0; i < regs.size(); i++) {
                        if(std::regex_search(str, regs[i])) {
                            if(js[currentId]["links"][i].contains("message")) {
                                message.setString(js[currentId]["links"][i]["message"]);
                                message.setPosition({inpPos.x - message.getBoundBox().x / 2, inpPos.y + 32 * 2});
                            } else if(js[currentId]["links"][i].contains("id")) {
                                currentId = js[currentId]["links"][i]["id"];
                                pos.y = 100;
                                fillBlock(currentId, js, printers, mus, backgroundColor, input, message, regs, fonts, pos, shader, programPath, gamePath);
                                currentPrinter = 0;
                                printers[currentPrinter].start();
                                pad.setFillColor(backgroundColor);
                                input.setActive(false);
                                message.setString("");
                            }
                            found = true;
                            message.start();
                            input.clear();
                            break;
                        }
                    }
                    if(!found) {
                        if(js[currentId].contains("defaultMessage")) {
                            message.setString(js[currentId]["defaultMessage"]);
                        } else {
                            message.setString("Unknown command");
                        }
                        message.setPosition({inpPos.x - message.getBoundBox().x / 2, inpPos.y + 32 * 2});
                        message.start();
                        input.clear();
                    }
                }
            }

            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F4) {
                window.close();
                if(window.getSize().x > 1600) {
                    window.create(sf::VideoMode{1600, 900}, "", sf::Style::None, settings);
                } else {
                    window.create(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen, settings);
                }
                window.setFramerateLimit(60);
                texture.create(window.getSize().x, window.getSize().y);
                shader.setUniform("scale", (sf::Vector2f)window.getSize());

                inpPos = {window.getSize().x / 2.0f, (float)window.getSize().y - 4 * 32};
                input.setPosition(inpPos);
                message.setPosition({inpPos.x - message.getBoundBox().x / 2, inpPos.y + 32 * 2});

                pad.setSize({(float)window.getSize().x, 32 * 5});
                pad.setPosition({0, (float)window.getSize().y - 32 * 5});
            }

            if(event.type == sf::Event::MouseWheelScrolled) {
                scrollSpeed += event.mouseWheelScroll.delta * dt * 100.0f;
            }

            input.processInput(event);
        }

        if(printers[currentPrinter].getPosistion().y + printers[currentPrinter].getBoundBox().y > (float)(window.getSize().y - 32 * 5) && !printers[currentPrinter].finished()) {
            scrollSpeed -= 100.0f * dt;
        }

        if(printers[currentPrinter].getPosistion().y < 0) {
            scrollSpeed += 100.0f * dt;
        }

        if(printers[0].getPosistion().y + printers[0].getBoundBox().y > (float)(window.getSize().y - 32 * 5)) {
            scrollSpeed -= 100.0f * dt;
        }

        scrollSpeed *= (1.0f - 5.0f * dt);

        input.setPosition({inpPos.x - input.getBoundBox().x / 2.0f, inpPos.y});

        if(currentPrinter == printers.size() - 1 && printers[currentPrinter].finished() && !input.isActive()) {
            input.setActive(true);
        }

        for (auto &p : printers) {
            p.update();
            p.move({0, scrollSpeed});
        }
        message.update();

        texture.clear(backgroundColor);
        for (auto &p : printers) {
            p.draw(texture);
        }

        texture.draw(pad);
        message.draw(texture);
        input.draw(texture);

        shader.setUniform("scale", (sf::Vector2f)window.getSize());
        shader.setUniform("time", deltaTimeClock.getElapsedTime().asSeconds());
        shader.setUniform("text", texture.getTexture());

        window.clear(backgroundColor);
        sf::RectangleShape drawRect;
        drawRect.setSize((sf::Vector2f)window.getSize());
        window.draw(drawRect, &shader);
//        for (auto &p : printers) {
//            p.draw(window);
//        }
//
//        window.draw(pad);
//        message.draw(window);
//        input.draw(window);

        window.display();
    }
    
    return 0;
}