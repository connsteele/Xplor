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

    static void generateCylinderCap(float radius, float height, int faces,
        std::vector<Xplor::Vertex>& out_geometry, std::vector<int>& out_indices)
    {
        float face_step = 2 * PI / faces; // radians to rotate by for each 
        
        // create the top center and push it back
        Xplor::Vertex top_center{0, height / 2.0f, 0};
        out_geometry.push_back(top_center);

        // Create two circles
        // top
        for (int i = 0; i < faces; ++i)
        {
            float angle = i * face_step; // radians for current face
            out_geometry.push_back({ radius * cos(angle), height / 2.0f, radius * sin(angle) });
        }

        Xplor::Vertex bot_center{ 0, -height / 2.0f, 0 };
        out_geometry.push_back(bot_center);
        // bottom
        for (int i = 0; i < faces; ++i)
        {
            float angle = i * face_step; // radians for current face
            out_geometry.push_back({ radius * cos(angle), -height / 2.0f, radius * sin(angle) });
        }


        // Create index order for the circle drawing
        // top
        for (int i = 1; i <= faces; ++i)
        {
            int next = i + 1;
            next = next > faces ? 1 : next; // loop back to 1 to fix index

            out_indices.insert(out_indices.end(), {0, i, next});
        }
        // bottom
        for (int i = 1; i <= faces; ++i)
        {
            int offset = faces + 1; // offset to start of second circle (bottom center index)
            int j = i + offset; // need advance to the second circle geometry
            int next = j + 1;
            next = next >= 2 * offset ? offset + 1 : next; // loop back to starting edge
            
            out_indices.insert(out_indices.end(), {offset, j, next });
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