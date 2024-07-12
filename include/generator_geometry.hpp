#include <array>
#include <glm/glm.hpp>
#include <math.h>
#include "xplor_types.hpp"

#define PI    3.1415926f

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


    //static void generateCylinder(float radius, float height, int faces, 
    //    std::vector<float> &out_geometry, std::vector<int>& out_indices)
    //{
    //    std::vector<float> unit_circle = generateUnitCircleVertices(faces);

    //    height /= 2.0f;
    //    int flip_height = 1; // when this hits two reset to 0 then inverse height

    //    // Create two triangles per rectangular face
    //    int k = 0; // unit circle index
    //    int k_next = 3; // unit circle index one spot ahead
    //    for (int i = 0, k = 0; i <= faces; ++i)
    //    {
    //        // If the elements of the unit circle are exhausted wrap around it
    //        k_next = k_next > (faces * 3) ? 0 : k_next;
    //        

    //        // Position data for a rectangle, keep index order in mind
    //        // top left 1
    //        out_geometry.push_back(unit_circle[k] * radius);   // x
    //        out_geometry.push_back(height);                    // y
    //        out_geometry.push_back(unit_circle[k+2] * radius); // z
    //        // bottom left 2
    //        out_geometry.push_back(unit_circle[k] * radius);     // x
    //        out_geometry.push_back(-height);                     // y
    //        out_geometry.push_back(unit_circle[k + 2] * radius); // z
    //        // bottom right 3
    //        out_geometry.push_back(unit_circle[k_next] * radius);     // x
    //        out_geometry.push_back(-height);                          // y
    //        out_geometry.push_back(unit_circle[k_next + 2] * radius); // z
    //        // top right 4
    //        out_geometry.push_back(unit_circle[k_next] * radius);     // x
    //        out_geometry.push_back(height);                           // y
    //        out_geometry.push_back(unit_circle[k_next + 2] * radius); // z

    //        // index order of points for CCW winding on both triangles
    //        out_indices.insert(out_indices.end(), { k, k + 1, k + 2 }); // 1, 2, 3
    //        out_indices.insert(out_indices.end(), { k+2, k+3, k });     // 3, 4, 1

    //        // iterate over the unit circle
    //        k += 3;
    //        k_next += 3;

    //    }
    //}

    static void generateCylinder(float radius, float height, int faces,
        std::vector<Xplor::Vertex>& out_geometry, std::vector<int>& out_indices)
    {
        // Create top and bottom data
        out_geometry = generateUnitCircleVertices(faces, height, radius);

        // Create indices for rectangle sides of the cylinder
        for (int i = 0; i < faces; ++i)
        {
            int next = (i + 1) % faces; // loop back around when hitting the edge

            // Triangle 1
            out_indices.push_back(i * 2); // top left
            out_indices.push_back(next * 2 + 1); // bottom right
            out_indices.push_back(i * 2 + 1); // bottom left
            
            // Triangle 2
            out_indices.push_back(i * 2); // top left
            out_indices.push_back(next * 2); // top right
            out_indices.push_back(next * 2 + 1); // bottom right
        }
    }

    

    //static std::array<float, 0> generateCone();

private:
    static std::vector<Xplor::Vertex> generateUnitCircleVertices(int num_faces, float height,
        float radius)
    {
        float face_step = 2 * PI / num_faces; // radians to rotate by for each face

        std::vector<Xplor::Vertex> vertices;
        for (int i = 0; i < num_faces; ++i)
        {
            float angle = i * face_step; // radians for current face
            
            // top circle
            vertices.push_back({ radius * cos(angle), height / 2.0f, radius * sin(angle)});
            // bottom circle
            vertices.push_back({ radius * cos(angle), -height / 2.0f, radius * sin(angle) });

        }

        return vertices;
    }
};