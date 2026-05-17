# RFC: Clustering Architecture for Distributed Raytracing

## Context
As part of the G-OOP-400 RayTracer project, the "Clustering" feature requires rendering a single scene across multiple machines connected via a network to drastically reduce rendering time. This is achieved by dividing the final image into distinct screen-space boundaries and tasking individual worker nodes with rendering subsets of the frame. 

## Design Architecture 
The clustering system delegates responsibility via a **Master / Worker (Client / Server) Architecture**. 
We rely on the standard TCP stack using the SFML Network module (`sf::TcpSocket` and `sf::TcpListener`). 

### 1. `ClusterWorker` (Server / Slave)
The worker application listens on a specific TCP port (e.g., `8080`). 
When it runs, it continuously accepts connections. 
Once a `ClusterClient` (Master) connects to it, it attempts to receive a payload `sf::Packet` detailing:
- The scene filepath (both master and workers must have access to the `.scene` definition).
- The full render width and height.
- `startY` and `endY` constraints specifying which horizontal slice of the view the worker is responsible for.

Once received, the `ClusterWorker`:
1. Reconstructs a full local scene representation via the existing `SceneParser` and `PluginLoader`.
2. Triggers the targeted IRenderer implementation on the specific partial chunk `[startY, endY]`. 
3. Streams the colors from its local `FrameBuffer` back through a resulting `sf::Packet` using unsigned 8-bit integers (`std::uint8_t`) to bound output ranges [0, 255] for R, G, and B.

### 2. `ClusterClient` (Client / Master) 
When the CLI argument `--cluster 192.168.X.Y,127.0.0.1` is provided alongside a `.scene` filepath, the raytracer instantiates a `ClusterClient` giving it the array of worker IPs.

The Client:
1. Connects to the requested IP endpoints.
2. Slices the target canvas equally among the available connected workers. For instance, if 2 workers are available for a 400 height image, Worker 1 gets `[0, 200]` and Worker 2 gets `[200, 400]`.
3. Issues the payloads containing the bounds to each machine.
4. Waits synchronously to receive the computed chunk data arrays. 
5. Unmarshalls the buffers and composites them sequentially onto the master `FrameBuffer`.
6. Refreshes the display graphics.

## Extensibility & Constraints
- Both ends must compile and strictly link against `sfml-network` natively across the CMake configuration.
- Endianness is handled automatically by SFML Packets across potential OS cross-compilations.
- Current constraints dictate relying on a fully shared file system structure between machines for resolving relative `.scene` artifacts, external `.obj` files or `.png` textures. 

## CLI Usage Model
**On Worker Machine:**
```bash
./raytracer --worker 8080
```
**On Master Machine:**
```bash
./raytracer scenes/test.scene --cluster 127.0.0.1,192.168.0.42
```
