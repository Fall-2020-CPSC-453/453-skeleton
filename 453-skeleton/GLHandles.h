#pragma once

#include "Handle.h"

#include <GL/glew.h>
#include <functional>
#include <stdexcept>

//------------------------------------------------------------------------------
// OpenGL has various combinations of glCreate* and glDelete* function pairs
//
// The handle class allows us to use them in a generic way, but requires that we
// call glCreate* and provide it with the corresponding glDelete* reference.
//
// These classes do that for the various known types that we plan to use.
//------------------------------------------------------------------------------

class GLuintHandle : public Handle<GLuint> {
	public:
		using Destructor = std::function<void(GLuint)>;

		template <typename C, typename ...Args>
		GLuintHandle(Destructor &&d, C &&c, Args&&... args)
			: Handle{d, c, args...}
		{
			if(value() == 0) {
				throw std::runtime_error("Invalid GLuintHandle");
			}
		}
};


class ShaderID : public GLuintHandle {
	public:
		ShaderID(GLenum type)
			: GLuintHandle{glDeleteShader, glCreateShader, type}
		{}
};

class ProgramID : public GLuintHandle {
	public:
		ProgramID()
			: GLuintHandle{glDeleteProgram, glCreateProgram}
		{}
};

class GenDeleteHandle : public GLuintHandle {
	public:
		template <typename D, typename G>
		GenDeleteHandle(D &&deleteFunc, G &&genFunc)
			: GLuintHandle{
				// Destructor
				[deleteFunc](GLuint  b) {
					deleteFunc(1, &b);
				},

				// Constructor
				[genFunc](){
					GLuint b{};
					genFunc(1, &b);
					return b;
				}
			}
		{}
};

class BufferID : public GenDeleteHandle {
	public:
		BufferID()
			: GenDeleteHandle{glDeleteBuffers, glGenBuffers}
		{}
};


class VertexArrayID : public GenDeleteHandle {
	public:
		VertexArrayID()
			: GenDeleteHandle{glDeleteVertexArrays, glGenVertexArrays}
		{}
};

