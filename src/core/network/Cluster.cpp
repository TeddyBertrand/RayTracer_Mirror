#include "core/network/Cluster.hpp"
#include "components/IRenderer.hpp"
#include "core/Raytracer.hpp"
#include "core/parser/SceneParser.hpp"
#include "core/plugin_loader/PluginLoader.hpp"
#include "core/scene/Scene.hpp"
#include "factory/SceneFactories.hpp"
#include "render/FrameBuffer.hpp"
#include <fstream>
#include <iostream>

namespace Raytracer {

ClusterWorker::ClusterWorker(unsigned short port) : _port(port) {}

void ClusterWorker::run() {
    if (_listener.listen(_port) != sf::Socket::Status::Done) {
        std::cerr << "Worker failed to listen on port " << _port << std::endl;
        return;
    }
    std::cout << "Worker listening on port " << _port << "..." << std::endl;

    while (_running) {
        sf::TcpSocket client;
        if (_listener.accept(client) != sf::Socket::Status::Done) {
            continue;
        }
        std::cout << "Master connected from " << client.getRemoteAddress().value() << std::endl;

        sf::Packet packet;
        if (client.receive(packet) == sf::Socket::Status::Done) {
            std::string scenePath;
            int renderWidth, renderHeight;
            int startY, endY;

            if (packet >> scenePath >> renderWidth >> renderHeight >> startY >> endY) {
                std::cout << "Received job: " << scenePath << " rows " << startY << "-" << endY
                          << std::endl;

                try {
                    SceneFactories factories;
                    PluginLoader pluginLoader(factories);
                    pluginLoader.loadPlugins("plugins");
                    SceneParser parser(factories);
                    Scene scene;
                    parser.loadScene(scenePath, scene);
                    auto renderer = parser.getRenderer();
                    scene.buildBVH();

                    FrameBuffer buffer(renderWidth * renderHeight);
                    renderer->render(scene, buffer, nullptr, startY, endY);

                    sf::Packet resultPacket;
                    resultPacket << startY << endY;
                    for (int y = startY; y < endY; ++y) {
                        for (int x = 0; x < renderWidth; ++x) {
                            Color c = buffer[y * renderWidth + x];
                            resultPacket << static_cast<std::uint8_t>(
                                                std::min(255.0, std::max(0.0, c.r * 255.0)))
                                         << static_cast<std::uint8_t>(
                                                std::min(255.0, std::max(0.0, c.g * 255.0)))
                                         << static_cast<std::uint8_t>(
                                                std::min(255.0, std::max(0.0, c.b * 255.0)));
                        }
                    }
                    if (client.send(resultPacket) != sf::Socket::Status::Done) { std::cerr << "Failed to send chunk back." << std::endl; }
                    std::cout << "Sent chunk back to master." << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Worker failed to render: " << e.what() << std::endl;
                }
            }
        }
    }
}

ClusterClient::ClusterClient(const std::vector<std::string>& workerAddresses, unsigned short port)
    : _workerAddresses(workerAddresses), _port(port) {}

void ClusterClient::distributeRender(const std::string& scenePath,
                                     FrameBuffer& targetBuffer,
                                     int width,
                                     int height) {
    _sockets.clear();
    for (const auto& addr : _workerAddresses) {
        auto socket = std::make_unique<sf::TcpSocket>();
        auto ip = sf::IpAddress::resolve(addr);
        if (ip.has_value() && socket->connect(ip.value(), _port) == sf::Socket::Status::Done) {
            _sockets.push_back(std::move(socket));
        }
    }

    if (_sockets.empty()) {
        std::cerr << "No workers available! Cannot distribute render." << std::endl;
        return;
    }

    int rowsPerWorker = height / _sockets.size();

    for (size_t i = 0; i < _sockets.size(); ++i) {
        int startY = i * rowsPerWorker;
        int endY = (i == _sockets.size() - 1) ? height : (i + 1) * rowsPerWorker;

        sf::Packet packet;
        packet << scenePath << width << height << startY << endY;
        if (_sockets[i]->send(packet) != sf::Socket::Status::Done) { std::cerr << "Failed to send job to worker." << std::endl; }
    }

    for (size_t i = 0; i < _sockets.size(); ++i) {
        sf::Packet resultPacket;
        if (_sockets[i]->receive(resultPacket) == sf::Socket::Status::Done) {
            int startY, endY;
            if (resultPacket >> startY >> endY) {
                for (int y = startY; y < endY; ++y) {
                    for (int x = 0; x < width; ++x) {
                        std::uint8_t r, g, b;
                        resultPacket >> r >> g >> b;
                        targetBuffer[y * width + x] = Color(r / 255.0, g / 255.0, b / 255.0);
                    }
                }
                std::cout << "Received result from worker " << i << " (" << startY << "-" << endY
                          << ")" << std::endl;
            }
        }
    }
}

} // namespace Raytracer
