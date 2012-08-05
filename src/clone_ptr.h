#ifndef TINSGame2012_clone_ptr_h
#define TINSGame2012_clone_ptr_h
#include <algorithm>
#include <cassert>
//It is desirable for this to be a smart reference
//(rather than a smart pointer)
//but this is not possible in C++ ):
template<typename Cloneable>
class clone_ptr
{
public:
    clone_ptr() :
        obj(0)
    {
    }
    //clone_ptr takes ownership of p
    //(that is, it does not clone p)
    explicit clone_ptr(Cloneable* p) :
        obj(p)
    {
        assert(obj);
    }
    clone_ptr(clone_ptr const& o) :
        obj(o.obj?o.obj->clone():0)
    {
    }
    clone_ptr& operator=(clone_ptr o)
    {
        swap(o);
        return *this;
    }
    
    void swap(clone_ptr& o) {
        std::swap(obj, o.obj);
    }
    ~clone_ptr() {
        delete obj;
    }
    Cloneable& get() {
        assert(obj);
        return *obj;
    }
    Cloneable const& get() const {
        assert(obj);
        return *obj;
    }
    Cloneable* operator->() const {
        return obj;
    }
    Cloneable& operator*() const {
        return *obj;
    }
private:
    Cloneable* obj;
};

template <typename Cloneable>
inline void swap(clone_ptr<Cloneable>& l, clone_ptr<Cloneable>& r) { l.swap(r); }


#endif
