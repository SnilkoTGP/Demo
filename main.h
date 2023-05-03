#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <array>
#include <vector>
#include <cstddef>

enum class MessageType : uint32_t {
    REQUEST = 0,
    RESPONSE = 1,
    REQUEST_UPDATE = 2,
    RESPONSE_UPDATE = 3,
    NOTIFY = 4
};

enum class ServiceId : uint32_t {
    PING = 0,
    COMPUTE_PICK = 2,
    UPDATE_ROI = 1001,
    UPDATE_RGB_IMAGE = 1002,
    UPDATE_POINT_CLOUD = 1003,
    UPDATE_STOP = 1005,
    UPDATE_STATE = 2001,
    UPDATE_DETECTION = 2002
};

enum class ServerState : uint32_t {
    RECORDING = 0,
    PROCESSING = 1,
    DONE = 2
};

constexpr size_t SIZE_HEADER_FIELD_SIZE = 4;
constexpr size_t MESSAGE_PKG_HEADER_SIZE = 3 * SIZE_HEADER_FIELD_SIZE;

struct Detection {
    float score;
    std::array<float, 16> grasp_transform;
};

struct Message {
    uint32_t size;
    MessageType type;
    uint32_t request_id;
    ServiceId service_id;
    std::vector<uint8_t> data;
};

struct RGBImageData {
    uint32_t width;
    uint32_t height;
    uint32_t data_length;
    std::vector<uint8_t> data;
};

struct PointCloudData {
    uint32_t width;
    uint32_t height;
    uint32_t data_length;
    std::vector<float> data;
};

#endif // MESSAGE_H
