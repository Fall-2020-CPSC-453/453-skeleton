#pragma once

#include <functional>

//------------------------------------------------------------------------------
// An RAII wrapper around a resource that is not a pointer.
//
// Similar to a unique_ptr, it allows only move and no copies.
//
// Assumes that T has a default value, and that the destructor
// can be safely called on the default value.
//------------------------------------------------------------------------------

template <typename T>
class Handle {
	public:
		using HandleDestructor = std::function<void(T)>;

		template <typename D, typename C, typename ...Args>
		Handle(D &&d, C &&c, Args&&... args)
			: resource(c(std::forward<Args>(args)...))
			, destructor(d)
		{}

		~Handle() {
			destructor(resource);
		}


		Handle(const Handle<T> &h) = delete;
		Handle<T> &operator=(const Handle<T> &h) = delete;

		Handle(Handle<T> &&other)
			: resource{other.resource}
		{
			other.resource = {};
		}

		Handle<T> &operator=(Handle<T> &&other) {
			destructor(resource);
			resource = other.resource;
			other.resource = {};
			return *this;
		}

		operator T() const { return resource; }
		T value() const { return resource; }

	private:
		T resource;
		HandleDestructor destructor;
};
