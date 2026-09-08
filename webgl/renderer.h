#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

// ============================================================
// Renderer2D
//
// Every visual element in this game — the felt table, the card
// bodies, the procedural suit glyphs, the stroke-font text, hover
// highlights, everything — boils down to colored 2D triangles.
//
// Rather than giving TableRenderer / CardRenderer / FontRenderer
// their own VAOs and draw calls, they all *push* geometry into this
// single batched renderer, which uploads one vertex buffer and issues
// one draw call per frame. This keeps "OpenGL calls" concentrated in
// exactly one place, and keeps the higher-level renderers as plain
// geometry generators.
//
// Coordinate system: pixel space, origin top-left, +x right, +y down
// (matches window/mouse coordinates so layout math stays intuitive).
// ============================================================
class Renderer2D {
public:
    void init();
    void shutdown();

    void beginFrame(int screenWidth, int screenHeight);
    void endFrame();

    // ---- low level ----
    void pushTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec4& color);
    void pushQuad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, const glm::vec4& color);
    // Fans from the polygon's centroid. Works for convex polygons and any
    // polygon that is star-shaped w.r.t. its centroid (rounded rects,
    // circles, our heart/spade glyphs all qualify).
    void pushPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);

    // ---- higher level helpers ----
    void pushRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    void pushRotatedRect(const glm::vec2& center, const glm::vec2& size, float rotationRad, const glm::vec4& color);
    void pushLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color);
    void pushCircle(const glm::vec2& center, float radius, const glm::vec4& color, int segments = 24);
    void pushRoundedRectOutline(const glm::vec2& center, const glm::vec2& size, float radius,
                                 float rotationRad, float thickness, const glm::vec4& color, int cornerSegments = 6);

    int trianglesDrawnLastFrame() const { return lastTriCount_; }

private:
    Shader shader_;
    GLuint vao_ = 0, vbo_ = 0;
    std::vector<float> vertexData_; // interleaved: x,y,r,g,b,a  (6 floats/vertex)
    glm::mat4 projection_{1.0f};
    int lastTriCount_ = 0;

    void pushVertex(const glm::vec2& p, const glm::vec4& color);
};

// ---- Small procedural-geometry helpers shared by table/card/font/suit code ----
namespace geom {
    std::vector<glm::vec2> roundedRectPoints(const glm::vec2& size, float radius, int cornerSegments = 6);
    std::vector<glm::vec2> circlePoints(float radius, int segments = 24);
    // Classic star-shaped-from-origin parametric heart curve, traced t in [0, 2*pi).
    std::vector<glm::vec2> heartPoints(float size, int segments = 40);

    glm::vec2 rotate(const glm::vec2& p, float angleRad);
    std::vector<glm::vec2> transform(const std::vector<glm::vec2>& pts, const glm::vec2& translate,
                                      float angleRad, float scale = 1.0f);
}