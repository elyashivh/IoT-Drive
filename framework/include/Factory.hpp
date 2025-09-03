#ifndef __ILRD_FACTORY_HPP__
#define __ILRD_FACTORY_HPP__

#include <memory>        //shared_ptr
#include <unordered_map> //unordered_map
#include <functional>    //std::function
#include <stdexcept> // std::out_of_range

namespace ilrd_166_7
{
    /**
     * @class Factory
     * @brief Generic factory template for creating polymorphic objects by key.
     *
     * @tparam BASE The base class type of the objects to be created.
     * @tparam KEY The type used as a key for identifying different derived types.
     * @tparam ARGS Variadic template parameters representing constructor arguments for the derived objects.
     *
     * This factory allows dynamic registration and creation of objects based on a key.
     * Each key is associated with a creation function that returns a shared pointer to a derived instance.
     */
    template <typename BASE, typename KEY, typename... ARGS>
    class Factory
    {
    public:
        explicit Factory() = default;
        ~Factory() = default;
        Factory(const Factory &other) = delete;
        Factory &operator=(const Factory &other) = delete;

        void Add(KEY objectName, std::function<std::shared_ptr<BASE>(ARGS... args)> objectCreationFunction);
        std::shared_ptr<BASE> Create(KEY objectName, ARGS &&...args) const;

    private:
        std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS... args)>> m_objectsCreationMap;
    }; // class factory

    
    /**************** CLASS IMPLEMENTATION ***********/
    template <typename BASE, typename KEY, typename... ARGS>
    void Factory<BASE, KEY, ARGS...>::Add(KEY objectName, std::function<std::shared_ptr<BASE>(ARGS... args)> objectCreationFunction)
    {
        m_objectsCreationMap[objectName] = objectCreationFunction;
    }

    template <typename BASE, typename KEY, typename... ARGS>
    std::shared_ptr<BASE> Factory<BASE, KEY, ARGS...>::Create(KEY objectName, ARGS &&...args) const
    {
    try
    {
        std::shared_ptr<BASE> product =  m_objectsCreationMap.at(objectName)(std::forward<ARGS>(args)...);
        return product;
    }
    catch (const std::out_of_range& e) 
    { 
        throw std::runtime_error("Invalid key");
    }

    }
}       // namespace ilrd_166_7

#endif // __ILRD_FACTORY_HPP__