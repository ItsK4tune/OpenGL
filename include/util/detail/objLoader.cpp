#include "util/objLoader.hpp"

bool loadObj(const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Cannot open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<std::array<float, 3>> temp_positions;
    std::vector<std::array<float, 2>> temp_texcoords;
    std::vector<std::array<float, 3>> temp_normals;
    std::vector<Vertex> unique_vertices;
    std::map<std::string, unsigned int> unique_vertex_map;
    std::vector<unsigned int> indices;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            temp_positions.push_back({x, y, z});
        }
        else if (keyword == "vt")
        {
            float u, v;
            ss >> u >> v;
            temp_texcoords.push_back({u, v});
        }
        else if (keyword == "vn")
        {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            temp_normals.push_back({nx, ny, nz});
        }
        else if (keyword == "f")
        {
            std::string vertex_str;
            std::vector<unsigned int> face_vertex_indices_for_current_face; // Lưu các chỉ số của đỉnh cho mặt hiện tại

            while (ss >> vertex_str) // Đọc tất cả các đỉnh của mặt này
            {
                if (unique_vertex_map.count(vertex_str))
                {
                    face_vertex_indices_for_current_face.push_back(unique_vertex_map[vertex_str]);
                }
                else
                {
                    std::stringstream vss(vertex_str);
                    std::string v_idx_str, vt_idx_str, vn_idx_str;

                    std::getline(vss, v_idx_str, '/');
                    std::getline(vss, vt_idx_str, '/');
                    std::getline(vss, vn_idx_str, '/'); // Sẽ là chuỗi rỗng nếu chỉ có v/vt

                    // Chuyển đổi và kiểm tra lỗi stoul cẩn thận hơn
                    unsigned int v_idx = 0, vt_idx = 0, vn_idx = 0;
                    try
                    {
                        v_idx = std::stoul(v_idx_str) - 1;
                        if (!vt_idx_str.empty())
                            vt_idx = std::stoul(vt_idx_str) - 1;
                        if (!vn_idx_str.empty())
                            vn_idx = std::stoul(vn_idx_str) - 1;
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error parsing face index: " << vertex_str << " - " << e.what() << std::endl;
                        // Có thể bỏ qua mặt này hoặc xử lý lỗi khác
                        face_vertex_indices_for_current_face.clear(); // Xóa các đỉnh đã thêm cho mặt lỗi này
                        break;                                        // Thoát khỏi vòng lặp while (ss >> vertex_str)
                    }

                    Vertex vertex{};

                    // Kiểm tra chỉ số trước khi truy cập
                    if (v_idx >= temp_positions.size())
                    { /* báo lỗi */
                        face_vertex_indices_for_current_face.clear();
                        break;
                    }
                    std::copy(temp_positions[v_idx].begin(), temp_positions[v_idx].end(), vertex.Position);

                    if (!vt_idx_str.empty())
                    {
                        if (vt_idx >= temp_texcoords.size())
                        { /* báo lỗi */
                            vertex.TexCoords[0] = temp_texcoords[vt_idx][0];
                            vertex.TexCoords[1] = 1.0f - temp_texcoords[vt_idx][1];
                        }
                        else
                            std::copy(temp_texcoords[vt_idx].begin(), temp_texcoords[vt_idx].end(), vertex.TexCoords);
                    }
                    else
                    {
                        vertex.TexCoords[0] = vertex.TexCoords[1] = 0.0f;
                    }

                    if (!vn_idx_str.empty())
                    {
                        if (vn_idx >= temp_normals.size())
                        { /* báo lỗi */
                            vertex.Normal[0] = vertex.Normal[1] = vertex.Normal[2] = 0.0f;
                        }
                        else
                            std::copy(temp_normals[vn_idx].begin(), temp_normals[vn_idx].end(), vertex.Normal);
                    }
                    else
                    {
                        // Nếu không có pháp tuyến, bạn có thể tính toán chúng sau hoặc để là 0
                        vertex.Normal[0] = vertex.Normal[1] = vertex.Normal[2] = 0.0f;
                    }

                    unsigned int new_vertex_index = static_cast<unsigned int>(unique_vertices.size());
                    unique_vertices.push_back(vertex);
                    unique_vertex_map[vertex_str] = new_vertex_index;
                    face_vertex_indices_for_current_face.push_back(new_vertex_index);
                }
            }

            // Tam giác hóa mặt (nếu có nhiều hơn 2 đỉnh)
            if (face_vertex_indices_for_current_face.size() >= 3)
            {
                // Tam giác đầu tiên
                indices.push_back(face_vertex_indices_for_current_face[0]);
                indices.push_back(face_vertex_indices_for_current_face[1]);
                indices.push_back(face_vertex_indices_for_current_face[2]);

                // Nếu là đa giác (ví dụ tứ giác), tạo thêm các tam giác
                // bằng cách sử dụng fan triangulation từ đỉnh đầu tiên
                for (size_t i = 3; i < face_vertex_indices_for_current_face.size(); ++i)
                {
                    indices.push_back(face_vertex_indices_for_current_face[0]);
                    indices.push_back(face_vertex_indices_for_current_face[i - 1]);
                    indices.push_back(face_vertex_indices_for_current_face[i]);
                }
            }
        }
    }

    outVertices = std::move(unique_vertices);
    outIndices = std::move(indices);
    return true;
}