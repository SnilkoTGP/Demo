#include "config.h"
#include <tinyxml2.h>

Config::Config(const std::string& file_path) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file_path.c_str());

    tinyxml2::XMLElement* server_element = doc.FirstChildElement("server");
    ip_address_ = server_element->FirstChildElement("ip")->GetText();
    port_ = std::stoi(server_element->FirstChildElement("port")->GetText());
}

std::string Config::GetIPAddress() const {
    return ip_address_;
}

int Config::GetPort() const {
    return port_;
}
