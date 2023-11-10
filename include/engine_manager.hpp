#pragma once

#include <vector>
#include <xplor_types.hpp>
#include <memory>
#include "window_manager.hpp"

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

        bool Run();

        static std::shared_ptr<EngineManager> GetInstance();


    private:
        static std::shared_ptr<EngineManager> m_instance;

    }; // end class
}; // end namespace