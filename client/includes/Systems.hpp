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

    inline void drawRectangles(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &rects = r.get_components<Components::Rect>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[pos, rect] : Zipper(positions, rects)) {
            renderer.drawRectangle(pos.x, pos.y, rect.width, rect.height, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
        }
    }

    inline void changeColorOverText(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        auto &colorsOverText = r.get_components<Components::ColorOverText>();
        auto &mouseOverTexts = r.get_components<Components::MouseOverText>();
        auto &soundTexts = r.get_components<Components::MouseOverTextSound>();

        for (auto &&[text, colorText, colorOverText, mouseOverText, soundText] : Zipper(texts, colorsTexts, colorsOverText, mouseOverTexts, soundTexts)) {
            if (!text.isDrawable || (text.text == "Accessibility" && isAccessibilityOn))
                continue;
            if (mouseOverText.isOver) {
                colorText.color = colorOverText.newColor;
            } else {
                colorText.color = colorOverText.defaultColor;
            }
        }
    }

    inline void handleMouseOverSoundText(Registry &r) {
        auto &clickableTexts = r.get_components<Components::MouseOverTextSound>();
        auto &mouseOverTexts = r.get_components<Components::MouseOverText>();
        std::function<void(int soundID)> secureCallback;
        int soundID = 0;

        for (auto &&[clickable, mouseOverText] : Zipper(clickableTexts, mouseOverTexts)) {
            if (mouseOverText.isOver && isAccessibilityOn) {
                secureCallback = clickable.callback;
                soundID = clickable.soundID;
                break;
            }
        }

        if (secureCallback) {
            secureCallback(soundID);
        }
    }

    inline void handleClickable(Registry &r) {
        auto &clickableTexts = r.get_components<Components::ClickableText>();
        auto &mouseOverTexts = r.get_components<Components::MouseOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        std::function<void(Registry &r)> secureCallback;
        const bool leftClicked = std::ranges::find(events.inputs, Graphic::Keys::LeftClick) != events.inputs.end();

        for (auto &&[clickable, mouseOverText] : Zipper(clickableTexts, mouseOverTexts)) {
            if (mouseOverText.isOver && leftClicked) {
                secureCallback = clickable.callback;
                break;
            }
        }

        if (secureCallback) {
            secureCallback(r);
        }
    }

    inline void handleMouse(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &positions = r.get_components<Position>();
        auto &mouseOverTexts = r.get_components<Components::MouseOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        auto [mouse_x, mouse_y] = events.mouse_pos;
        std::function<void(Registry &r)> secureCallback;

        for (auto &&[pos, mouseOverText, text] : Zipper(positions, mouseOverTexts, texts)) {
            if (!text.isDrawable)
                continue;
            if (mouse_x >= static_cast<int>(pos.x) && mouse_x <= pos.x + static_cast<int>(text.text.size()) * text.fontSize &&
                mouse_y >= static_cast<int>(pos.y) && mouse_y <= static_cast<int>(pos.y) + text.fontSize) {
                mouseOverText.isOver = true;
            } else {
                mouseOverText.isOver = false;
            }
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
            float life_y = position.y - 10.f;
            if (life_y < 0)
                life_y = position.y + drawable.height / 2;
            renderer.drawRectangle(position.x, life_y, drawable.width, 5.f, 255, 0, 0, 255);
            renderer.drawRectangle(position.x, life_y, drawable.width * life_ratio, 5.f, 0, 128, 0, 255);
        }
    }

    inline void handleInputBox(Registry &r)
    {
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        auto &inputTexts = r.get_components<Components::InputText>();
        auto &positions = r.get_components<Position>();
        auto &inputs = r.get_components<Components::Input>();
        auto &rects = r.get_components<Components::Rect>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        static std::vector<Graphic::Keys> last_inputs;
        bool canUpdate = true;


        if (events.inputs.size() != last_inputs.size())
        {
            last_inputs = events.inputs;
            canUpdate = false;
        }

        for (std::size_t i = 0; i < events.inputs.size(); ++i)
        {
            if (events.inputs[i] != last_inputs[i])
            {
                last_inputs = events.inputs;
                canUpdate = false;
                break;
            }
        }

        for (auto &&[inputText, pos, input, colorText] : Zipper(inputTexts,  positions, inputs, colorsTexts)) {
            for (auto &&[rect, inputRect] : Zipper(rects, inputs)) {
                if (inputRect.inputTextTitle == input.inputTextTitle && canUpdate) {
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

    inline void handleInputs([[maybe_unused]] Registry &r)
    {
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
                for (uint8_t j = i; j < Protocol::NB_INPUTS_KEYS; ++j) {
                    last_inputs.input_keys[j] = inputs.input_keys[j];
                }
                Network::Packet packet(inputs, Protocol::INPUT_KEYS);
                Client::getInstance().getNetworkLib().send(packet.serialize());
                return;
            }
        }
    } 
}
