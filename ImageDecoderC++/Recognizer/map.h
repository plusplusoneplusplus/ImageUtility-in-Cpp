#pragma once
#include "lodepng.h"
#include <iostream>
#include <vector>

using namespace std;

typedef struct rgb_point
{
	unsigned r;
	unsigned g;
	unsigned b;
	unsigned h;

	rgb_point(unsigned _r, unsigned _g, unsigned _b, unsigned _h)
	{
		r = _r;
		g = _g;
		b = _b;
		h = _h;
	}
	rgb_point(){}

}point;


class map
{
public:
	vector<unsigned char> image;
	unsigned width, height;

	map(char* image_path)
	{
		unsigned error = lodepng::decode(image,width,height,image_path);

		if(error != 0)
		{
			cout<<"Fail to load image!!!"<<endl;
			exit(1);
		}

		cout<<"Load Image Successfully"<<endl;
	}

	point get_point(vector<unsigned char>& map, int x, int y)
	{
		int index = x * width + y;
		point p(map[4*index],map[4*index+1],map[4*index +2],map[4*index+3]);
		return p;
	}

	point* get_nine_around_points(vector<unsigned char>& map,int x, int y)
	{
		point *p = new point[9];
		p[0] = get_point(map,x-1,y-1);
		p[1] = get_point(map,x-1,y);
		p[2] = get_point(map,x-1,y+1);
		p[3] = get_point(map,x,y-1);
		p[4] = get_point(map,x,y);
		p[5] = get_point(map,x,y+1);
		p[6] = get_point(map,x+1,y-1);
		p[7] = get_point(map,x+1,y);
		p[8] = get_point(map,x+1,y+1);
		return p;
	}

	void save(char* new_path)
	{
		std::vector<unsigned char> saveImage;
		unsigned error = lodepng::encode(saveImage,image,width,height);
		lodepng::save_file(saveImage,new_path);
	}

	void set_point(vector<unsigned char>& map, int x, int y, point* p)
	{
		int index = x *width + y;
		if(p != NULL)
		{
			map[4*index] = p->r;
			map[4*index+1] = p->g;
			map[4*index+2] = p->b;
			map[4*index+3] = p->h;
		}
	}

};