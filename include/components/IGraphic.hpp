#pragma once

namespace Raytracer {

/**
 * @brief Interface for graphics output handling (display window, pixel buffer, etc.)
 */
class IGraphic {
public:
    virtual ~IGraphic() = default;

    /**
     * @brief Initialize the graphic display (create window, buffers, etc.)
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool init() = 0;

    /**
     * @brief Inform the graphic backend of the final image size to display.
     */
    virtual void setImageSize(int width, int height) = 0;

    /**
     * @brief Update a single pixel in the buffer
     * @param x X coordinate
     * @param y Y coordinate
     * @param color Packed color value (RGBA)
     */
    virtual void updatePixel(int x, int y, unsigned int color) = 0;

    /**
     * @brief Refresh/present the current buffer to the display
     */
    virtual void refresh() = 0;

    /**
     * @brief Check if the display window is still open
     * @return true if open, false if closed
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief Close the graphic display (window) if applicable
     */
    virtual void close() = 0;
};

} // namespace Raytracer
