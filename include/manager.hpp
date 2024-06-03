#pragma once

#include <memory>

namespace Xplor
{
    /// <summary>
    /// Parent template class for all Singleton Manager Classes
    /// </summary>
    template<typename T>
    class Manager
    {
        public:
            static std::shared_ptr<T> getInstance()
            {
                if (!m_instance)
                    m_instance = std::make_shared<T>();
                return m_instance;
            }

        protected:
        private:
            static std::shared_ptr<T> m_instance;
    };

    // Declare space
    template<typename T>
    std::shared_ptr<T> Manager<T>::m_instance = nullptr; 
}