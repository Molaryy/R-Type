/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include <chrono>
#include <cmath>
#include <limits>
#include "Client.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "Gameplay.hpp"
#include "INetworkClient.hpp"
#include "Main.hpp"
#include "PacketHandler.hpp"
#include "RTypeProtocol.hpp"

namespace Systems {
    inline void networkReceiver([[maybe_unused]] Registry &r) {
        auto &core = Client::getInstance();
        auto &network = core.getNetworkLib();
        auto &packet_handler = core.getPacketHandler();

        for (std::vector<uint8_t> oldest_packet = network.getOldestPacket(); !oldest_packet.empty(); oldest_packet = network.getOldestPacket()) {
            Network::Packet deserialized_packet(oldest_packet);
            packet_handler(deserialized_packet);
        }
    }

    inline void drawAllTexts(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[pos, text, colorText] : Zipper(positions, texts, colorsTexts)) {
            if (text.isDrawable)
                renderer.drawText(
                    text.text,
                    static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    text.fontSize,
                    colorText.color.r,
                    colorText.color.g,
                    colorText.color.b,
                    colorText.color.a
                );
        }
    }

    inline void changeColorOverText(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &positions = r.get_components<Position>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        auto &colorsOverText = r.get_components<Components::ColorOverText>();
        auto &menuOptions = r.get_components<Components::MenuOption>();

        for (auto &&[pos, text, colorText, colorOverText, menuOpt] : Zipper(positions, texts, colorsTexts, colorsOverText, menuOptions)) {
            if (colorOverText.isOver || menuOpt.isFocused) {
                colorText.color = colorOverText.newColor;
            } else {
                colorText.color = colorOverText.defaultColor;
            }
        }
    }

    inline void handleMouse(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &positions = r.get_components<Position>();
        auto &clickables = r.get_components<Components::ClickableText>();
        auto &colorsOverTexts = r.get_components<Components::ColorOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        const bool leftClicked = std::ranges::find(events.inputs, Graphic::Keys::LeftClick) != events.inputs.end();
        auto [mouse_x, mouse_y] = events.mouse_pos;
        std::function<void(Registry &r)> secureCallback;

        for (auto &&[pos, clickable, text, colorsOverText] : Zipper(positions, clickables, texts, colorsOverTexts)) {
            if (mouse_x >= static_cast<int>(pos.x) && mouse_x <= pos.x + static_cast<int>(text.text.size()) * text.fontSize &&
                mouse_y >= static_cast<int>(pos.y) && mouse_y <= static_cast<int>(pos.y) + text.fontSize) {
                if (leftClicked) {
                    secureCallback = clickable.callback;
                    break;
                }
                colorsOverText.isOver = true;
            } else {
                colorsOverText.isOver = false;
            }
        }
        if (secureCallback) {
            secureCallback(r);
        }
    }

    inline void spriteSheetHandler(Registry &r) {
        SparseArray<Components::Drawable> &drawables = r.get_components<Components::Drawable>();

        for (auto &&[drawable] : Zipper(drawables))
            drawable.next_frame(drawable);
    }

    inline void drawEntities(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &lifes = r.get_components<Life>();
        auto &drawables = r.get_components<Components::Drawable>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[drawable, position] : Zipper(drawables, positions)) {
            if (!drawable.can_draw)
                continue;
            renderer.drawTexture(drawable.textureID, position.x, position.y, drawable.width, drawable.height, drawable.text_x, drawable.text_y,
                                 drawable.text_width, drawable.text_height);
        }

        for (auto &&[life, drawable, position] : Zipper(lifes, drawables, positions)) {
            if (!drawable.can_draw)
                continue;
            const float life_ratio = static_cast<float>(life.current) / static_cast<float>(life.max);
            renderer.drawRectangle(position.x, position.y - 10.f, drawable.width, 5.f, 255, 0, 0, 255);
            renderer.drawRectangle(position.x, position.y - 10.f, drawable.width * life_ratio, 5.f, 0, 128, 0, 255);
        }
    }

    inline void handleInputs([[maybe_unused]] Registry &r) {
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        static Protocol::InputsKeysPacket last_inputs{};
        Protocol::InputsKeysPacket inputs{};

        inputs.input_keys[Protocol::InputKey::MOVE_UP] = std::ranges::find(events.inputs, Graphic::Keys::UpArrow) != events.inputs.end();
        inputs.input_keys[Protocol::InputKey::MOVE_DOWN] = std::ranges::find(events.inputs, Graphic::Keys::DownArrow) != events.inputs.end();
        inputs.input_keys[Protocol::InputKey::MOVE_LEFT] = std::ranges::find(events.inputs, Graphic::Keys::LeftArrow) != events.inputs.end();
        inputs.input_keys[Protocol::InputKey::MOVE_RIGHT] = std::ranges::find(events.inputs, Graphic::Keys::RightArrow) != events.inputs.end();
        inputs.input_keys[Protocol::InputKey::SHOOT] = std::ranges::find(events.inputs, Graphic::Keys::Space) != events.inputs.end();

        for (uint8_t i = 0; i < Protocol::NB_INPUTS_KEYS; ++i) {
            if (last_inputs.input_keys[i] ^ inputs.input_keys[i]) {
                for (uint8_t j = i; j < Protocol::NB_INPUTS_KEYS; ++j)
                    last_inputs.input_keys[j] = inputs.input_keys[j];
                Network::Packet packet(inputs, Protocol::INPUT_KEYS);
                Client::getInstance().getNetworkLib().send(packet.serialize());
                return;
            }
        }
    }

    inline void menuNavSystem(Registry &r) {
        static auto lastNavigationTime = std::chrono::steady_clock::now();
        constexpr std::chrono::milliseconds navigationCooldown(200);
        static bool prevUpPressed = false;
        static bool prevDownPressed = false;
        auto &positions = r.get_components<Position>();
        auto &menuOptions = r.get_components<Components::MenuOption>();
        auto &rendererTexts = r.get_components<Components::RenderText>();
        auto &colorOverTexts = r.get_components<Components::ColorOverText>();

        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();

        bool currUpPressed = (std::find(events.inputs.begin(), events.inputs.end(), Graphic::Keys::UpArrow) != events.inputs.end());
        bool currDownPressed = (std::find(events.inputs.begin(), events.inputs.end(), Graphic::Keys::DownArrow) != events.inputs.end());

        auto now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastNavigationTime) < navigationCooldown) {
            prevUpPressed = currUpPressed;
            prevDownPressed = currDownPressed;
            return;
        }

        bool upTriggered = (currUpPressed && !prevUpPressed);
        bool downTriggered = (currDownPressed && !prevDownPressed);

        prevUpPressed = currUpPressed;
        prevDownPressed = currDownPressed;

        if (!upTriggered && !downTriggered)
            return;

        std::vector<std::size_t> menuIndices;
        for (std::size_t i = 0; i < r.max_entities(); ++i) {
            if (menuOptions[i].has_value() && rendererTexts[i].has_value()) {
                menuIndices.push_back(i);
            }
        }
        if (menuIndices.empty())
            return;

        std::sort(menuIndices.begin(), menuIndices.end(), [&](std::size_t a, std::size_t b) {
            return positions[a].value().y < positions[b].value().y;
        });

        std::size_t currentIndex = 0;
        bool found = false;
        for (std::size_t i = 0; i < menuIndices.size(); i++) {
            if (menuOptions[menuIndices[i]].value().isFocused) {
                currentIndex = i;
                found = true;
                break;
            }
        }
        if (!found) {
            currentIndex = 0;
            menuOptions[menuIndices[0]].value().isFocused = true;
            colorOverTexts[menuIndices[0]].value().isOver = true;
            lastNavigationTime = now;
            return;
        }
        menuOptions[menuIndices[currentIndex]].value().isFocused = false;
        colorOverTexts[menuIndices[currentIndex]].value().isOver = false;
        if (upTriggered) {
            currentIndex = (currentIndex == 0) ? menuIndices.size() - 1 : currentIndex - 1;
        } else if (downTriggered) {
            currentIndex = (currentIndex + 1) % menuIndices.size();
        }
        menuOptions[menuIndices[currentIndex]].value().isFocused = true;
        colorOverTexts[menuIndices[currentIndex]].value().isOver = true;
        lastNavigationTime = now;
    }

    inline void handleMenuEnter(Registry &r) {
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        bool enterPressed = (std::find(events.inputs.begin(), events.inputs.end(), Graphic::Keys::Enter) != events.inputs.end());

        if (!enterPressed)
            return;

        auto &menuOptions = r.get_components<Components::MenuOption>();
        auto &clickables  = r.get_components<Components::ClickableText>();

        for (auto &&[menuOption, clickable] : Zipper(menuOptions, clickables)) {
            if (menuOption.isFocused) {
                clickable.callback(r);
                break;
            }
        }
    }
}
