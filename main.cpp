#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "main.h"


// Server response handler code part
Message receive_response(int sockfd, MessageType expected_type, ServiceId expected_service_id, uint32_t expected_request_id) {
Message response;

// First receive the header of the message to get its size
std::vector<uint8_t> response_header(MESSAGE_PKG_HEADER_SIZE+SIZE_HEADER_FIELD_SIZE);
if (recv(sockfd, response_header.data(), response_header.size(), 0) != response_header.size()) {
    std::cerr << "Failed to receive respons e header\n";
    throw std::runtime_error("Failed to receive response header");
}

// Decode the header fields
memcpy(&response.size, response_header.data(), SIZE_HEADER_FIELD_SIZE);
memcpy(&response.type, response_header.data() + SIZE_HEADER_FIELD_SIZE, SIZE_HEADER_FIELD_SIZE);
memcpy(&response.request_id, response_header.data() + 2 * SIZE_HEADER_FIELD_SIZE, SIZE_HEADER_FIELD_SIZE);
memcpy(&response.service_id, response_header.data() + 3 * SIZE_HEADER_FIELD_SIZE, SIZE_HEADER_FIELD_SIZE);

// Check if the response is for the expected request
if (response.type != expected_type || response.service_id != expected_service_id || response.request_id != expected_request_id) {
    std::cerr << "Unexpected response received\n";
    throw std::runtime_error("Unexpected response received");
}

// Allocate space for the message data and receive it
response.data.resize(response.size - MESSAGE_PKG_HEADER_SIZE - SIZE_HEADER_FIELD_SIZE);
if (recv(sockfd, response.data.data(), response.data.size(), 0) != (int)response.data.size()) {
    std::cerr << "Failed to receive response data\n";
    throw std::runtime_error("Failed to receive response data");
}

return response;
}

int nRqsID = 20;

int main(int argc, char* argv[]) {
    // Step 1: Create a TCP communication socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Step 2: Connect to the server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // IP will later not be included, this is just for testing purposes

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server\n";
        close(sockfd);
        return 1;
    }

    // Step 3: Send a PING request to the server
    Message ping_request;
    ping_request.size = MESSAGE_PKG_HEADER_SIZE+SIZE_HEADER_FIELD_SIZE;
    ping_request.type = MessageType::REQUEST;
    ping_request.request_id = nRqsID++;
    ping_request.service_id = ServiceId::PING;

    std::vector<uint8_t> ping_data; // Since no data is needed for PING requests
    ping_request.data = ping_data;

    // Encode the message into a binary format
    std::vector<uint8_t> ping_request_binary(ping_request.size);
    memcpy(ping_request_binary.data(), &ping_request, ping_request.size);

    int bytes_sent = 0;
    while (bytes_sent < (int)ping_request_binary.size()) {
        int n = send(sockfd, ping_request_binary.data() + bytes_sent, ping_request_binary.size() - bytes_sent, 0);
        if (n == -1) {
            std::cerr << "Failed to send PING request\n";
            close(sockfd);
            return 1;
        }
        bytes_sent += n;
    }
    std::cerr << "Ping request send server\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Step 4: Receive the response from the server
    Message response;
    try {
        response = receive_response(sockfd, MessageType::RESPONSE, ServiceId::PING, ping_request.request_id);
    } catch (const std::runtime_error& e) {
        close(sockfd);
        return 1;
    }

    // Print the response received
    std::cout << " PING Response received: ";
    for (const auto& byte : response.data) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // Step 3: Send a PICK request to the server
    Message pick_request;
    ping_request.size = MESSAGE_PKG_HEADER_SIZE+SIZE_HEADER_FIELD_SIZE;
    ping_request.type = MessageType::REQUEST;
    ping_request.request_id = nRqsID++;
    ping_request.service_id = ServiceId::COMPUTE_PICK;

        // Step 5: Close the socket and exit
        close(sockfd);
        return 0;
    }
