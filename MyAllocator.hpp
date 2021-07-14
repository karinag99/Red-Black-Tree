#ifndef _ALLOCATOR_
#define _ALLOCATOR_

#include <unordered_set>

template <class Type>
class MyAllocator{
private:
    std::unordered_set<Type*> allocated;

public:

    MyAllocator() = default;
    MyAllocator(const MyAllocator<Type>& other) = delete;
    MyAllocator& operator=(const MyAllocator<Type>& other) = delete;

    Type* allocate()
    {
        Type* temp = new Type();
        allocated.insert(temp);

        return temp;
    }

    template <class... Args>
    Type* allocate(Args&&... args)
    {
        Type* temp = new Type(std::forward<Args>(args)...);
        allocated.insert(temp);

        return temp;
    }

    void deallocate(Type* ptr)
    {
        if(allocated.count(ptr) == 0)
            throw std::invalid_argument("ptr is not allocated");

        allocated.erase(ptr);
        delete ptr;
    }

    bool is_allocated(Type* ptr) const
    {
        return allocated.find(ptr) != allocated.end();
    }

    size_t size() const
    {
        return allocated.size();
    }
};

#endif