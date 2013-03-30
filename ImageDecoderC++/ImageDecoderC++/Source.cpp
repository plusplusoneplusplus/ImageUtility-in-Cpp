#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

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

// original image
std::vector<unsigned char> image;
unsigned width, height;

void init(char* image_path)
{
	//char* imagePath = "C:\\Users\\Ruby\\Desktop\\final essay\\huashan.png";
	unsigned error = lodepng::decode(image,width,height,image_path);

	if(error != 0)
	{
		cout<<"Fail to load image!!!"<<endl;
		exit(1);
	}
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

void get_point_file()
{
	ofstream stream;
	stream.open("C:\\Users\\Ruby\\Desktop\\final essay\\data.txt");

	unsigned int count = 0;

	for(int i = 0 ; i < height; i ++)
	{
		for(int j = 0 ; j < width ; j++)
		{
			int index = width * i + j;

			unsigned char* pixel = &image[index * 4];
			if(*pixel == 0 && *(pixel+1) == 0 && *(pixel+2) == 0 && *(pixel+3) == 255)
			{
				count ++;
				//stream<<i << "," << j <<std::endl;
			}
		}
	}

	stream.close();
	//std::cout<< count <<std::endl;
}

void save_output_image(char* path)
{
	std::vector<unsigned char> saveImage;
	unsigned error = lodepng::encode(saveImage,image,width,height);
	lodepng::save_file(saveImage,path);
}

void get_specified_point_test()
{
	init("C:\\Users\\Ruby\\Desktop\\final essay\\huashan.png");

	point p = get_point(image,10,20);
	cout<<"point:"<<(int)p.r<<" "<<(int)p.g<<" "<<(int)p.b<<endl;
}

void gray_convert_test()
{
	init("C:\\Users\\Ruby\\Desktop\\final essay\\huashan.png");
	
	for(int i = 0 ; i < height ; i ++)
		for(int j = 0 ;  j < width ; j ++)
		{
			point p= get_point(image,i,j);
			unsigned gray = p.r * 0.3 + p.g * 0.59 + p.b * 0.11;
			point gray_point(gray,gray,gray,p.h);
			set_point(image,i,j,&gray_point);
		}

	save_output_image("C:\\Users\\Ruby\\Desktop\\final essay\\huashan_huidu.png");
}

void smooth_test()
{
	init("C:\\Users\\Ruby\\Desktop\\final essay\\huashan.png");

	for(int i = 1 ; i < height -1 ;i ++)
		for(int j = 1 ; j < width -1 ; j++)
		{
			point* points = get_nine_around_points(image,i,j);
			int r_sum = 0;
			int g_sum = 0;
			int b_sum = 0;
			int h_sum = 0;
			for(int k = 0 ; k  < 9 ; k++)
			{
				r_sum += points[k].r;
				g_sum += points[k].g;
				b_sum += points[k].b;
				h_sum += points[k].h;
			}
			point pt(r_sum/9,g_sum/9,b_sum/9,h_sum/9);
			set_point(image,i,j,&pt);
			delete[] points;
		}

	save_output_image("C:\\Users\\Ruby\\Desktop\\final essay\\huashan_pinghua.png");
}

void sharp_test()
{
	init("C:\\Users\\Ruby\\Desktop\\final essay\\huashan.png");

	for(int i = 0 ; i < height-1 ; i++)
		for(int j = 0; j< width-1 ; j++)
		{
			point current = get_point(image,i,j);
			point left = get_point(image,i+1,j);
			point right = get_point(image,i,j+1);
			int _r = abs((int)(current.r - left.r)) + abs((int)(current.r - right.r));
			int _g = abs((int)(current.g - left.g)) + abs((int)(current.g - right.g));
			int _b = abs((int)(current.b - left.b)) + abs((int)(current.b - right.b));
			int _h = abs((int)(current.h - left.h)) + abs((int)(current.h - right.h));

			double percentage = 0.02;

			_r *= percentage;
			_g *= percentage;
			_b *= percentage;
			_h *= percentage;

			point pt(current.r + _r,current.g+_g,current.b+_b,current.h+_h);
			set_point(image,i,j,&pt);
		}

	save_output_image("C:\\Users\\Ruby\\Desktop\\final essay\\huashan_ruihua.png");
}

int main()
{
	get_specified_point_test();

	gray_convert_test();

	smooth_test();

	sharp_test();

	return 0;
}
