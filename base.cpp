/* 
 * File:   base.cpp
 * Author: cantaro86
 *
 * Created on 14 gennaio 2019, 11.22
 */


#include "base.hpp"

Vector Vector::GetNormalized() const 
{ 
  double l = GetLength(); 
  if (l>0) 
    return(*this * (1.0 / l)); 
  else 
    return (Vector(0.0, 0.0, 0.0)); 
}

double Vector::GetLength() const 
{ 
  return(sqrt(GetSqrLength())); 
}

Vector getCrossProduct(const Vector& v1, const Vector& v2) 
{ 
  return Vector(v1.Y*v2.Z - v1.Z*v2.Y, v1.Z*v2.X - v1.X*v2.Z, v1.X*v2.Y - v1.Y*v2.X); 
}

void Plane::Set(const Vector& point, const Vector& norm) 
{ 
  Normal=norm.GetNormalized(); 
  Distance=-(Normal^point); 
}

void Plane::Set(const Vector& p1, const Vector& p2, const Vector& p3) 
{ 
  Normal= getCrossProduct(p2 - p1 , p3 - p2).GetNormalized();

  // Set an orientation for the plane

  if (Normal.Z < 0)
    Normal = Normal * (-1);
  if (Normal.Z == 0 && Normal.X <0)
    Normal = Normal * (-1);
      
  Distance=-(Normal^p1); 	
}



Polygon::Polygon() 
{ 
  NumOfPoints = 0; 
}

void Polygon::AddPoint(const Vector& p) 
{

  // Checks if the second point is not equal to the first
  if (NumOfPoints==1 && (p == Points[0]) )
    {
    // printf("The point: %f,%f,%f has already been chosen \n", p.X,p.Y,p.Z);
      return;
    }

  // Checks if the inserted point lies on an edge
  if (NumOfPoints >= 2)
    {
      unsigned int end = orderedPoints.size();
      if ( getCrossProduct( (p-orderedPoints[end-1]), (orderedPoints[end-1]-orderedPoints[0]) ).GetSqrLength() < 1e-10)
	{
	  // printf("The point: %f,%f,%f lies on an edge \n", p.X,p.Y,p.Z);
	  return;
	}
    for (unsigned int i=1; i<end; i++)
      {
	if ( getCrossProduct( (p-orderedPoints[i-1]), (orderedPoints[i-1]-orderedPoints[i]) ).GetSqrLength() < 1e-10)
	  {
	    // printf("The point: %f,%f,%f lies on an edge \n", p.X,p.Y,p.Z);
	    return;
	  }											   
      }    
  }
  
  // Checks if the point is not on the plane
  if (NumOfPoints>=3 && fabs(GetPlane().GetDistance(p)) > 1e-10)
    {
      // printf("The point: %f,%f,%f is not on the same plane. The distance is %f \n", p.X,p.Y,p.Z,GetPlane().GetDistance(p));
      return;
    }
  
  if (NumOfPoints<MaxNumOfPoints) 
    {
      Points[NumOfPoints++] = p;
      ordering();
    }
  else
    printf("Reached the max number of points: %d \n", NumOfPoints);
}


Plane Polygon::GetPlane() const
{
  Plane plane;

  if (NumOfPoints >= 3)
    plane.Set(Points[0], Points[1], Points[2]);

  return(plane);
}

void Polygon::Print()
{
  printf("polygon\r\n");
  for (int i=0; i<NumOfPoints; i++)
    printf("%f,%f,%f\r\n", Points[i].X, Points[i].Y, Points[i].Z);
}

void Polygon::Print_o()
{
  printf("polygon\r\n");
  for (unsigned int i=0; i<orderedPoints.size(); i++)
    printf("%f,%f,%f\r\n", orderedPoints[i].X, orderedPoints[i].Y, orderedPoints[i].Z);
}



// Clockwise ordering
void Polygon::ordering()
{
  orderedPoints.clear();
  
  double a=0;
  double b=0;
  double c=0;
  for (int i=0; i<NumOfPoints; i++)
    {
      a += Points[i].X;
      b += Points[i].Y;
      c += Points[i].Z;
    }
  Vector baricenter(a/NumOfPoints,b/NumOfPoints,c/NumOfPoints);
  
  std::map <double, int> angles;
  Vector ref_ray = Points[0] - baricenter;
  Vector normal = GetPlane().Normal;
  double prod;
  
  for (int i=0; i<NumOfPoints; i++)
    {
      prod = getCrossProduct(ref_ray, (Points[i]-baricenter)) ^ normal;
      angles[ atan2(  prod, (Points[i]-baricenter)^ref_ray ) ] = i;
    }

  for (std::map<double, int>::iterator it = angles.begin(); it != angles.end(); it++)
    {
      orderedPoints.insert(orderedPoints.begin(), Points[it->second]);
      // orderedPoints.push_back(Points[it->second]);    //  for the opposite order 
    }
}

// Check if the polygon is convex
bool Polygon::isConvex()
{
  Vector normal = GetPlane().Normal;
  Vector vec0 = (orderedPoints[0] - orderedPoints[orderedPoints.size()-1]);
  Vector vec1 = vec0;
  Vector vec2;
  for (unsigned int i=1; i<orderedPoints.size(); i++)
      {
	vec2 = (orderedPoints[i]-orderedPoints[i-1]);
	if ( (getCrossProduct(vec1, vec2)^normal) > 0)
	  return false;
	vec1 = vec2;
	if ( (i==orderedPoints.size()-1) && (getCrossProduct(vec2, vec0)^normal) > 0 )
	  return false;
      }
  return true;
}




Polygon Pyramid::GetAreaInsidePyramid(const Polygon& triangle) const
{
  Polygon result;
  Polygon intersected_Poly = intersect_Pyr_Plane(triangle);

  Vector normal = triangle.GetPlane().Normal;
  
  unsigned int size_py = intersected_Poly.orderedPoints.size();
  unsigned int size_tr = triangle.orderedPoints.size();

  if (size_py == 0)
    return intersected_Poly;
  
  std::vector<Vector> base_Vec;  
  for (unsigned int i=0; i<size_py; i++)
    base_Vec.push_back( intersected_Poly.orderedPoints[i] );
  base_Vec.push_back( intersected_Poly.orderedPoints[0] );
  
  std::vector<Vector> triangle_Vec;
  for (unsigned int j=0; j<size_tr; j++)
    triangle_Vec.push_back( triangle.orderedPoints[j] );
  triangle_Vec.push_back( triangle.orderedPoints[0] );    // Add the first point in the last position
    
  if (size_py == 1)
    {
      bool is_inside = true;
      for (unsigned int i=0; i<size_tr; i++)
	{
	  if ( ( getCrossProduct( triangle_Vec[i+1] - triangle_Vec[i], base_Vec[0] - triangle_Vec[i] )^normal ) > 0 )
	    is_inside = false;
	}
      if (is_inside)
	return intersected_Poly;
      else
	return result;
    }
  
  // find intersection using the Sutherland–Hodgman Algorithm

  bool v1_in;
  bool v2_in;

  std::vector<Vector> triangle_temp;
    
  for (unsigned int i=0; i<size_py; i++) 
    {
      for (unsigned int j=0; j<triangle_Vec.size()-1; j++)
	{
	  v1_in = ( getCrossProduct( base_Vec[i+1] - base_Vec[i], triangle_Vec[j] - base_Vec[i] )^normal ) <= 0;
	  v2_in = ( getCrossProduct( base_Vec[i+1] - base_Vec[i], triangle_Vec[j+1] - base_Vec[i] )^normal ) <= 0;
	  
	  if (v1_in==true && v2_in==true)
	    {
	    triangle_temp.push_back( triangle_Vec[j+1] );
	    }
	  else if (v1_in==false && v2_in==true)
	    {
	      triangle_temp.push_back( intersect_Pt_Pt( triangle_Vec[j],triangle_Vec[j+1], base_Vec[i],base_Vec[i+1] ) );
	      triangle_temp.push_back( triangle_Vec[j+1] );
	    }
	  else if (v1_in==true && v2_in==false)
	    {
	      triangle_temp.push_back( intersect_Pt_Pt( triangle_Vec[j],triangle_Vec[j+1], base_Vec[i],base_Vec[i+1] ) );
	    }
	  else
	    {
	      // do nothing
	    }
	}
      triangle_Vec = triangle_temp;
      triangle_Vec.push_back(triangle_temp[0]);
      triangle_temp.clear();
    }
  for (unsigned int j=0; j<triangle_Vec.size()-1; j++)
    result.AddPoint(triangle_Vec[j]);
    
  return result;
}




Polygon Pyramid::intersect_Pyr_Plane(const Polygon& triangle) const
{
  Polygon intersected_Poly;
  Plane plane = triangle.GetPlane();
  
  for (unsigned int i=0; i<BasePoints.orderedPoints.size(); i++)
    {
      intersect_Line_Plane(intersected_Poly, plane, BasePoints.orderedPoints[i], ApexPoint);
    }
  for (unsigned int i=0; i<BasePoints.orderedPoints.size()-1; i++)
    {
      intersect_Line_Plane(intersected_Poly, plane, BasePoints.orderedPoints[i], BasePoints.orderedPoints[i+1]);
    }
  intersect_Line_Plane(intersected_Poly, plane, BasePoints.orderedPoints[BasePoints.orderedPoints.size()-1], BasePoints.orderedPoints[0]);

  return intersected_Poly;
}



void intersect_Line_Plane(Polygon& orderedPoly, const Plane& plane, const Vector& p1, const Vector& p2)
{

  Vector n = plane.Normal;
  double d = -1 * plane.Distance;

  Vector p2p1 = p2 - p1;
  double nDotp1 = n^p1;
  double nDotp2p1 = n^p2p1;
    
  if(nDotp2p1==0)  // line and plane are parallel
    {
      if( (d-nDotp1) == 0)  // the line is on the plane
	{
	  orderedPoly.AddPoint(p1);
	  orderedPoly.AddPoint(p2);
	}
      return;
    }
  else 
    {
      double coeff = (d-nDotp1)/nDotp2p1;
      if (coeff < 0 || coeff > 1)  // the plane intersect the line out of the segment p1-p2
	return;
      
      Vector I = p1 +  ( p2p1 * coeff );
      orderedPoly.AddPoint(I);
      return;
    }
}



Vector intersect_Pt_Pt(const Vector& A, const Vector& B, const Vector& p1, const Vector& p2)
{

  Vector normal = getCrossProduct(p2-p1,B-A);
  Plane plane(A,B,normal);

  Vector n = plane.Normal;
  double d = -1 * plane.Distance;
  Vector p2p1 = p2 - p1;
  double nDotp1 = n^p1;
  double nDotp2p1 = n^p2p1;
  
  double coeff = (d-nDotp1)/nDotp2p1;
  Vector I = p1 +  ( p2p1 * coeff );
  
  return I;
}
