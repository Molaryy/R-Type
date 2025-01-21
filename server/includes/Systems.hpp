/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Systems.hpp
*/

#pragma once

#include <cmath>
#include <fstream>
#include <stdexcept>

#include "entities/Boss.hpp"
#include "entities/BossHeart.hpp"
#include "entities/Wall.hpp"
#include "entities/BonusHealth.hpp"
#include "entities/BonusTripleShot.hpp"
#include "entities/BonusForce.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Registry.hh"
#include "Zipper.hh"
#include "entities/EnemyTank.hpp"
#include "entities/EnemyTurret.hpp"
#include "entities/EnemyFly.hpp"
#include "entities/Player.hpp"
#include "entities/PlayerBullet.hpp"

namespace Systems {
    inline void handleClientInputs(Registry &r) {
        Network::INetworkServer &network = Server::getInstance().getNetwork();

        SparseArray<ClientInputs> &inputs = r.get_components<ClientInputs>();
        SparseArray<Position> &positions = r.get_components<Position>();
        SparseArray<Velocity> &velocities = r.get_components<Velocity>();
        SparseArray<Delay> &clocks = r.get_components<Delay>();
        const SparseArray<Bonus> &bonuses = r.get_components<Bonus>();
        const SparseArray<Life> &lifes = r.get_components<Life>();
        const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[entity, input, velocity, position, clock, life, bonus] : IndexedZipper(inputs, velocities, positions, clocks, lifes, bonuses)) {
            if (!life.is_alive())
                continue;
            velocity.x = 0;
            velocity.y = 0;
            ++clock.last;
            if (!input.input_keys.empty()) {
                if (std::ranges::find(input.input_keys, Protocol::MOVE_UP) != input.input_keys.end())
                    velocity.y -= PLAYER_SPEED;
                else if (std::ranges::find(input.input_keys, Protocol::MOVE_DOWN) != input.input_keys.end())
                    velocity.y += PLAYER_SPEED;
                if (std::ranges::find(input.input_keys, Protocol::MOVE_LEFT) != input.input_keys.end())
                    velocity.x -= PLAYER_SPEED;
                else if (std::ranges::find(input.input_keys, Protocol::MOVE_RIGHT) != input.input_keys.end())
                    velocity.x += PLAYER_SPEED;
                if (std::ranges::find(input.input_keys, Protocol::SHOOT) != input.input_keys.end() && clock.delay <= clock.last) {
                    clock.last = 0;
                    callbacks.emplace([&] {
                        PlayerBullet::create(r, Position(position.x + PLAYER_SIZE_X, position.y + PLAYER_SIZE_Y / 2.0f), bonus);
                    });
                }
            }
            for (auto &&[network_id] : Zipper(network_ids)) {
                Protocol::EntityPositionVelocityPacket pos_vel(
                    entity,
                    {position.x, position.y},
                    {velocity.x, velocity.y});
                Network::Packet packet(
                    pos_vel,
                    Protocol::POSITION_VELOCITY
                );
                network.send(network_id.id, packet.serialize());
            }

            std::optional<Position> &bon_pos = positions[bonus.id];
            std::optional<Velocity> &bon_vel = velocities[bonus.id];
            if (bonus.type == Bonus::None || !bon_pos || !bon_vel)
                continue;
            bon_pos->x = position.x + PLAYER_SIZE_X;
            bon_pos->y = position.y;
            bon_vel->x = velocity.x;
            bon_vel->y = velocity.y;

            for (auto &&[network_id] : Zipper(network_ids)) {
                Protocol::EntityPositionVelocityPacket pos_vel(
                    bonus.id,
                    {bon_pos->x, bon_pos->y},
                    {bon_vel->x, bon_vel->y});
                Network::Packet packet(
                    pos_vel,
                    Protocol::POSITION_VELOCITY
                );
                network.send(network_id.id, packet.serialize());
            }
        }
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    inline void levelEndlessHandler(Registry &r, std::size_t &score) {
        if (score == 0) {
            const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
            SparseArray<Position> &positions = r.get_components<Position>();
            const SparseArray<Velocity> &velocities = r.get_components<Velocity>();
            float i = 100;
            Network::INetworkServer &network = Server::getInstance().getNetwork();

            for (auto &&[entity, _, position, velocity] : IndexedZipper(network_ids, positions, velocities)) {
                position.x = 100;
                position.y = i;
                i += 50;
                Protocol::EntityPositionVelocityPacket pos_vel(entity, {position.x, position.y}, {velocity.x, velocity.y});
                Network::Packet packet(pos_vel, Protocol::POSITION_VELOCITY);
                for (auto &&[network_id] : Zipper(network_ids))
                    network.send(network_id.id, packet.serialize());
            }
        }

        score += 1;

        if (score % 60 != 0)
            return;
        std::vector<std::pair<std::function<entity_t(Registry &)>, double>> spawn_rates{
            {EnemyFly::create, 2},
            {EnemyTurret::create, 1},
            {EnemyTank::create, 0.3}
        };
        double difficulty = static_cast<double>(score) / 500.0 * (0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8)) + 0.5;
        if (difficulty > 1)
            difficulty = 1 + (difficulty - 1) * 0.5;
        if (difficulty > 3)
            difficulty = 3 + (difficulty - 3) * 0.5;
        if (difficulty > 4)
            difficulty = 4;
        for (auto &&[spawn, rate] : spawn_rates) {
            for ([[maybe_unused]] std::size_t _ : std::ranges::iota_view{static_cast<std::size_t>(0), static_cast<std::size_t>(difficulty * rate)})
                spawn(r);
        }
    }

    class LevelCampaignHandlerSystem {
    public:
        explicit LevelCampaignHandlerSystem(Registry &reg, std::size_t &initialPos)
            : score_(initialPos) {
            try {
                loadFile_();
                success = true;
            } catch (const std::runtime_error &e) {
                std::cerr << e.what() << ", starting endless mode" << std::endl;
                reg.add_system([this, &initialPos](Registry &r) {
                    levelEndlessHandler(r, initialPos);
                });
                success = false;
            }
        }


        void operator()(Registry &r) {
            if (!success)
                return;
            score_ += 1;
            if (pos_in_level_ < WIDTH) {
                do {
                    pos_in_level_ -= CAMERA_SPEED;
                    loadLine_(r);
                } while (pos_in_level_ < WIDTH);
                return;
            }
            if (next_to_draw_ >= levels_[atm_level_].size()) {
                if (frame_at_end_ <= 90) {
                    frame_at_end_++;
                }
                next_to_draw_ = 0;
                pos_in_level_ = 0;
                atm_level_ += 1;
                cleanLevel_(r);
                if (atm_level_ >= levels_.size()) {
                    const SparseArray<ComponentEntityType> &types = r.get_components<ComponentEntityType>();
                    SparseArray<Life> &lifes = r.get_components<Life>();
                    for (auto &&[type, life] : Zipper(types, lifes))
                        if (type.type == Protocol::PLAYER)
                            life.current = -1;
                    return;
                }
                return;
            }
            pos_in_level_ -= CAMERA_SPEED;
            loadLine_(r);
        }

    private:
        void loadLine_(Registry &r) {
            if (pos_in_level_ / TILE_SIZE <= next_to_draw_)
                return;
            std::vector<LevelElementType> &line = levels_[atm_level_][next_to_draw_];

            const auto x = static_cast<float>(next_to_draw_ * TILE_SIZE > WIDTH ? WIDTH : next_to_draw_ * TILE_SIZE);
            float y = 0;
            for (LevelElementType &level_element : line) {
                map_lvl_elem_type_.at(level_element)(r, Position(x, y));
                y += TILE_SIZE;
            }
            next_to_draw_++;
        }

        static void cleanLevel_(Registry &r) {
            SparseArray<ComponentEntityType> types = r.get_components<ComponentEntityType>();
            Network::INetworkServer &network = Server::getInstance().getNetwork();

            for (auto &&[id, type] : IndexedZipper(types)) {
                if (type.type == Protocol::BONUS_DAMAGE || type.type == Protocol::BONUS_TRIPLE_SHOT || type.type == Protocol::PLAYER)
                    continue;
                Network::Packet packet(Protocol::DeadPacket(id, false), Protocol::KILL);
                const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
                for (auto &&[network_id] : Zipper(network_ids))
                    network.send(network_id.id, packet.serialize());
                r.kill_entity(id);
            }
        }

        void loadFile_() {
            std::ifstream ifs("assets/levels.txt");
            std::string line;

            if (!ifs.is_open())
                throw std::runtime_error("Failed to open levels file: levels.txt");

            std::getline(ifs, line);
            while (ifs) {
                if (line.empty() || line[0] == '#') {
                    std::getline(ifs, line);
                    continue;
                }
                std::size_t width;
                std::size_t height;
                std::stringstream ss(line);
                ss >> width >> std::skipws >> height;
                if (!width || !height)
                    throw std::runtime_error("Levels file: Missing width or height");
                levels_.emplace_back(width, std::vector(height, VOID_ELEM_TYPE));
                for (std::size_t y = 0; y < height; ++y) {
                    std::getline(ifs, line);
                    if (line.size() > width)
                        throw std::runtime_error("Levels file: Size width missmatch");
                    std::size_t i = 0;
                    for (char c : line)
                        levels_.back()[i++][y] = map_char_elem_type_.contains(c) ? map_char_elem_type_.at(c) : VOID_ELEM_TYPE;
                }
                std::getline(ifs, line);
            }
            ifs.close();
        }

        std::size_t score_;
        std::size_t pos_in_level_{};
        std::size_t next_to_draw_{};
        std::size_t atm_level_{};
        bool success = false;
        std::size_t frame_at_end_ = 0;

        enum LevelElementType {
            VOID_ELEM_TYPE,
            WALL,
            PLAYER,
            TURRET,
            TANK,
            BOSS,
            BOSS_HEART,
            BONUS_HEALTH,
            BONUS_DAMAGE,
            BONUS_TRIPLE_SHOT,
            FLY
        };

        std::vector<std::vector<std::vector<LevelElementType>>> levels_;

        const std::unordered_map<char, LevelElementType> map_char_elem_type_{
            {' ', VOID_ELEM_TYPE},
            {'X', WALL},
            {'P', PLAYER},
            {'T', TURRET},
            {'K', TANK},
            {'B', BOSS},
            {'H', BOSS_HEART},
            {'F', FLY},
            {'1', BONUS_HEALTH},
            {'2', BONUS_DAMAGE},
            {'3', BONUS_TRIPLE_SHOT},
        };

        const std::unordered_map<LevelElementType, std::function<void(Registry &, const Position &)>> map_lvl_elem_type_ = {
            {
                VOID_ELEM_TYPE,
                [](Registry &, const Position &) {
                }
            },
            {WALL, Wall::createFromPos},
            {
                PLAYER,
                [id = 0](Registry &r, const Position &pos) mutable {
                    const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
                    SparseArray<Position> &positions = r.get_components<Position>();
                    const SparseArray<Velocity> &velocities = r.get_components<Velocity>();
                    Network::INetworkServer &network = Server::getInstance().getNetwork();

                    int i = 0;
                    for (auto &&[entity, network_id, position, velocity] : IndexedZipper(network_ids, positions, velocities)) {
                        if (i++ != id)
                            continue;
                        ++id;
                        position.x = pos.x;
                        position.y = pos.y;
                        Protocol::EntityPositionVelocityPacket pos_vel(entity, {position.x, position.y}, {velocity.x, velocity.y});
                        Network::Packet packet(pos_vel, Protocol::POSITION_VELOCITY);
                        for (auto &&[network_id2] : Zipper(network_ids))
                            network.send(network_id2.id, packet.serialize());
                    }
                }
            },
            {TURRET, EnemyTurret::createFromPos},
            {TANK, EnemyTank::createFromPos},
            {BOSS, Boss::createFromPos,},
            {BOSS_HEART, BossHeart::createFromPos,},
            {FLY, EnemyFly::createFromPos},
            {BONUS_DAMAGE, BonusForce::create},
            {BONUS_HEALTH, BonusHealth::create},
            {BONUS_TRIPLE_SHOT, BonusTripleShot::create},
        };
    };

    inline void killNoHealthEntitys(Registry &r) {
        std::queue<std::tuple<entity_t, bool, bool>> entity_to_kill;
        for (auto &&[id, life, type] : IndexedZipper(r.get_components<Life>(), r.get_components<ComponentEntityType>()))
            if (life.current <= 0 && life.current != -2)
                entity_to_kill.emplace(id, type.type != Protocol::PLAYER, life.current == 0);

        Network::INetworkServer &network = Server::getInstance().getNetwork();
        while (!entity_to_kill.empty()) {
            std::tuple<unsigned long, bool, bool> &tuple = entity_to_kill.front();
            Network::Packet packet(Protocol::DeadPacket(std::get<0>(tuple), std::get<2>(tuple)), Protocol::KILL);
            const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
            for (auto &&[network_id] : Zipper(network_ids))
                network.send(network_id.id, packet.serialize());
            if (std::get<1>(tuple))
                r.kill_entity(std::get<0>(tuple));
            else
                r.get_entity_component<Life>(std::get<0>(tuple))->current = -2;
            entity_to_kill.pop();
        }
    }

    inline void runArtificialIntelligence(Registry &r) {
        SparseArray<ArtificialIntelligence> &ias = r.get_components<ArtificialIntelligence>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[id, ia] : IndexedZipper(ias))
            callbacks.emplace([&r, id, &ia] {
                ia.ia(r, id);
            });
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    inline void killOutOfScreenEntity(Registry &r) {
        const SparseArray<Position> &positions = r.get_components<Position>();
        const SparseArray<ComponentEntityType> &types = r.get_components<ComponentEntityType>();
        const SparseArray<Collision> &collisions = r.get_components<Collision>();
        SparseArray<Life> &lifes = r.get_components<Life>();

        for (auto &&[id, pos, type, col, life] : IndexedZipper(positions, types, collisions, lifes)) {
            if (type.type != Protocol::PLAYER) {
                if (pos.x < -col.width || pos.y < -col.height || pos.x > WIDTH + col.width || pos.y > HEIGHT + col.height)
                    life.current = -1;
            } else if (life.is_alive() && (pos.x < -15 || pos.y < -15 || pos.x > WIDTH - PLAYER_SIZE_X + 15 || pos.y > HEIGHT - PLAYER_SIZE_Y + 15)) {
                life.current = -1;
            }
        }
    }

    inline void gameOver(Registry &r, const std::function<void()> &gameOverCallBack) {
        const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
        const SparseArray<Life> &lifes = r.get_components<Life>();

        for (auto &&[network_id, life] : Zipper(network_ids, lifes))
            if (life.is_alive())
                return;
        gameOverCallBack();
    }
}
