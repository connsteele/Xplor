#include <array>
#include <glm/glm.hpp>
#include <math.h>


class GeometryGenerator {
public:

    GeometryGenerator()
    {

    }

    static std::array<float, 180> generateCubeData()
    {
        std::array<float, 180> data{
            // First 3 are points and second 2 are texture coordinates
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        return data;
    }

    static std::array<float, 108> generatePlainCubeData()
    {
        std::array<float, 108> data{
            // First 3 are points and second 2 are texture coordinates
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
                                
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
                                
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
                                
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
                                
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        return data;
    }

    static std::array<float, 20> GeneratePlaneData()
    {
        std::array<float, 20> vertices{
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 1.f, 1.f, // bot right
            -0.5f, -0.5f, 0.0f, 1.f, 0.f, // bot left
            -0.5f, 0.5f, -0.0f, 0.f, 0.f // top left
        };

        return vertices;
    }

    static std::array<unsigned int, 6> GeneratePlaneEBO()
    {
        return std::array<unsigned int, 6>{
            0, 1, 3,
            1, 2, 3
        };
    }


    static std::array<float, 108> GenerateBoundingBoxVertices(const glm::vec3& min, const glm::vec3& max)
    {
        std::array<float, 108> data{
            min.x, min.y, min.z,  
            max.x, min.y, min.z,  
            max.x,  max.y, min.z, 
            max.x,  max.y, min.z, 
            min.x,  max.y, min.z, 
            min.x, min.y, min.z,  

            min.x, min.y,  max.z, 
            max.x, min.y,  max.z, 
            max.x,  max.y,  max.z,
            max.x,  max.y,  max.z,
            min.x,  max.y,  max.z,
            min.x, min.y,  max.z, 

            min.x,  max.y,  max.z,
            min.x,  max.y, min.z, 
            min.x, min.y, min.z,  
            min.x, min.y, min.z,  
            min.x, min.y,  max.z, 
            min.x,  max.y,  max.z,

            max.x,  max.y,  max.z,
            max.x,  max.y, min.z, 
            max.x, min.y, min.z,  
            max.x, min.y, min.z,  
            max.x, min.y,  max.z, 
            max.x,  max.y,  max.z,

            min.x, min.y, min.z,  
            max.x, min.y, min.z,  
            max.x, min.y,  max.z, 
            max.x, min.y,  max.z, 
            min.x, min.y,  max.z, 
            min.x, min.y, min.z,  

            min.x,  max.y, min.z, 
            max.x,  max.y, min.z, 
            max.x,  max.y,  max.z,
            max.x,  max.y,  max.z,
            min.x,  max.y,  max.z,
            min.x,  max.y, min.z
        };
        return data;
    }

    //static std::vector<unsigned int> GenerateBoundingBoxIndices()
    //{
    //    return {
    //        // Front face
    //        0, 1,
    //        1, 2,
    //        2, 3,
    //        3, 0,
    //        // Back face
    //        4, 5,
    //        5, 6,
    //        6, 7,
    //        7, 4,
    //        // Connect front and back faces
    //        0, 4,
    //        1, 5,
    //        2, 6,
    //        3, 7
    //    };
    //}

    static std::vector<float> generateCylinderVertices(float radius, float height, int slices)
    {
        std::vector<float> vertices{};

        // Top center
                                         // pos                  // normal
        vertices.insert(vertices.end(), {0.f, height / 2.f, 0.f, 0.f, 1.f, 0.f });

        // Bottom center                // pos                     // normal
        vertices.insert(vertices.end(), { 0.f, -height / 2.f, 0.f, 0.f, -1.f, 0.f });

        for (int i = 0; i <= slices; ++i)
        {
            float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
            float x = radius * cosf(theta);
            float z = radius * sinf(theta);
            
            // Top Circle Vertices
            vertices.insert(vertices.end(), {x, height / 2.f, z, 0.f, 1.f, 0.f}); // pos then normals

            // Bottom Circle Vertices
            vertices.insert(vertices.end(), { x, height / -2.f, z, 0.f, -1.f, 0.f});

            // Side Vertices (two per segment)
            vertices.insert(vertices.end(), {x, height / 2.f, z, x, 0.f, z});
            vertices.insert(vertices.end(), { x, height / -2.f, z, x, 0.f, z });
        }

        return vertices;
    }

    static std::vector<unsigned int> generateCylinderIndices(int slices)
    {
        std::vector<unsigned int> indices;

        // top circle indices
        for (int i = 2; i <= slices + 1; ++i)
        {
            indices.push_back(0); // Center top
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        // bottom circle indices
        for (int i = slices + 3; i <= 2 * slices + 2; ++i) {
            indices.push_back(1); // Center bottom
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        // side indices
        int start = 2 * slices + 4;
        for (int i = 0; i < slices; ++i) {
            int top1 = start + 2 * i;
            int top2 = start + 2 * (i + 1);
            int bottom1 = top1 + 1;
            int bottom2 = top2 + 1;

            indices.push_back(top1);
            indices.push_back(bottom1);
            indices.push_back(top2);

            indices.push_back(bottom1);
            indices.push_back(bottom2);
            indices.push_back(top2);
        }
        return indices;
    }

    static std::array<float, 0> generateCone();

private:

};