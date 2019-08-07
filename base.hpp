/* 
 * File:   base.hpp
 * Author: cantaro86
 *
 * Created on 14 gennaio 2019, 11.22
 */


#ifndef TEST_HPP_INCLUDED
#define TEST_HPP_INCLUDED

#include <math.h>
#include <stdio.h>

#include<iostream>
#include<vector>
#include<map>

// vector in R3
class Vector
{
public:

	double X;
	double Y; 
	double Z;

	//constructor for uninitialized object 
	Vector() {}
	//create vector and set values
	Vector(double x, double y, double z) : X(x), Y(y), Z(z) {}
	//set values
	void Set(double x, double y, double z) { X=x; Y=y; Z=z; }
	//set values to zero 
	void Reset() { X=0.0; Y=0.0; Z=0.0; }
	//add vector
	Vector operator+(const Vector& v) const { return(Vector(X+v.X, Y+v.Y, Z+v.Z)); }
	//sub vector
	Vector operator-(const Vector& v) const { return(Vector(X-v.X, Y-v.Y, Z-v.Z)); }
	//get scaled vector
	Vector operator*(double f) const { return(Vector(X*f, Y*f, Z*f)); }
	//get scalar product
	double operator^(const Vector& v) const { return(X*v.X + Y*v.Y + Z*v.Z); }
	//get vector with length 1 or NULL_Vector
	Vector GetNormalized() const; 
	//get vector length
	double GetLength() const;
	//square of vector length 
	double GetSqrLength() const { return(X*X + Y*Y + Z*Z); }
	// compare two vectors
	bool operator==(const Vector& v) const
		{
		  if ( (fabs(v.X-X)<1e-10) && (fabs(v.Y-Y)<1e-10) && (fabs(v.Z-Z)<1e-10) )
		    return true;
		  else
		    return false;
		}
};



// get the cross product of 2 vectors
Vector getCrossProduct(const Vector& v1, const Vector& v2); 



// plane in R3
class Plane
{
public:
	Vector Normal;
	double Distance;
	
	//set plane with non-valid parameters
	Plane() { Normal.Reset(); Distance=0; }
	// create plane with point and vector describing the normal direction (does not has to be normalized)
	Plane(const Vector& point, const Vector& norm) { Set(point, norm); }
	// create plane with 3 points (on the plane)
	Plane(const Vector& p1, const Vector& p2, const Vector& p3) { Set(p1, p2, p3); }
	// create plane with point and vector describing the normal direction (does not has to be normalized)
	void Set(const Vector& point, const Vector& norm);
	// create plane with 3 points
	void Set(const Vector& p1, const Vector& p2, const Vector& p3); 
	//get distance for a point from the plane. depending on which side point is, the number will be positive or negative
	double GetDistance(const Vector& _P) const { return((Normal^_P) + Distance); }
};                                    




// Polygon with a variable number of points
// all points should be on the same plane : GetPlane().GetDistance(Points[i]) < epsilon 
class Polygon
{
public:
	enum { MaxNumOfPoints = 10 };

	Vector Points[MaxNumOfPoints];
	int NumOfPoints;
  
	// init empty polygon
	Polygon();
	// add point to polygon
	void AddPoint(const Vector& p); 
	// get plane equation
	Plane GetPlane() const;
	// print values
        void Print();
  
	std::vector<Vector> orderedPoints;

        // populate the orderedPoints in clockwise order 
        void ordering();

        // print the ordered points  
        void Print_o();

        // check if the Polygon is convex
	bool isConvex();
};



// n sided pyramid
// the BasePonits describe a convex polygon. 
class Pyramid
{
public:
	Vector ApexPoint;
	Polygon BasePoints; 

	// init without any baseponit set
	Pyramid() { ApexPoint.Reset(); }
  
	// get a polygon that describes the area of the polygon that is inside the pyramid
	// the input polygon has to be convex. ( I included an appropriate check) 
	// if there is no intersection, then a empty polygon is returned
	// if the polygon is completly inside the pyramid, the orignal polygon is returned
	Polygon GetAreaInsidePyramid(const Polygon& polygon) const;

	// Finds the intersection between the plane containing the triangle (any polygon) and the pyramid
	Polygon intersect_Pyr_Plane(const Polygon& triangle) const;
	
};


// Finds the intersection line-plane, and add the point (0,1 or 2) into orderedPoly
void intersect_Line_Plane(Polygon& orderedPoly, const Plane& plane, const Vector& p1, const Vector& p2);

// This function finds the intersection of two lines on the same plane
Vector intersect_Pt_Pt(const Vector& A, const Vector& B, const Vector& p1, const Vector& p2);

#endif

