#pragma once

#include <vector>
#include <xplor_types.hpp>
#include <set>
#include <memory>
#include "manager.hpp"
#include "window_manager.hpp"
#include "camera.hpp"
#include "game_object.hpp"
#include <iostream>
#include <fstream>

namespace Xplor
{
    class EngineManager : public Manager<EngineManager>
    {
        // The engine manager should manage all other managers and fit all
        // the piece together. In the editor the engine manager is what will
        // be instaniated to use the xplor engine
   

    public:
        void Init();

        void createWindow(int width, int height, bool fullscreen);

        void createCamera(CameraVectors vectors, float speed = 3.f, float fov = 90.f);

        bool run();


        // Update all game logic
        void update(float deltaTime);

        // Render all game objects
        void render_objects(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);


        // We want a method that will add an object into our vector to keep track of
        // so we can update it with all the other ones and render it

        void addGameObject(std::shared_ptr<GameObject> object);

        void rayCursorTest(const Xplor::Ray& ray);

        bool rayIntersect(const Xplor::Ray & ray, const BoundingBox& bbox, float& out_t);

        void exportScene(std::string filepath)
        {
            json scene_json = SerializeScene();
            std::ofstream file(filepath);
            file << std::setw(4) << scene_json;
        }

        void importScene(std::string filepath)
        {
            std::ifstream inputFile(filepath);
            json scene_json;
            inputFile >> scene_json;
            DeserializeScene(scene_json);
        }

        void addDebugObject(const glm::vec3& position);
        void addDebugObject(const glm::vec3& position, const glm::vec3& velocity);


        size_t getObjectCount()
        {
            return m_object_count;
        }

        void clearSelection();

        std::shared_ptr<Camera> getCamera()
        {
            return m_active_camera;
        }

        const float getDeltaTime() const
        {
            return m_delta_time;
        }

        void imguiDockSpace();

        bool isSelected(const std::shared_ptr<GameObject>& game_object);

        const std::vector<std::shared_ptr<GameObject>> getGameObjects() const;

    private:
        float m_delta_time; // Time between current and last frame

        // Drop templating on this for now, this vector contains every game objects in the scene
        //std::vector<GameObject> gameObjects;
        std::vector<std::shared_ptr<GameObject>> m_game_objects{};
        std::set<std::shared_ptr<GameObject>> m_selected{}; // used for O(logn) lookup time and better cache locality than unorderd
        std::shared_ptr<Camera> m_active_camera;
        float m_last_frame_time{};
        size_t m_object_count{};

        /// <summary>
        /// Constructor
        /// </summary>

        json SerializeScene() const
        {
            json sceneData;
            for (const auto& object : m_game_objects)
            {
                sceneData.push_back(object->Serialize());
            }

            return sceneData;
        }

        void DeserializeScene(const json& sceneData)
        {
            m_game_objects.clear();

            for (const auto& objectData : sceneData)
            {
                auto type = objectData.at("type").get<Xplor::GameObjectType>();

                switch (type) {
                case Xplor::GameObjectType::GameObject: {
                    auto object = std::make_shared<GameObject>();
                    object->Deserialze(objectData);
                    m_game_objects.push_back(object);
                    break;
                }
                case Xplor::GameObjectType::PropObject: {
                    auto object = std::make_shared<PropObject>();
                    object->Deserialze(objectData);
                    m_game_objects.push_back(object);
                    break;
                }
                default:
                    assert(false && "Undefined enum value in switch statement");
                }

            }
        }
        

    }; // end class
}; // end namespace