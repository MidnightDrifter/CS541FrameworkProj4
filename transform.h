
#ifndef _TRANSFORM_
#define _TRANSFORM_

#include <glm/glm.hpp>
using namespace glm;

typedef float ROW4[4];

class MAT4
{
public:
    float M[4][4];

    // Initilaize all matrices to the identity
    MAT4()
    {
        for( int i=0; i<4; i++ )
            for( int j=0; j<4; j++)
                M[i][j] = i==j ? 1.0 : 0.0;
    }

    // Some indexing operations for the matrix
    ROW4& operator[](const int i)  { return M[i]; }
    const ROW4& operator[](const int i) const { return M[i]; }

    // Used to communicate to OpenGL
    float* Pntr();

    // Calculate the inverse matrix.
    MAT4 inverse();
	void operator=(glm::mat4 t)
	{
		//Double check that this is correct--glm::mat4 might be in column-major order

		M[0][0] = t[0][0];
		M[0][1] = t[0][1];
		M[0][2] = t[0][2];
		M[0][3] = t[0][3];
		M[1][0] = t[1][0];
		M[1][1] = t[1][1];
		M[1][2] = t[1][2];
		M[1][3] = t[1][3];
		M[2][0] = t[2][0];
		M[2][1] = t[2][1];
		M[2][2] = t[2][2];
		M[2][3] = t[2][3];
		M[3][0] = t[3][0];
		M[3][1] = t[3][1];
		M[3][2] = t[3][2];
		M[3][3] = t[3][3];
		

/*
		M[0][0] = t[0][0];
		M[0][1] = t[1][0];
		M[0][2] = t[2][0];
		M[0][3] = t[3][0];
		M[1][0] = t[0][1];
		M[1][1] = t[1][1];
		M[1][2] = t[2][1];
		M[1][3] = t[3][1];
		M[2][0] = t[0][2];
		M[2][1] = t[1][2];
		M[2][2] = t[2][2];
		M[2][3] = t[3][2];
		M[3][0] = t[0][3];
		M[3][1] = t[1][3];
		M[3][2] = t[2][3];
		M[3][3] = t[3][3];
		*/
	}
	
};

MAT4 Rotate(const int i, const float theta);
MAT4 Scale(const vec3 s);
MAT4 Scale(const float x, const float y, const float z);
MAT4 Translate(const vec3 t);
MAT4 Translate(const float x, const float y, const float z);
MAT4 Perspective(const float rx, const float ry,
                 const float front, const float back);
MAT4 operator* (const MAT4 A, const MAT4 B);


MAT4 makeEyeViewingTransform(vec3 view, vec3 center, vec3 up);

#endif
