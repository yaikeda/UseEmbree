#include "useembree.h"

#include <string>
#include <opencv2/opencv.hpp>

// define struct
struct UseEmbree::Vertex { float x, y, z, a; };
struct UseEmbree::Triangle { int v0, v1, v2; };
struct UseEmbree::Color { float r, g, b, a; };
struct UseEmbree::ImagePixel { unsigned char b, g, r, a; };
  
UseEmbree::UseEmbree():
	m_width(256),
	m_height(256)
{
	m_device = rtcNewDevice(NULL);
	std::cout << "UseEmbree: created a new device." << std::endl;
}

UseEmbree::~UseEmbree()
{
	rtcDeleteDevice(m_device);
	std::cout << "UseEmbree: deleted a device." << std::endl;
}

void UseEmbree::run()
{
	// initialize at application startup
	rtcInit();
	std::cout << "rtcInit();" << std::endl;

	// create scene
	// RTC_SCENE_STATIC or RTC_SCENE_DYNAMIC
	// RTC_INTERSECT<X> means a number of rays
	m_scene = rtcNewScene(RTC_SCENE_STATIC, RTC_INTERSECT1);
	std::cout << "create Scene" << std::endl;

	// add geometries 
	addTriangleGeometry();

	// trace rays
	renderImage();

	// cleanup at application exit
	rtcExit();
	std::cout << "rtcExit();" << std::endl;
}

void UseEmbree::addTriangleGeometry()
{
	size_t numTriangles = 1;
	size_t numVertices = 3;

	// create geometry (like alloc() in C?)
	unsigned int geomID = rtcNewTriangleMesh2(m_scene, RTC_GEOMETRY_STATIC, numTriangles, numVertices);

	// fill vertex buffer
	Vertex* vertices = (Vertex*)rtcMapBuffer(m_scene, geomID, RTC_VERTEX_BUFFER);
	vertices[0].x = -10.0f; vertices[0].y = -10.0f; vertices[0].z = 10.0f;
	vertices[1].x =  0.0f; vertices[1].y = 10.0f; vertices[1].z = 10.0f;
	vertices[2].x = 10.0f; vertices[2].y = -10.0f; vertices[2].z = 10.0f;
	rtcUnmapBuffer(m_scene, geomID, RTC_VERTEX_BUFFER);
	
	// fill index buffer
	Triangle* triangles = (Triangle*)rtcMapBuffer(m_scene, geomID, RTC_INDEX_BUFFER);
	triangles[0].v0 = 0; triangles[0].v1 = 1; triangles[0].v2 = 2;
	rtcUnmapBuffer(m_scene, geomID, RTC_INDEX_BUFFER);

	// commit changes of the scene
	rtcCommit(m_scene);
	std::cout << "addTriangleGeometry();" << std::endl;
}

void UseEmbree::renderImage()
{
	// variables
	ImagePixel* img = new ImagePixel[m_width * m_height];

	// for each pixel
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			Color color = renderPixel(&x, &y);
			img[y * m_width + x].r = (unsigned char)(255.0f * color.r);
			img[y * m_width + x].g = (unsigned char)(255.0f * color.g);
			img[y * m_width + x].b = (unsigned char)(255.0f * color.b);
			img[y * m_width + x].a = 0;
		}
	}

	// output image
	writeImage(img);

	delete(img);
	std::cout << "renderImage();" << std::endl;
}

UseEmbree::Color UseEmbree::renderPixel(const int *x, const int *y)
{
	// initialize ray
	RTCRay ray;
	ray.org[0] = 0.0f;
	ray.org[1] = 0.0f;
	ray.org[2] = 0.0f;

	ray.dir[0] = *x - m_width * 0.5;
	ray.dir[1] = *y - m_height * 0.5;
	ray.dir[2] = 1.0f; // should rewrite
	ray.tnear = 0.0f;
	ray.tfar = std::numeric_limits<float>::infinity();
	ray.time = 0;
	ray.geomID = RTC_INVALID_GEOMETRY_ID;
	ray.primID = RTC_INVALID_GEOMETRY_ID;
	ray.mask = -1;

	// intersect ray
	rtcIntersect(m_scene, ray);

	// shade this pixel
	Color color;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 0.0f;

	if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 0.0f;
		color.a = 0.0f;
	}

	return color;
}

void UseEmbree::writeImage(ImagePixel* img)
{
	// Image viewer
	// input: one pointer to rendered image file
	// output: no
	// purpose: view rendered image 
	//
	// If you would like to use another library to show the rendered images up, you can easily use them with modifying this function.
	// Currently, I'm using openCV because it has the method to output images, show images and analize images.
	// However, the calculating speed of openCV is slow because it is one of well known huge library.
	// To aboid this problem, you can use "freeglut" or "glfw"
	// Also, there are various ideas; making a dll binary file and read it from Python, read this program from WPF or Windows form or output rendered images with particular iamge format by your selves.
	// If you need a help, please let me know.
	cv::Mat im(m_width, m_height, CV_8UC4, img);
	cv::imshow("UseEmbree", im);
	cvWaitKey(0);
}