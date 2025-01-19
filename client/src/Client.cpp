/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Client
*/

#include "Client.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "Components.hh"
#include "Packet.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"
#include "RTypeProtocol.hpp"
#include "Scenes.hpp"
#include "Systems.hh"
#include "Systems.hpp"
#include "Scenes.hpp"

Client::~Client() = default;

Client::Client(const std::string &ip, const std::size_t port, const bool debug)
    : graph_loader_("./", "raylib_graphics"),
      network_loader_("./", "asio_client"),
      debug_(debug) {
    std::cout << ip << port << std::endl;
    try {
        auto *create_graph_lib = graph_loader_.get_function<Graphic::IRenderer *()>("create_instance");
        auto *create_network_lib = network_loader_.get_function<Network::INetworkClient *()>("create_instance");

        renderer_.reset(create_graph_lib());
        network_lib_.reset(create_network_lib());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load dyn lib : " + std::string(e.what()));
    }
    setupPacketHandler_();

    connectToServer_(ip, port);
}

Client &Client::createInstance(const std::string &ip, const std::size_t port, const bool debug) {
    instance_.reset(new Client(ip, port, debug));
    return *instance_;
}

Client &Client::getInstance() {
    return *instance_;
}

Graphic::IRenderer &Client::getRenderer() const {
    return *renderer_;
}

Network::INetworkClient &Client::getNetworkLib() const {
    return *network_lib_;
}

Network::PacketHandler &Client::getPacketHandler() {
    return packet_handler_;
}

Registry &Client::getRegistry() {
    return registry_;
}

bool Client::connectToServer_(const std::string &ip, const std::size_t port) {
    network_lib_->connect(ip, static_cast<uint16_t>(port));
    bool success = false;

    packet_handler_.setPacketCallback(
        Protocol::ACCEPT_CONNECTION,
        [&success, this]([[maybe_unused]] const Network::Packet &packet) {
            std::cout << "Connection established with server" << success << std::endl;
            success = true;
        }
    );

    Network::Packet packet(Protocol::EmptyPacket(), Protocol::CONNECT);
    network_lib_->send(packet.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!success) {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't connect to the server: time out");
        std::vector<uint8_t> oldest_packet = network_lib_->getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler_(deserialized_packet);
    }
    return true;
}

void Client::setupPacketHandler_() {
    packet_handler_.setPacketCallback(Protocol::START_GAME, [this](Network::Packet &) {
        registry_.clear_entities();

        const entity_t e = registry_.spawn_entity();

        registry_.add_component(e, Components::Drawable(BACKGROUND_ID, WIDTH, HEIGHT, 0, 0, WIDTH / (HEIGHT / 189.0), 189));
        registry_.add_component(e, Position(0, 0));
    });
    packet_handler_.setPacketCallback(Protocol::POSITION_VELOCITY, [this](const Network::Packet &packet) {
        auto [network_id, position, velocity] = packet.getPayload<Protocol::EntityPositionVelocityPacket>();
        SparseArray<Components::ServerId> server_ids = registry_.get_components<Components::ServerId>();

        const auto it = std::ranges::find_if(server_ids, [network_id](const std::optional<Components::ServerId> &server_id) {
            return server_id.has_value() && server_id->id == network_id;
        });
        if (it == server_ids.end()) {
            std::cerr << "Failed to find server id: " << network_id << std::endl;
            return;
        }
        const entity_t entity_id = std::ranges::distance(server_ids.begin(), it);

        std::optional<Velocity> &vel = registry_.get_components<Velocity>()[entity_id];
        if (vel.has_value()) {
            vel.value().x = velocity.x;
            vel.value().y = velocity.y;
        }
        std::optional<Position> &pos = registry_.get_components<Position>()[entity_id];
        if (pos.has_value()) {
            pos.value().x = position.x;
            pos.value().y = position.y;
        }
    });
    packet_handler_.setPacketCallback(Protocol::SPAWN, [this](const Network::Packet &packet) {
        auto [entity_id, type, position, velocity] = packet.getPayload<Protocol::SpawnEntityPacket>();
        const entity_t e = registry_.spawn_entity();

        registry_.add_component(e, Position(position.x, position.y));
        registry_.add_component(e, Components::ServerId(entity_id));
        registry_.add_component(e, Components::ComponentEntityType(type));
        registry_.add_component(e, Velocity(velocity.x, velocity.y));
        switch (type) {
        case Protocol::EntityType::PLAYER:
            registry_.add_component(e, Components::Drawable(PLAYER_ID, PLAYER_SIZE, PLAYER_SIZE, 0, 0, PLAYER_SIZE, PLAYER_SIZE));
            break;
        case Protocol::EntityType::PLAYER_BULLET:
            registry_.add_component(e, Components::Drawable(BULLET_ID, PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, 0, 0, PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE));
            break;
        case Protocol::EntityType::ENEMY_FLY:
            registry_.add_component(e, Components::Drawable(ENNEMY_ID, FLY_SIZE, FLY_SIZE, 0, 0, FLY_SIZE, FLY_SIZE));
            break;
        default:
            std::cerr << "Unknown entity type: " << type << std::endl;
            break;
        }
    });
    packet_handler_.setPacketCallback(Protocol::HIT, [](Network::Packet &) {
        std::cout << "HIT received\n";
    });
    packet_handler_.setPacketCallback(Protocol::KILL, [](Network::Packet &) {
        std::cout << "KILL received\n";
    });
    packet_handler_.setPacketCallback(Protocol::SERVER_SHUTDOWN, [](Network::Packet &) {
        std::cout << "SERVER_SHUTDOWN received\n";
    });
    packet_handler_.setPacketCallback(Protocol::END_GAME, [](Network::Packet &) {
        std::cout << "END_GAME received\n";
    });
}

void Client::setupSystems_() {
    registry_.add_system(Systems::networkReceiver);
    registry_.add_system(Systems::position_velocity);
    registry_.add_system(Systems::drawAllTexts);
    registry_.add_system(Systems::changeColorOverText);
    registry_.add_system(Systems::handleMouse);
    registry_.add_system(Systems::drawEntities);
    registry_.add_system(Systems::handleInputs);
    registry_.add_system(Systems::drawRectangles);
    registry_.add_system(Systems::handleInputBox);
    registry_.add_system(Systems::handleClickable);
    registry_.add_system(Systems::handleClickableSoundText);
    if (debug_)
        registry_.add_system(Systems::log);
    registry_.add_system([](Registry &r) {
        Systems::limit_framerate(r, 30);
    });
}

std::unique_ptr<Client> Client::instance_ = nullptr;

void Client::run() {
    setupSystems_();

    renderer_->initWindow(WIDTH, HEIGHT, "rtype");

    if (getLocalUsername().empty()) {
        createSignForm(registry_);
    } else {
        createMenuScene(registry_);
    }

    renderer_->loadTexture("assets/spaceship.gif");
    renderer_->loadTexture("assets/enemies.gif");
    renderer_->loadTexture("assets/missiles.gif");
    renderer_->loadTexture("assets/maps/space.png");
    for (const auto &voice_button : VOICE_BUTTONS_TITLES_FILEPATHS) {
        std::cout << voice_button << std::endl;
        renderer_->loadSound(voice_button);
    }


    while (!renderer_->windowShouldClose()) {
        renderer_->beginDrawing();
        renderer_->clearBackground(0, 0, 0, 0);

        registry_.run_systems();
        renderer_->endDrawing();
    }
    renderer_->closeWindow();
}
