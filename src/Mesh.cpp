#include "Mesh.hpp"
#include <vector>

/**
 * @brief Construct a new Mesh:: Mesh object
 *        Rrepresents a wireframe mesh in OpneGL using vertex and edge data
 *        Handles creation, management, and drawing of mesh data stored in GPU buffers.
 * 
 * @param vertices 
 * @param edges 
 */
Mesh::Mesh(const std::vector<MiniGLM::vec3>& vertices, const std::vector<std::pair<int, int>>& edges)
    : VAO_(0), VBO_(0), EBO_(0), edgeCount_(0) {
    setupMesh(vertices, edges);
}

/**
 * @brief Destroy the Mesh:: Mesh object
 *        Clean up OpenGL resources
 *        Deletes VAO, VBO, and EBO if they have been created
 */
Mesh::~Mesh() {
    if (EBO_) glDeleteBuffers(1, &EBO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
}

/**
 * @brief Move Constructor
 *        Tranfers ownership of OpenGL resources from one Mesh instance.
 *        The Source's OpenGL handles are reset to zero.
 * 
 * @param other Mesh instance to move from
 */
Mesh::Mesh(Mesh&& other) noexcept
    : VAO_(other.VAO_), VBO_(other.VBO_), EBO_(other.EBO_), edgeCount_(other.edgeCount_) {
    other.VAO_ = other.VBO_ = other.EBO_ = 0;
    other.edgeCount_ = 0;
}

/**
 * @brief Move assignment operator.
 *
 * Transfers OpenGL resources and cleans up existing ones.
 * Resets the source's OpenGL handles to zero.
 *
 * @param other Mesh instance to move from.
 * @return Reference to this Mesh.
 */
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (EBO_) glDeleteBuffers(1, &EBO_);
        if (VBO_) glDeleteBuffers(1, &VBO_);
        if (VAO_) glDeleteVertexArrays(1, &VAO_);

        VAO_ = other.VAO_; VBO_ = other.VBO_; EBO_ = other.EBO_; edgeCount_ = other.edgeCount_;
        other.VAO_ = other.VBO_ = other.EBO_ = 0;
        other.edgeCount_ = 0;
    }
    return *this;
}

/**
 * @brief Sets up mesh data and uploads it to GPU buffers.
 * 
 *        - Converts the list of edge pairs (each a tuple of vertex indices) into
 *        a flat vector of unsigned intefer indices. Each edge is split into two
 *        indices.
 * 
 *        - Calculates and stores the total number of indices which determines how
 *         many elements will be drawn
 * 
 *        - Creates OpenGL handles for the VAO, VBO and EBO, allocating unique GPU
 *        resources for each via 'glGenVertexArrays' and 'glGenBuffers'.
 * 
 *        - VAO binding: Binds the VAO so subsequent configuration is store in it.
 *        
 *        - VBO binding: Binds the VBO and uploads all vertex position to the GPU
 *        'glBufferData'. Data is marked as `GL_STATIC_DRAW`, indicating the data 
 *         will not change (for performance).
 * 
 *        - Vertex attribute setup: Enables vertex attributes index 0 and describes
 *        to OpenGL how to interpret each vertex as a 3-component float vector,
 *        non-normalized, tightly packed.
 * 
 *        - EBO binding: Binds the EBO and uploads the flat list of edge indices to
 *        the GPU, also with 'glBufferData'.
 * 
 *        - Unbind VOA to prevent accidental modification of the mesh's configuration
 *        from outside this function This completes all setup needed for future rendering.
 * 
 * @param vertices List of vertex positions
 * @param edges List of edges as index pairs into the vertex array.
 */
void Mesh::setupMesh(const std::vector<MiniGLM::vec3>& vertices, const std::vector<std::pair<int, int>>& edges) {
    std::vector<GLuint> edgeIndices;
    edgeIndices.reserve(edges.size() * 2);
    for (const auto& e : edges) {
        edgeIndices.push_back(static_cast<GLuint>(e.first));
        edgeIndices.push_back(static_cast<GLuint>(e.second));
    }
    edgeCount_ = static_cast<GLsizei>(edgeIndices.size());

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MiniGLM::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MiniGLM::vec3), reinterpret_cast<void*>(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(GLuint), edgeIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

/**
 * @brief Binds the mesh's VAO for rendering.
 * 
 */
void Mesh::bind() const {
    glBindVertexArray(VAO_);
}

/**
 * @brief Draws the mesh as lines.
 *        
 *        Binds VAO and issues a draw call using edge indices with GL_Lines. Afterwards unbinds the VAO
 * 
 */
void Mesh::draw() const {
    glBindVertexArray(VAO_);
    glDrawElements(GL_LINES, edgeCount_, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

/**
 * @brief Unbinds any VAO, resetting OpenGL's current VAO binding.
 * 
 */
void Mesh::unbind() {
    glBindVertexArray(0);
}
