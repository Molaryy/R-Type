/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Packet
*/

#include "Packet.hpp"

#include <ctime>

namespace Network {
    Packet::Packet() = default;

    Packet::Packet(const std::vector<byte> &data) {
        long offset = 0;
        constexpr size_t min_packet_size = sizeof(_timestamp) - sizeof(_commandId) - sizeof(uint32_t);

        if (data.size() < min_packet_size)
            throw std::runtime_error("Packet is to small : Need at least 14 bytes + payload size");

        _timestamp = deserializeField<uint64_t>(data, offset);
        offset += sizeof(_timestamp);

        _commandId = deserializeField<uint16_t>(data, offset);
        offset += sizeof(_commandId);

        _payloadSize = deserializeField<uint32_t>(data, offset);
        offset += sizeof(_payloadSize);

        if (data.size() < static_cast<std::size_t>(offset + _payloadSize))
            throw std::runtime_error(std::string("Payload size isn't : ") + std::to_string(_payloadSize));

        _payload = std::vector(data.begin() + offset, data.begin() + offset + _payloadSize);
    }

    std::vector<Packet::byte> Packet::serialize() {
        std::vector<byte> data;

        updateTimestamp();
        serializeField(_timestamp, data);
        serializeField(_commandId, data);
        serializeField(_payloadSize, data);
        data.insert(data.end(), _payload.begin(), _payload.end());

        return data;
    }

    void Packet::updateTimestamp() {
        _timestamp = std::time(nullptr);
    }

    [[nodiscard]] uint64_t Packet::getTimestamp() const {
        return _timestamp;
    }

    [[nodiscard]] uint16_t Packet::getCommandId() const {
        return _commandId;
    }
} // namespace Network
