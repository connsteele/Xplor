#pragma once

#include <vector>
#include <xplor_types.hpp>
#include <memory>
#include "window_manager.hpp"
#include "camera.hpp"
#include "game_object.hpp"

namespace Xplor
{
    class EngineManager
    {
        // The engine manager should manage all other managers and fit all
        // the piece together. In the editor the engine manager is what will
        // be instaniated to use the xplor engine
   
        

    public:
        void Init();

        void CreateWindow(int width, int height, bool fullscreen);

        void CreateCamera(CameraVectors vectors, float speed = 3.f, float fov = 90.f);

        bool Run();


        // Update all game objects
        void Update(float deltaTime);

        // Render all game objects
        void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

        static std::shared_ptr<EngineManager> GetInstance();


        // We want a method that will add an object into our vector to keep track of
        // so we can update it with all the other ones and render it

        void AddGameObject(std::shared_ptr<GameObject> object);

        size_t GetObjectCount()
        {
            return m_objectCount;
        }

    private:
        static std::shared_ptr<EngineManager> m_instance;
        // Drop templating on this for now, this vector contains every game objects in the scene
        //std::vector<GameObject> gameObjects;
        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
        std::unique_ptr<Camera> m_activeCamera;
        float m_lastFrameTime{};
        size_t m_objectCount{};
        

    }; // end class
}; // end namespace