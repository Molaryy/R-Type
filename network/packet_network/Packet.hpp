/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Packet
*/

#pragma once

#include <vector>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <string>

namespace Network {
    /**
     * @class Packet
     * @brief Represents a network packet capable of serialization and deserialization.
     */
    class Packet {
    public:
        /**
         * @brief Type alias for a byte
         */
        using byte = uint8_t;

        /**
         * @brief Default constructor
         */
        Packet();


        /**
         * @brief Constructs a Packet from raw data
         * @param data A vector of bytes represeting packet raw data received throught network
         */
        explicit Packet(const std::vector<byte> &data);

        /**
         * @brief Constructs a Packet with a payload and command ID
         * @tparam PayloadType Struct to use as data
         * @param payload The payload to be included in the packet
         * @param commandId The command identifier for the packet
         */
        template<typename PayloadType>
        Packet(const PayloadType &payload, const uint16_t commandId)
            : _commandId(commandId) {
            _payloadSize = sizeof(PayloadType);
            serializeField<PayloadType>(payload, _payload);
        }


        /**
         * @brief Serializes the packet into a vector of bytes.
         * @return A vector of bytes representing the serialized packet.
         */
        [[nodiscard]] std::vector<byte> serialize();

        /**
         * @brief Updates the timestamp of the packet to the current time.
         */
        void updateTimestamp();

        /**
         * @brief Gets the timestamp of the packet.
         * @return The timestamp as a 64-bit unsigned integer.
         */
        [[nodiscard]] uint64_t getTimestamp() const;

        /**
         * @brief Gets the command ID of the packet.
         * @return The command ID as an 16-bit unsigned integer.
         */
        [[nodiscard]] uint16_t getCommandId() const;


        /**
         * @brief Retrieves the payload from the packet.
         * @tparam PayloadType The type of the payload data.
         * @return The payload data of the specified type.
         * @throws std::runtime_error If the payload size does not match the expected size.
         */
        template<typename PayloadType>
        [[nodiscard]] PayloadType getPayload() const {
            if (_payload.size() != sizeof(PayloadType))
                throw std::runtime_error("Payload size isn't : " + std::to_string(_payload.size()));

            return deserializeField<PayloadType>(_payload, 0);
        }

    private:
        uint64_t _timestamp{};
        uint16_t _commandId{};
        uint32_t _payloadSize{};
        std::vector<byte> _payload{};

        template<typename T>
        static void serializeField(const T &value, std::vector<byte> &data) {
            const auto *bytes = reinterpret_cast<const byte *>(&value);
            data.insert(data.end(), bytes, bytes + sizeof(T));
        }

        template<typename T>
        static T deserializeField(const std::vector<byte> &data, const size_t offset) {
            if (offset + sizeof(T) > data.size())
                throw std::runtime_error("Packet can't be deserialized : Too small");
            T value;
            std::memcpy(&value, data.data() + offset, sizeof(T));
            return value;
        }
    };
} // namespace Network
