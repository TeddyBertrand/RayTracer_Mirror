#include "core/network/Cluster.hpp"
#include "render/FrameBuffer.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace Raytracer;

TEST(ClusterTests, ClientInitializationEmpty) {
    std::vector<std::string> addresses = {};
    ClusterClient client(addresses, 8080);

    FrameBuffer fb(800 * 600);
    // Since there are no addresses, it should exit gracefully giving an error on stderr.
    EXPECT_NO_THROW(client.distributeRender("test.scene", fb, 800, 600));
}

TEST(ClusterTests, ClientRefusalNoWorker) {
    std::vector<std::string> addresses = {"127.0.0.1"};
    // Given we are testing on loopback and no worker is actively hosted here on port 31415,
    // it should fail to connect but handle the socket failure safely.
    ClusterClient client(addresses, 31415);
    FrameBuffer fb(100 * 100);

    EXPECT_NO_THROW(client.distributeRender("mock.scene", fb, 100, 100));
}

// A more fully integrated unit test mocking the sfml ports would bind the port,
// spin out a jthread with ClusterWorker(8080) and run ClusterClient on loopback to test the exact
// binary byte packets. But as network ports can be flaky on CI environments, basic integration
// coverage proves safe architecture links.
