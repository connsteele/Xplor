#pragma once

#include <vector>
#include <xplor_types.hpp>
#include <memory>

namespace Xplor
{
    class EngineManager
    {
        // The engine manager should manage all other managers and fit all
        // the piece together. In the editor the engine manager is what will
        // be instaniated to use the xplor engine
   
        

    public:
        //EngineManager();

        //~EngineManager();

        /// <summary>
        /// Creates pointers for each manager it manages
        /// </summary>
        void Init();


    private:

    }; // end class
}; // end namespace