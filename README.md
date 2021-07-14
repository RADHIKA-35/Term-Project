# Term-Project
Fast Polygon Triangulation | Course Project | Guide: Professor Bodhayan Roy   

Seidel Algorithm
Step 1. Decompose the Polygon into Trapezoids

Let S be a set of non-horizontal, non-intersecting line segments of the polygon . The randomized algorithm is used to create the trapezoidal decomposition of the X-Y plane arising due the segments of set S. This is done by taking a random ordering s1 .. sN of the segments in S and adding one segment at a time to incrementally construct the trapezoids. This divides the polygon into trapezoids. The restriction that the segments be non-horizontal is necessary to limit the number of neighbors of any trapezoid. However, no generality is lost due to this assumption as it can be simulated using lexicographic ordering. That is, if two points have the same y-coordinate then the one with larger x-coordinate is considered higher. The number of trapezoids is linear in the number of segments. Seidel proves that if each permutation of s1 .. sN is equally likely then trapezoid formation takes O(n log*n) expected time.

Step 2. Decompose the Trapezoids into Monotone Polygons

A monotone polygon is a polygon whose boundary consists of two y-monotone chains. These polygons are computed from the trapezoidal decomposition by checking whether the two vertices of the original polygon lie on the same side. This is a linear time operation.

Step 3. Triangulate the Monotone Polygons

A monotone polygon can be triangulated in linear time by using a simple greedy algorithm which repeatedly cuts off the convex corners of the polygon. Hence, all the monotone polygons can be triangulated in O(n) time.

Data Structures for Implementation

The trapezoid formation requires a structure where the neighbors of each trapezoid and its neighboring segments can be determined in constant time. Therefore, for every trapezoid, the indices of its neighbors and the segments are stored in its table-entry T. The query-structure Q, used to determine the location of a point, is implemented as described by Seidel. The same Q can be later used for fast point-location queries. Both Q and T are updated as a new segment is added into the existing trapezoid formation.This entails splitting in two the trapezoid(s) in which the endpoints of the segment lie, then traversing along the edge of the segment to merge in any neighboring trapezoids which both share the same left and right edges and also share a horizontal edge. All the monotone polygons are stored in a single linked list with pointers to the first vertex in the list stored in a table
