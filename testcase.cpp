/* 
 * File:   testcase.cpp
 * Author: cantaro86
 *
 * Created on 14 gennaio 2019, 11.25
 */


#include "base.hpp"


/* output for the following function should be (the requirements does not define which is the first point) 
polygon
-0.200000,-0.500000,1.000000
0.000000,0.000000,1.000000
-0.200000,0.500000,1.000000
-0.500000,0.500000,1.000000
-0.500000,-0.500000,1.000000
*/



int test_case()
{
	Pyramid pyramid;
	pyramid.ApexPoint.Set(0, 0, 2);
	pyramid.BasePoints.AddPoint(Vector(-1, -1, 0));
	pyramid.BasePoints.AddPoint(Vector(+1, -1, 0));
	pyramid.BasePoints.AddPoint(Vector(+1, +1, 0));
	pyramid.BasePoints.AddPoint(Vector(-1, +1, 0));

	if ( !pyramid.BasePoints.isConvex() )
	  {
	    std::cout << "The base of the Pyramid is not convex" << std::endl;
	    return 1;
	  }
	
	Polygon triangle;
	triangle.AddPoint(Vector(-2, -5, 1));
	triangle.AddPoint(Vector( 0,  0, 1));
       	triangle.AddPoint(Vector(-2, +5, 1));
	

	if ( !triangle.isConvex() )
	  {
	    std::cout << "The triangle is not convex" << std::endl;
	    return 1;
	  }

	// check that vertexes are enough 
	if ( (pyramid.BasePoints.NumOfPoints<3) || (triangle.NumOfPoints<3) )
	  {
	    std::cout << "Not enough vertexes" << std::endl;
	    return 1;
	  }

	Polygon poylgon = pyramid.GetAreaInsidePyramid(triangle);
	poylgon.Print();

	return 0;
}


int main()
{
  return test_case();  // it returns 1 in case of EXIT_FAILURE
}
