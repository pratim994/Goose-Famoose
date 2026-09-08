#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

// ============================================================
// Renderer2D
// ============================================================

void Renderer2D::init() {
    bool ok = shader_.loadFromFiles(std::string(SHADER_DIR) + "/shape.vert",
                                     std::string(SHADER_DIR) + "/shape.frag");
    if (!ok) {
        // Fallback inline source, in case the shaders/ directory wasn't
        // found next to the executable (keeps the program runnable even
        // if it's launched from an unexpected working directory).
        const char* vs =
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "uniform mat4 uProjection;\n"
            "out vec4 vColor;\n"
            "void main() {\n"
            "    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);\n"
            "    vColor = aColor;\n"
            "}\n";
        const char* fs =
            "#version 330 core\n"
            "in vec4 vColor;\n"
            "out vec4 FragColor;\n"
            "void main() { FragColor = vColor; }\n";
        shader_.loadFromSource(vs, fs);
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Generous dynamic buffer for a frame's worth of 2D triangles.
    glBufferData(GL_ARRAY_BUFFER, 2 * 1024 * 1024 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    vertexData_.reserve(1024 * 6);
}

void Renderer2D::shutdown() {
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
    vbo_ = vao_ = 0;
}

void Renderer2D::beginFrame(int screenWidth, int screenHeight) {
    // top-left origin, y grows downward: matches window/mouse pixel coords.
    projection_ = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    vertexData_.clear();
}

void Renderer2D::endFrame() {
    lastTriCount_ = (int)(vertexData_.size() / 6 / 3);
    if (vertexData_.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    GLint bufSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufSize);
    GLsizeiptr needed = (GLsizeiptr)(vertexData_.size() * sizeof(float));
    if (needed > bufSize) {
        // Grow the buffer if a very geometry-heavy frame needs more room.
        glBufferData(GL_ARRAY_BUFFER, needed, vertexData_.data(), GL_DYNAMIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertexData_.data());
    }

    shader_.use();
    shader_.setMat4("uProjection", projection_);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vertexData_.size() / 6));
    glBindVertexArray(0);
}

void Renderer2D::pushVertex(const glm::vec2& p, const glm::vec4& color) {
    vertexData_.push_back(p.x);
    vertexData_.push_back(p.y);
    vertexData_.push_back(color.r);
    vertexData_.push_back(color.g);
    vertexData_.push_back(color.b);
    vertexData_.push_back(color.a);
}

void Renderer2D::pushTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec4& color) {
    pushVertex(a, color);
    pushVertex(b, color);
    pushVertex(c, color);
}

void Renderer2D::pushQuad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, const glm::vec4& color) {
    // a,b,c,d wound around the quad's perimeter (either direction).
    pushTriangle(a, b, c, color);
    pushTriangle(a, c, d, color);
}

void Renderer2D::pushPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color) {
    if (points.size() < 3) return;
    glm::vec2 centroid(0.0f);
    for (auto& p : points) centroid += p;
    centroid /= (float)points.size();

    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        pushTriangle(centroid, points[i], points[j], color);
    }
}

void Renderer2D::pushRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    glm::vec2 a = pos;
    glm::vec2 b = pos + glm::vec2(size.x, 0.0f);
    glm::vec2 c = pos + size;
    glm::vec2 d = pos + glm::vec2(0.0f, size.y);
    pushQuad(a, b, c, d, color);
}

void Renderer2D::pushRotatedRect(const glm::vec2& center, const glm::vec2& size, float rotationRad, const glm::vec4& color) {
    float hw = size.x * 0.5f, hh = size.y * 0.5f;
    glm::vec2 local[4] = { {-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh} };
    glm::vec2 world[4];
    for (int i = 0; i < 4; ++i) world[i] = geom::rotate(local[i], rotationRad) + center;
    pushQuad(world[0], world[1], world[2], world[3], color);
}

void Renderer2D::pushLine(const glm::vec2& a, const glm::vec2& b, float thickness, const glm::vec4& color) {
    glm::vec2 dir = b - a;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len < 1e-6f) return;
    dir /= len;
    glm::vec2 normal(-dir.y, dir.x);
    glm::vec2 offset = normal * (thickness * 0.5f);
    pushQuad(a + offset, b + offset, b - offset, a - offset, color);
}

void Renderer2D::pushCircle(const glm::vec2& center, float radius, const glm::vec4& color, int segments) {
    auto local = geom::circlePoints(radius, segments);
    auto world = geom::transform(local, center, 0.0f);
    pushPolygon(world, color);
}

void Renderer2D::pushRoundedRectOutline(const glm::vec2& center, const glm::vec2& size, float radius,
                                         float rotationRad, float thickness, const glm::vec4& color, int cornerSegments) {
    auto outerLocal = geom::roundedRectPoints(size, radius, cornerSegments);
    glm::vec2 innerSize = size - glm::vec2(thickness * 2.0f, thickness * 2.0f);
    innerSize = glm::max(innerSize, glm::vec2(0.01f, 0.01f));
    float innerRadius = std::max(0.0f, radius - thickness);
    auto innerLocal = geom::roundedRectPoints(innerSize, innerRadius, cornerSegments);

    auto outer = geom::transform(outerLocal, center, rotationRad);
    auto inner = geom::transform(innerLocal, center, rotationRad);
    size_t n = std::min(outer.size(), inner.size());
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        pushQuad(outer[i], outer[j], inner[j], inner[i], color);
    }
}

// ============================================================
// geom:: procedural point generators
// ============================================================
namespace geom {

static const float PI = 3.14159265358979323846f;

std::vector<glm::vec2> roundedRectPoints(const glm::vec2& size, float radius, int cornerSegments) {
    std::vector<glm::vec2> pts;
    float hw = size.x * 0.5f;
    float hh = size.y * 0.5f;
    float r = std::min(radius, std::min(hw, hh));

    if (r < 0.05f) {
        pts = { {-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh} };
        return pts;
    }

    struct CornerDef { glm::vec2 center; float startAngle; };
    CornerDef corners[4] = {
        { { hw - r, -hh + r }, -PI / 2.0f }, // top-right:    -90 -> 0
        { { hw - r,  hh - r },  0.0f      }, // bottom-right:   0 -> 90
        { {-hw + r,  hh - r },  PI / 2.0f }, // bottom-left:   90 -> 180
        { {-hw + r, -hh + r },  PI        }, // top-left:     180 -> 270
    };

    for (auto& c : corners) {
        for (int i = 0; i <= cornerSegments; ++i) {
            float t = c.startAngle + (PI / 2.0f) * (float(i) / float(cornerSegments));
            pts.push_back(c.center + glm::vec2(std::cos(t), std::sin(t)) * r);
        }
    }
    return pts;
}

std::vector<glm::vec2> circlePoints(float radius, int segments) {
    std::vector<glm::vec2> pts;
    pts.reserve(segments);
    for (int i = 0; i < segments; ++i) {
        float t = (2.0f * PI * i) / float(segments);
        pts.push_back({ std::cos(t) * radius, std::sin(t) * radius });
    }
    return pts;
}

std::vector<glm::vec2> heartPoints(float size, int segments) {
    // Classic parametric heart, star-shaped w.r.t. the origin, traced
    // for t in [0, 2*pi). Raw formula spans x in [-16,16], y in [-17,5].
    std::vector<glm::vec2> pts;
    pts.reserve(segments);
    float scale = size / 32.0f; // normalize raw x-range (32 wide) to `size`
    for (int i = 0; i < segments; ++i) {
        float t = (2.0f * PI * i) / float(segments);
        float x = 16.0f * std::pow(std::sin(t), 3.0f);
        float y = 13.0f * std::cos(t) - 5.0f * std::cos(2.0f * t) - 2.0f * std::cos(3.0f * t) - std::cos(4.0f * t);
        // Negate y so the point sits at the bottom in our y-down screen
        // space (lobes up, point down — the familiar heart orientation).
        pts.push_back({ x * scale, -y * scale });
    }
    return pts;
}

glm::vec2 rotate(const glm::vec2& p, float angleRad) {
    float c = std::cos(angleRad), s = std::sin(angleRad);
    return { p.x * c - p.y * s, p.x * s + p.y * c };
}

std::vector<glm::vec2> transform(const std::vector<glm::vec2>& pts, const glm::vec2& translate,
                                  float angleRad, float scale) {
    std::vector<glm::vec2> out;
    out.reserve(pts.size());
    for (auto& p : pts) out.push_back(rotate(p * scale, angleRad) + translate);
    return out;
}

} // namespace geom