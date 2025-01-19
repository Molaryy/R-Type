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

        registry_.add_component(e, Components::Drawable(BACKGROUND, WIDTH, HEIGHT, 0, 0, WIDTH / (HEIGHT / 189.0), 189, [](Components::Drawable &drawable) {
            drawable.text_x += 3;
            if (drawable.text_x >= 1221)
                drawable.text_x = 0;
        }));
        registry_.add_component(e, Position(0, 0));
    });
    packet_handler_.setPacketCallback(Protocol::POSITION_VELOCITY, [this](const Network::Packet &packet) {
        auto [network_id, position, velocity] = packet.getPayload<Protocol::EntityPositionVelocityPacket>();
        SparseArray<Components::ServerId> server_ids = registry_.get_components<Components::ServerId>();

        const auto it = std::ranges::find_if(server_ids, [network_id](const std::optional<Components::ServerId> &server_id) {
            return server_id.has_value() && server_id->id == network_id;
        });
        if (it == server_ids.end()) {
            std::cerr << "POSITION_VELOCITY: Failed to find server id: " << network_id << std::endl;
            return;
        }
        const entity_t entity_id = std::ranges::distance(server_ids.begin(), it);

        std::optional<Velocity> &vel = registry_.get_components<Velocity>()[entity_id];
        if (vel.has_value()) {
            vel->x = velocity.x;
            vel->y = velocity.y;
        }
        std::optional<Position> &pos = registry_.get_components<Position>()[entity_id];
        if (pos.has_value()) {
            pos->x = position.x;
            pos->y = position.y;
        }
    });
    packet_handler_.setPacketCallback(Protocol::SPAWN, [this](const Network::Packet &packet) {
        auto [entity_id, type, position, size, velocity, health] = packet.getPayload<Protocol::SpawnEntityPacket>();
        const entity_t e = registry_.spawn_entity();

        switch (type) {
            case Protocol::EntityType::PLAYER:
                registry_.add_component(e, Components::Drawable(
                                            SPACESHIPS,
                                            size.x, size.y,
                                            0, static_cast<float>(17 * std::ranges::count_if(
                                                                      registry_.get_components<Components::ComponentEntityType>(),
                                                                      [](const std::optional<Components::ComponentEntityType> &ent_type) {
                                                                          return ent_type.has_value() && ent_type->type == Protocol::EntityType::PLAYER;
                                                                      })),
                                            33, 17,
                                            [increasing = true, frame = 0](Components::Drawable &drawable) mutable {
                                                if (frame++ < 3)
                                                    return;
                                                frame = 0;
                                                if (drawable.text_x > drawable.text_width * 3)
                                                    increasing = false;
                                                if (drawable.text_x <= 0)
                                                    increasing = true;
                                                drawable.text_x += increasing ? drawable.text_width : -drawable.text_width;
                                            }));
                registry_.add_component(e, Life(health, health));
                break;
            case Protocol::EntityType::PLAYER_BULLET:
                registry_.add_component(e, Components::Drawable(PLAYER_BULLET, size.x, size.y, 0, 0, 82, 16,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width)
                                                                        drawable.text_x = 0;
                                                                }));
                break;
            case Protocol::EntityType::ENEMY_BULLET:
                registry_.add_component(e, Components::Drawable(ENEMY_BULLET, size.x, size.y, 0, 0, 48, 48,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width * 3)
                                                                        drawable.text_x = 0;
                                                                }));
                break;
            case Protocol::EntityType::ENEMY_FLY:
                registry_.add_component(e, Components::Drawable(FLY_ENEMY, size.x, size.y, 0, 0, 33, 36, [frame = 0](Components::Drawable &drawable) mutable {
                    if (frame++ < 3)
                        return;
                    frame = 0;
                    drawable.text_x += drawable.text_width;
                    if (drawable.text_x > drawable.text_width * 8)
                        drawable.text_x = 0;
                }));
                registry_.add_component(e, Life(health, health));
                break;
            case Protocol::EntityType::ENEMY_TURRET:
                registry_.add_component(e, Components::Drawable(SHOOTER_ENEMY, size.x, size.y, 0, 0, 65, 65,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width * 5)
                                                                        drawable.text_x = 0;
                                                                }));
                registry_.add_component(e, Life(health, health));
                break;
            case Protocol::BONUS_HEALTH:
                registry_.add_component(e, Components::Drawable(BONUS_HEALTH, size.x, size.y, 0, 0, 24, 24,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width * 4)
                                                                        drawable.text_x = 0;
                                                                }));
                break;
            case Protocol::BONUS_DAMAGE:
                registry_.add_component(e, Components::Drawable(BONUS_FORCE, size.x, size.y, 0, 0, 32, 32, [frame = 0](Components::Drawable &drawable) mutable {
                    if (frame++ < 3)
                        return;
                    frame = 0;
                    drawable.text_x += drawable.text_width;
                    if (drawable.text_x > drawable.text_width * 2)
                        drawable.text_x = 0;
                }));
                break;
            case Protocol::BONUS_TRIPLE_SHOT:
                registry_.add_component(e, Components::Drawable(BONUS_TRIPLE_SHOT, size.x, size.y, 0, 0, 30, 22,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width * 5)
                                                                        drawable.text_x = 0;
                                                                }));
                break;
            case Protocol::ENEMY_TANK:
                registry_.add_component(e, Components::Drawable(TANK_ENEMY, size.x, size.y, 0, 0, 65, 50,
                                                                [frame = 0](Components::Drawable &drawable) mutable {
                                                                    if (frame++ < 3)
                                                                        return;
                                                                    frame = 0;
                                                                    drawable.text_x += drawable.text_width;
                                                                    if (drawable.text_x > drawable.text_width * 2)
                                                                        drawable.text_x = 0;
                                                                }));
                break;
default:
                std::cerr << "Unknown entity type: " << type << std::endl;
                break;
        }
        registry_.add_component(e, Position(position.x, position.y));
        registry_.add_component(e, Components::ServerId(entity_id));
        registry_.add_component(e, Components::ComponentEntityType(type));
        registry_.add_component(e, Velocity(velocity.x, velocity.y));
    });
    packet_handler_.setPacketCallback(Protocol::HIT, [this](const Network::Packet &packet) {
        auto [network_id, health] = packet.getPayload<Protocol::HitPacket>();
        SparseArray<Components::ServerId> server_ids = registry_.get_components<Components::ServerId>();

        const auto it = std::ranges::find_if(server_ids, [network_id](const std::optional<Components::ServerId> &server_id) {
            return server_id.has_value() && server_id->id == network_id;
        });
        if (it == server_ids.end()) {
            std::cerr << "HIT: Failed to find server id: " << network_id << std::endl;
            return;
        }
        const entity_t entity_id = std::ranges::distance(server_ids.begin(), it);

        std::optional<Life> &life = registry_.get_components<Life>()[entity_id];
        if (life.has_value())
            life->current = health;
    });
    packet_handler_.setPacketCallback(Protocol::KILL, [this](const Network::Packet &packet) {
        auto [network_id, natural] = packet.getPayload<Protocol::DeadPacket>();
        SparseArray<Components::ServerId> server_ids = registry_.get_components<Components::ServerId>();
        const auto it = std::ranges::find_if(server_ids, [network_id](const std::optional<Components::ServerId> &server_id) {
            return server_id.has_value() && server_id->id == network_id;
        });
        if (it == server_ids.end()) {
            std::cerr << "KILL: Failed to find server id: " << network_id << std::endl;
            return;
        }
        const entity_t entity_id = std::ranges::distance(server_ids.begin(), it);

        if (!natural)
            return registry_.kill_entity(entity_id);
        const std::optional<Position> &pos = registry_.get_components<Position>()[entity_id];
        std::optional<Components::Drawable> &draw = registry_.get_components<Components::Drawable>()[entity_id];

        if (pos.has_value() && draw.has_value()) {
            const entity_t e = registry_.spawn_entity();

            registry_.add_component(e, Position(pos->x, pos->y));
            registry_.add_component(e, Components::Drawable(EXPLOSION_1, (std::max)(draw->width, draw->height), (std::max)(draw->width, draw->height), 0, 0, 65,
                                                            66,
                                                            [frame = 0](Components::Drawable &drawable) mutable {
                                                                if (frame++ < 3)
                                                                    return;
                                                                frame = 0;
                                                                drawable.text_x += drawable.text_width;
                                                                if (drawable.text_x > drawable.text_width * 2) {
                                                                    drawable.text_width = 0;
                                                                    drawable.text_height = 0;
                                                                }
                                                            }));
        }
        const std::optional<Components::ComponentEntityType> &type = registry_.get_components<Components::ComponentEntityType>()[entity_id];
        if (type.has_value() && type->type == Protocol::PLAYER) {
            draw->can_draw = false;
            return;
        }
        registry_.kill_entity(entity_id);
    });
    packet_handler_.setPacketCallback(Protocol::SERVER_SHUTDOWN, [](Network::Packet &) {
        std::cout << "SERVER_SHUTDOWN received\n";
    });
    packet_handler_.setPacketCallback(Protocol::END_GAME, [this](const Network::Packet &packet) {
        auto [score, new_id] = packet.getPayload<Protocol::EndGamePacket>();
        my_server_id = new_id;

        lobbyPage(registry_, lobby_id);

        entity_t e = registry_.spawn_entity();
        registry_.add_component(e, Components::RenderText(std::string("Score: ") + std::to_string(score), 30, true));
        registry_.add_component(e, Position(330, 130));
        registry_.add_component(e, Components::ColorText{COLOR_WHITE});

        e = registry_.spawn_entity();
        registry_.add_component(e, Components::RenderText(std::string("Game Over"), 40, true));
        registry_.add_component(e, Position(300, 80));
        registry_.add_component(e, Components::ColorText{COLOR_WHITE});
    });
}

void Client::setupSystems_() {
    registry_.add_system(Systems::networkReceiver);
    registry_.add_system(Systems::position_velocity);
    registry_.add_system(Systems::drawAllTexts);
    registry_.add_system(Systems::changeColorOverText);
    registry_.add_system(Systems::handleMouse);
    registry_.add_system(Systems::drawEntities);
    registry_.add_system(Systems::spriteSheetHandler);
    registry_.add_system(Systems::handleInputs);
    if (debug_)
        registry_.add_system(Systems::log);
    registry_.add_system([](Registry &r) {
        Systems::limit_framerate(r, SERVER_TPS);
    });
}

std::unique_ptr<Client> Client::instance_ = nullptr;

void Client::run() {
    setupSystems_();

    renderer_->initWindow(WIDTH, HEIGHT, "rtype");

    createMenuScene(registry_);

    for (const std::string &path : textures_paths)
        renderer_->loadTexture(path);

    while (!renderer_->windowShouldClose()) {
        renderer_->beginDrawing();
        renderer_->clearBackground(0, 0, 0, 0);

        registry_.run_systems();
        renderer_->endDrawing();
    }
    renderer_->closeWindow();
}
