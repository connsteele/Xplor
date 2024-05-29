#include <array>

class GeometryGenerator {
public:

    GeometryGenerator()
    {

    }

    static std::array<float, 180> GenerateCubeData()
    {
        std::array data{
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

    static std::array<float, 20> GeneratePlaneData()
    {
        std::array vertices{
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


    static std::vector<float> GenerateBoundingBoxVertices(const glm::vec3& min, const glm::vec3& max)
    {
        return {
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
        /*return {
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
        };*/
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

private:

};