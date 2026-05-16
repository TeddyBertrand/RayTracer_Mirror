#pragma once

#include "core/scene/Scene.hpp"
#include "render/FrameBuffer.hpp"
#include <SFML/Network.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace Raytracer {

// Skeleton for the Worker
class ClusterWorker {
public:
    ClusterWorker(unsigned short port);
    void run();

private:
    void processJob(sf::TcpSocket& client,
                    const std::string& scenePath,
                    int renderWidth,
                    int renderHeight,
                    int startY,
                    int endY);

    unsigned short _port;
    sf::TcpListener _listener;
    bool _running = true;
};

// Skeleton for the Master's Network Renderer
class ClusterClient {
public:
    ClusterClient(const std::vector<std::string>& workerAddresses, unsigned short port);

    // Divide and send work
    void distributeRender(const std::string& scenePath,
                          FrameBuffer& targetBuffer,
                          int width,
                          int height);

private:
    std::vector<std::string> _workerAddresses;
    unsigned short _port;
    std::vector<std::unique_ptr<sf::TcpSocket>> _sockets;
};

} // namespace Raytracer
