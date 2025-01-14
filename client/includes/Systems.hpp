/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include "Client.hpp"
#include "Components.hpp"
#include "INetworkClient.hpp"
#include "Main.hpp"
#include "PacketHandler.hpp"
#include "Gameplay.hpp"
#include "RTypeProtocol.hpp"

namespace Systems
{
    inline void networkReceiver([[maybe_unused]] Registry &r)
    {
        auto &core = Client::getInstance();
        auto &network = core.getNetworkLib();
        auto &packet_handler = core.getPacketHandler();

        for (std::vector<uint8_t> oldest_packet = network.getOldestPacket(); !oldest_packet.empty(); oldest_packet = network.getOldestPacket())
        {
            Network::Packet deserialized_packet(oldest_packet);
            packet_handler(deserialized_packet);
        }
    }

    inline void drawAllTexts(Registry &r)
    {
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[text, colorText] : Zipper(texts, colorsTexts))
        {
            renderer.drawText(text.text, text.x, text.y, text.fontSize, colorText.color.r, colorText.color.g, colorText.color.b, colorText.color.a);
        }
    }

    inline void drawOverText(Registry &r)
    {
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        auto &colorsOverText = r.get_components<Components::ColorOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[text, colorText, colorOverText] : Zipper(texts, colorsTexts, colorsOverText))
        {
            if (colorOverText.isOver)
            {
                colorText.color = colorOverText.newColor;
            }
            else
            {
                colorText.color = colorOverText.defaultColor;
            }
            renderer.drawText(text.text, text.x, text.y, text.fontSize, colorText.color.r, colorText.color.g, colorText.color.b, colorText.color.a);
        }
    }

    inline void handleMouse(Registry &r)
    {
        auto &texts = r.get_components<Components::RenderText>();
        auto &clickables = r.get_components<Components::ClickableText>();
        auto &colorsOverTexts = r.get_components<Components::ColorOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        const bool leftClicked = std::ranges::find(events.inputs, Graphic::Keys::LeftClick) != events.inputs.end();
        auto [mouse_x, mouse_y] = events.mouse_pos;
        std::function<void(Registry &r)> secureCallback;

        for (auto &&[clickable, text, colorsOverText] : Zipper(clickables, texts, colorsOverTexts)) {
            if (mouse_x >= text.x && mouse_x <= text.x + static_cast<int>(text.text.size()) * text.fontSize &&
                mouse_y >= text.y && mouse_y <= text.y + text.fontSize) {
                if (leftClicked) {
                    secureCallback = clickable.callback;
                    break;
                }
                colorsOverText.isOver = true;
            }
            else
            {
                colorsOverText.isOver = false;
            }
        }
        if (secureCallback)
        {
            secureCallback(r);
        }
    }

    inline void drawEntities(Registry &r)
    {
        auto &entities = r.get_components<Components::Entity>();
        auto &drawables = r.get_components<Components::Drawable>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[entity, drawable] : Zipper(entities, drawables)) {
            renderer.drawTexture(drawable.textureID, entity.x, entity.y, static_cast<int>(entity.width), static_cast<int>(entity.height), 0);
        }
    }

    inline void moveEntities(Registry &r)
    {
        auto &entities = r.get_components<Components::Entity>();
        auto &movables = r.get_components<Components::Movable>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        Protocol::InputsKeysPacket inputs;

        if (std::ranges::find(events.inputs, Graphic::Keys::UpArrow) != events.inputs.end())
        {
            inputs.input_keys[Protocol::InputKey::MOVE_UP] = true;
        }
        if (std::ranges::find(events.inputs, Graphic::Keys::DownArrow) != events.inputs.end())
        {
            inputs.input_keys[Protocol::InputKey::MOVE_DOWN] = true;
        }
        if (std::ranges::find(events.inputs, Graphic::Keys::LeftArrow) != events.inputs.end())
        {
            inputs.input_keys[Protocol::InputKey::MOVE_LEFT] = true;
        }
        if (std::ranges::find(events.inputs, Graphic::Keys::RightArrow) != events.inputs.end())
        {
            inputs.input_keys[Protocol::InputKey::MOVE_RIGHT] = true;
        }
        if (std::ranges::find(events.inputs, Graphic::Keys::Space) != events.inputs.end())
        {
            inputs.input_keys[Protocol::InputKey::SHOOT] = true;
        }

        Network::Packet packet(inputs, Protocol::INPUT_KEYS);
        Client::getInstance().getNetworkLib().send(packet.serialize());
    }
}
