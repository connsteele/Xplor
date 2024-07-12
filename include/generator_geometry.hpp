#include <array>
#include <glm/glm.hpp>
#include <math.h>

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

    static std::vector<float> generateUnitCircleVertices(int num_faces)
    {
        float face_step = 2 * PI / num_faces;
        float face_angle; // radians

        std::vector<float> vertices;
        for (int i = 0; i <= num_faces; ++i)
        {
            face_angle = i * face_step;
            // might want to swap the y and z for my unit system
            vertices.push_back(cos(face_angle)); // x
            vertices.push_back(sin(face_angle)); // y
            vertices.push_back(0.0f); // z

        }

        return vertices;
    }

    static std::vector<float> generateCylinder(float radius, float height, int faces)
    {
        float step_angle = (2 * PI) / faces; // angle between faces in radians

        // Create the top and bottom circles of the cylinder
        std::vector<float> unit_circle;
        for (int i = 0; i < faces; ++i)
        {
            float angle = i * step_angle;

            // Top
            unit_circle.push_back(radius * cos(angle)); // x
            unit_circle.push_back(height / 2.f);        // y
            unit_circle.push_back(radius * sin(angle)); // z

            // Bottom
            unit_circle.push_back(radius * cos(angle)); // x
            unit_circle.push_back(-height / 2.f);       // y
            unit_circle.push_back(radius * sin(angle)); // z

        }

        std::vector<float> vertices;
        // Generate the sides of the cylinder in counter clock wise winding
        for (int i = 0; i < faces; ++i)
        {
            int next_i = (i + 1) % faces;

            // Indices for the top and bottom vertex positions
            int top_i = 3 * (2 * i);
            int bottom_i = 3 * (2 * i + 1);
            int top_next = 3 * (2 * next_i);
            int bottom_next = 3 * (2 * next_i + 1);

            // Triangle 1 - Counter-clockwise
            vertices.push_back(unit_circle[top_i]);        // Top i
            vertices.push_back(unit_circle[top_i + 1]);    // Top i
            vertices.push_back(unit_circle[top_i + 2]);    // Top i
            vertices.push_back(unit_circle[bottom_next]);  // Bottom next
            vertices.push_back(unit_circle[bottom_next + 1]); // Bottom next
            vertices.push_back(unit_circle[bottom_next + 2]); // Bottom next
            vertices.push_back(unit_circle[bottom_i]);     // Bottom i
            vertices.push_back(unit_circle[bottom_i + 1]); // Bottom i
            vertices.push_back(unit_circle[bottom_i + 2]); // Bottom i

            // Triangle 2 - Counter-clockwise
            vertices.push_back(unit_circle[top_i]);        // Top i
            vertices.push_back(unit_circle[top_i + 1]);    // Top i
            vertices.push_back(unit_circle[top_i + 2]);    // Top i
            vertices.push_back(unit_circle[top_next]);     // Top next
            vertices.push_back(unit_circle[top_next + 1]); // Top next
            vertices.push_back(unit_circle[top_next + 2]); // Top next
            vertices.push_back(unit_circle[bottom_next]);  // Bottom next
            vertices.push_back(unit_circle[bottom_next + 1]); // Bottom next
            vertices.push_back(unit_circle[bottom_next + 2]); // Bottom next
        }

        return vertices;
    }

    

    //static std::array<float, 0> generateCone();

private:

};