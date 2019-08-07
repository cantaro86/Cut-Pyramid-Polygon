Determining the cut of any given polygon and a pyramid 
==========================

This is a tri-dimensional geometry problem I solved in C++ for an exam.   
It uses the **Sutherland–Hodgman** algorithm to find the intersection between two polygons.


1)  Problem description

Determine the cut of any given polygon (2 dimensional) and a pyramid (3 di-
mensional).
The polygon and the base of the pyramid are assumed to have more than 2
vertexes and are convex.
The pyramid is assumed to be a filled solid. If the intersection is empty, then
returns the empty polygon. If the polygon is inside the pyramid, then returns
the original polygon.

2)  Algorithm

The algorithm to solve the problem is the following:   

 A) Determine the plane containing the polygon A.  
 B) Determine the intersection of the plane with the pyramid. The result is another convex polygon B.  
 C) Determine the intersection between A and B.  

- A  
In order to get the plane it is enough to use the function `GetPlane`. Here it is important to have previously checked the quality of the vertexes (they should not be aligned and should form a convex polygon). The algorithm checks also if the points belong to the same plane, however this is not relevant for the use of GetPlane since it only uses the first 3 points.  
Another important thing is to select an orientation for the plane, i.e. if we want to look the plane from above or from below (or left or right if it is vertical). This feature is implemented in the function `Plane::Set`. Having an orientation is important when ordering the points clockwise with the function `ordering`.
The function `ordering` sorts the points in clockwise order. First it computes the barycenter and the segments connecting it with each points. Then it sorts all the segments using the angle between each segment and a reference segment. The angle is obtained using the arctan function and the properties of the cross and
scalar products.
- B  
The intersection between the plane and the pyramid is computed by the function `Pyramid::intersect_Pyr_Plane`. It basically computes the line-plane intersection between the plane and each edge of the pyramid.
- C  
I decided to compute the intersection between the initial polygon A and the polygon B obtained in step 2, by using the **Sutherland–Hodgman** algorithm.
The function `Pyramid::GetAreaInsidePyramid` first considers some particular cases, i.e. when the final output is the empty polygon or a single point polygon, and then implements the Sutherland–Hodgman method. 

#### Instructions:
In the `testcase.cpp` file the Pyramid and Polygon objects are created. You can add more points if you wish.
The output of the program represents the vertexes of the polygon resulting from the intersection.

To compile the program just run `make` in the shell.

#### Comments:
The code is not well written but it works correctly. It uses both C-style vectors and STL vectors. It returns objects by value and not by reference. In a future implementation I suggest to return smart pointers owning the Vector objects. Another improvement is to use the std::map instead of a std::vector in order to keep the elements (Points) ordered. The function `ordering` is not efficient at all.        
