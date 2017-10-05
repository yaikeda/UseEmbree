//Embree renderer is an external renderer built by Intel Inc. in this system, I use Embree renderer as a first choice.
//Author: Yasunari Ikeda
//Last Update: 09/29/2017

#pragma once
#pragma warning(disable:4996)

// Headers for using Embree renderer
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <iostream>
#include <fstream>

#if 0
// If you understand how to use MXCSR setting, please turn on this block.
// Headers for setting MXCSR
#include <xmmintrin.h>
#include <pmmintrin.h>

// MXCSR setting
_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

class UseEmbree {
public:
	UseEmbree();
	~UseEmbree();
	void run();
private:
	// struct
	struct Vertex;
	struct Triangle;
	struct Color;
	struct ImagePixel;
	
	// instance variables
	RTCDevice m_device;
	RTCScene m_scene;
	const int m_width;
	const int m_height;

	// inter functions
	void addTriangleGeometry();
	void renderImage();
	Color renderPixel();
	void writeImage(ImagePixel* img);
};