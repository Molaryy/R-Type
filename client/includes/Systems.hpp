/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

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

        for (auto &&[pos, text, colorText, colorOverText] : Zipper(positions, texts, colorsTexts, colorsOverText)) {
            if (!text.isDrawable)
                continue;
            if (colorOverText.isOver) {
                colorText.color = colorOverText.newColor;
            } else {
                colorText.color = colorOverText.defaultColor;
            }
        }
    }


    #define NAME_SIZE 30
    inline void handleInputBox(Registry &r) {
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        auto &inputTexts = r.get_components<Components::InputText>();
        auto &positions = r.get_components<Position>();
        auto &inputs = r.get_components<Components::Input>();
        auto &rects = r.get_components<Components::Rect>();
        auto &colorsTexts = r.get_components<Components::ColorText>();

        for (auto &&[inputText, pos, input, colorText] : Zipper(inputTexts,  positions, inputs, colorsTexts)) {
            for (auto &&[rect, inputRect] : Zipper(rects, inputs)) {
                if (inputRect.inputTextTitle == input.inputTextTitle) {
                    for (auto key : events.inputs) {
                        if (key >= Graphic::Keys::A && key <= Graphic::Keys::Z) {
                            key = static_cast<Graphic::Keys>(key + 65);
                            if (inputText.text.text.size() < NAME_SIZE) {
                                inputText.text.text += static_cast<char>(key);
                            }
                        } else if (key >= Graphic::Keys::Num0 && key <= Graphic::Keys::Num9) {
                            key = static_cast<Graphic::Keys>(key + 22);
                            if (inputText.text.text.size() < NAME_SIZE) {
                                inputText.text.text += static_cast<char>(key);
                            }
                        } else if (key == Graphic::Keys::Backspace) {
                            if (!inputText.text.text.empty()) {
                                inputText.text.text.pop_back();
                            }
                        }
                    }
            }
            renderer.drawText(inputText.text.text, pos.x, pos.y + 30, inputText.text.fontSize, colorText.color.r, colorText.color.g, colorText.color.b, colorText.color.a);
            }
        }
    }

    inline void drawRectangles(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &rects = r.get_components<Components::Rect>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[pos, rect] : Zipper(positions, rects)) {
            renderer.drawRectangle(pos.x, pos.y, rect.width, rect.height, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
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
            if (!text.isDrawable)
                continue;
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

    inline void drawEntities(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &drawables = r.get_components<Components::Drawable>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[drawable, position] : Zipper(drawables, positions)) {
            //if (!drawable.can_draw)
            //    continue;
            renderer.drawTexture(drawable.textureID, position.x, position.y,
                                 drawable.width, drawable.height, drawable.text_x, drawable.text_y, drawable.text_width, drawable.text_height);
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
}
