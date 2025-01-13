/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include "Client.hpp"
#include "INetworkClient.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"
#include "Zipper.hh"
#include "Components.hpp"
#include "Components.hh"
#include "Main.hpp"
#include "Gameplay.hpp"

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
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[text, colorText] : Zipper(texts, colorsTexts)) {
            renderer.drawText(text.text, text.x, text.y, text.fontSize, colorText.color.r, colorText.color.g, colorText.color.b, colorText.color.a);
        }
    }

    inline void drawOverText(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &colorsTexts = r.get_components<Components::ColorText>();
        auto &colorsOverText = r.get_components<Components::ColorOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[text, colorText, colorOverText] : Zipper(texts, colorsTexts, colorsOverText)) {
            if (colorOverText.isOver) {
                colorText.color = colorOverText.newColor;
            } else {
                colorText.color = colorOverText.defaultColor;
            }
            renderer.drawText(text.text, text.x, text.y, text.fontSize, colorText.color.r, colorText.color.g, colorText.color.b, colorText.color.a);
        }
    }

    inline void handleMouse(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &clickables = r.get_components<Components::ClickableText>();
        auto &colorsOverText = r.get_components<Components::ColorOverText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        Graphic::event_t events = renderer.getEvents();
        bool leftClicked = std::find(events.inputs.begin(), events.inputs.end(), Graphic::Keys::LeftClick) != events.inputs.end();
        std::pair<int, int> mousePos = events.mouse_pos;
        std::function<void(Registry &r)> secureCallback;

        for (auto &&[clickable, text, colorsOverText] : Zipper(clickables, texts, colorsOverText)) {
            if (mousePos.first >= text.x && mousePos.first <= text.x + (int)text.text.size() * text.fontSize &&
                mousePos.second >= text.y && mousePos.second <= text.y + text.fontSize) {
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

    inline void DrawEntities(Registry &r) {
        auto &entities = r.get_components<Components::Entity>();
        auto &drawables = r.get_components<Components::Drawable>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[entity, drawable] : Zipper(entities, drawables)) {
            renderer.drawTexture(drawable.textureID, entity.x, entity.y, entity.width, entity.height);
        }
    }
}
