#include "core/Raytracer.hpp"
#include "core/network/Cluster.hpp"
#include <string>

int main(int argc, const char* argv[]) {
    if (argc >= 3 && std::string(argv[1]) == "--worker") {
        unsigned short port = std::stoi(argv[2]);
        Raytracer::ClusterWorker worker(port);
        worker.run();
        return 0;
    }

    Raytracer::Raytracer raytracer(argc, argv);
    raytracer.run();
    return raytracer.getStatus();
}
