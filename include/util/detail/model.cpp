#include "util/model.hpp"
#include <stdexcept>
#include <map>

Model::Model(const std::string &path) : VAO(0), VBO(0), EBO(0)
{
    if (loadObj(path))
    {
        if (!vertices.empty() && !indices.empty())
        {
            setupMesh();
        }
        else
        {
            std::cerr << "MODEL: No vertices or indices populated after loading OBJ: " << path << std::endl;
            if (vertices.empty())
                std::cerr << "MODEL: Vertices list is empty." << std::endl;
            if (indices.empty())
                std::cerr << "MODEL: Indices list is empty." << std::endl;
        }
    }
    else
    {
        std::cerr << "MODEL: Failed to load OBJ file: " << path << std::endl;
    }
}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Model::loadObj(const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ERROR::MODEL::Could not open file: " << path << std::endl;
        return false;
    }

    std::vector<std::array<float, 3>> temp_positions;
    std::vector<std::array<float, 2>> temp_texcoords;
    std::vector<std::array<float, 3>> temp_normals;
    std::vector<Vertex> unique_vertices;
    std::map<std::string, unsigned int> unique_vertex_map;
    std::vector<unsigned int> indices;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword; // Đọc từ khóa đầu tiên của dòng (v, f, vt, vn, #, ...)

        if (keyword == "v")
        { // Vertex position
            Vertex current_vertex;
            if (!(ss >> current_vertex.Position[0] >> current_vertex.Position[1] >> current_vertex.Position[2]))
            {
                std::cerr << "ERROR::MODEL::Could not parse vertex position in line: " << line << std::endl;
                continue;
            }
            temp_vertex_positions.push_back(current_vertex);
        }
        else if (keyword == "f")
        { // Face
            std::string face_vertex_str;
            unsigned int v_indices_for_face[4]; // Hỗ trợ tối đa 4 đỉnh (quad)
            int vertex_count_in_face = 0;

            while (ss >> face_vertex_str && vertex_count_in_face < 4)
            {
                std::stringstream face_ss(face_vertex_str);
                std::string v_str;
                std::getline(face_ss, v_str, '/'); // Lấy phần trước dấu '/' đầu tiên

                try
                {
                    unsigned int v_idx = std::stoul(v_str);
                    // Chỉ số trong file .obj là 1-based, cần chuyển sang 0-based cho mảng
                    if (v_idx == 0 || v_idx > temp_vertex_positions.size())
                    {
                        std::cerr << "ERROR::MODEL::Invalid vertex index " << v_idx
                                  << " (1-based) for temp_vertex_positions size " << temp_vertex_positions.size()
                                  << " in line: " << line << std::endl;
                        vertex_count_in_face = 0; // Đánh dấu mặt này không hợp lệ
                        break;
                    }
                    v_indices_for_face[vertex_count_in_face++] = v_idx - 1; // 0-based
                }
                catch (const std::invalid_argument &ia)
                {
                    std::cerr << "ERROR::MODEL::Invalid vertex index string: \"" << v_str << "\" in line: " << line << std::endl;
                    vertex_count_in_face = 0;
                    break;
                }
                catch (const std::out_of_range &oor)
                {
                    std::cerr << "ERROR::MODEL::Vertex index out of range: \"" << v_str << "\" in line: " << line << std::endl;
                    vertex_count_in_face = 0;
                    break;
                }
            }

            if (vertex_count_in_face >= 3)
            { // Cần ít nhất 3 đỉnh để tạo tam giác
                // Tam giác đầu tiên
                this->indices.push_back(v_indices_for_face[0]);
                this->indices.push_back(v_indices_for_face[1]);
                this->indices.push_back(v_indices_for_face[2]);

                if (vertex_count_in_face == 4)
                {                                                   // Nếu là tứ giác, tạo tam giác thứ hai
                    this->indices.push_back(v_indices_for_face[0]); // v1
                    this->indices.push_back(v_indices_for_face[2]); // v3
                    this->indices.push_back(v_indices_for_face[3]); // v4
                }
            }
            else if (vertex_count_in_face > 0)
            { // Nếu có đọc được đỉnh nhưng không đủ 3
                std::cerr << "WARNING::MODEL::Face with less than 3 vertices ignored in line: " << line << std::endl;
            }
        }
        // Bỏ qua các dòng khác như vt, vn, #, usemtl, mtllib, o, g, s
    }
    file.close();

    // Gán các đỉnh đã đọc vào vector vertices của model
    if (!temp_vertex_positions.empty())
    {
        this->vertices = std::move(temp_vertex_positions); // Di chuyển dữ liệu hiệu quả
    }
    else
    {
        std::cerr << "ERROR::MODEL::No vertex positions ('v') found in OBJ file: " << path << std::endl;
        return false;
    }

    if (this->indices.empty() && !this->vertices.empty())
    {
        std::cout << "WARNING::MODEL::No faces ('f') found or parsed correctly in OBJ file. Model might not render if it relies on indexed drawing." << std::endl;
    }

    return true;
}

void Model::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Thuộc tính vị trí đỉnh
    // Layout location = 0 trong shader
    // 3 thành phần float cho mỗi vị trí (Vertex::Position)
    // Stride là sizeof(Vertex)
    // Offset của thành viên Position trong struct Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Nếu bạn thêm các thuộc tính khác vào Vertex (ví dụ: Normal), bạn sẽ cấu hình chúng ở đây:
    // Ví dụ cho Normal (giả sử layout location = 1 trong shader):
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO để tránh thay đổi ngoài ý muốn
}

void Model::Draw(GLuint shaderProgram)
{
    if (VAO == 0 || indices.empty())
    { // Kiểm tra xem VAO đã được tạo và có chỉ số để vẽ không
        return;
    }

    glUseProgram(shaderProgram); // Kích hoạt shader program
    glBindVertexArray(VAO);      // Bind VAO của model này
    // Vẽ các tam giác sử dụng EBO
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO
    glUseProgram(0);      // Tắt shader program (tùy chọn, nhưng là thói quen tốt)
}