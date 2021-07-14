//Name: Radhika Gupta
//Roll no.: 18MA20035

//C++ programme for Triangulation of polygons in O(n^2)
//Ear clipping method:
//1)Traverse through each vertex of the polygon to check whether it is a an ear or not.
//2)The vertex is ear if the diagonal formed by joining the adjacent vertices 
//does not touches or intersect any non adjacent vertices of the polygon
//3)If the vertex is an ear then join its adjacent vertices, and remove the ear from the polygon
//4)Repeat the above steps for the new polygon
//5)For O(n^2), the input of vertices is taken in circularly double linked list

#include <stdio.h>
#include <iostream>
#include <math.h>
typedef float coordinates[2]; // typedef for an array of x and y coordinates of vertex
typedef struct polygon_vertex
{ //vertex structure
   int index;
   bool ear; // ear=1, if the vertex is an ear
   coordinates coordinate;
   polygon_vertex *next, *prev;
} polygon_vertex;

int num_vertices = 0;        // Total number of polygon vertices.
polygon_vertex *head = NULL; //head of the circularly double linked list

//function to calculate the area of triangle with p,q,r as vertices
float area_of_triangle(coordinates p, coordinates q, coordinates r)
{
   float area_triangle = 0.5 * ((q[0] - p[0]) * (r[1] - p[1]) -
                                (r[0] - p[0]) * (q[1] - p[1]));
   ; //formula of area in coordinate geometery
   return area_triangle;
}

//to check whether vertices p,q,r are collinear by calculating the area bounded p,q,r
bool check_points_collinearity(coordinates p, coordinates q, coordinates r)
{
   //if area bounded is very very less, then p,q,r are collinear
   return (area_of_triangle(p, q, r) <= 0.25) && (area_of_triangle(p, q, r) >= -0.25);
}

//to check whether r lies on the line segment formed by joining p and q
bool point_lie_between(coordinates p, coordinates q, coordinates r)
{
   if (!check_points_collinearity(p, q, r))
      return 0; //return 0 if p,q,r are not collinear

   //if line segment pq is not vertical then x coordinate of r lies in between that of p and q
   if (p[0] != q[0])
      return ((p[0] <= r[0]) && (r[0] <= q[0])) || ((p[0] >= r[0]) && (r[0] >= q[0]));
   else // otherwise y coordinate of r lies in between that of p and q
      return ((p[1] <= r[1]) && (r[1] <= q[1])) || ((p[1] >= r[1]) && (r[1] >= q[1]));
}

//to check if the diagonals pq and rs intersects/touches or not
bool intersecting_diagonals(coordinates p, coordinates q, coordinates r, coordinates s)
{
   //to check if diagonals intersect properly
   //condition for proper intersection is that non of p,q,r,s are collinear
   if (!(check_points_collinearity(p, q, r) || check_points_collinearity(p, q, s) ||
         check_points_collinearity(r, s, p) || check_points_collinearity(r, s, q)))
      //proper intersection will occur when r and s lie on different sides of line segment pq and
      //p and q lie on different sides of line segment rs
      //this will happen when one of r and s lies completely to the left of pq and
      //one of p and q lies completely to the left of rs
      return (!(area_of_triangle(p, q, r) > 0.25) ^ !(area_of_triangle(p, q, s) > 0.25))
       && (!(area_of_triangle(r, s, p) > 0.25) ^ !(area_of_triangle(r, s, q) > 0.25));

   //if diagonals intersect improperly/touches, then either of r,s lies in between the line segments pq
   //and either of p,q lies in between the line segments r,s
   else if (point_lie_between(p, q, r) || point_lie_between(p, q, s) 
            || point_lie_between(r, s, p) || point_lie_between(r, s, q))
      return 1;

   else
      return 0;
}

//returns 1 if and only if pq is a proper internal or external diagonal
//Condition for this that pq should not intersect/touch any of the non adjacent edges
bool check_diagonal(polygon_vertex *p, polygon_vertex *q)
{
   polygon_vertex *r, *r_next;
   r = head;
   do
   { //loop to traverse the whole polygon by considering one edge at a time
      r_next = r->next;
      //excluding the adjacent edges to pq
      if ((r != p) && (r_next != p) && (r != q) && (r_next != q) 
            && intersecting_diagonals(p->coordinate, q->coordinate, r->coordinate, r_next->coordinate))
         return 0; //if pq intersect/touches the (r,r_next)
      r = r->next;
   } while (r != head);
   return 1;
}

//to chech whether the diagonal pq is strictly internal to the
//polygon in the neighborhood of the p
bool check_neighborhood(polygon_vertex *p, polygon_vertex *q)
{
   polygon_vertex *p_prev, *p_next;
   p_next = p->next;
   p_prev = p->prev;

   //if p is a convex vertex that is if the area bounded in neighbourhood is positive
   if (area_of_triangle(p->coordinate, p_next->coordinate, p_prev->coordinate) >= -0.25)
      //then pq is strictly internal in the nieghbourhood if p is convex when q and p_prev are joined
      //and q is convex when p and p_next are joined
      return (area_of_triangle(p->coordinate, q->coordinate, p_prev->coordinate) > 0.25 
               && area_of_triangle(q->coordinate, p->coordinate, p_next->coordinate) > 0.25);

   //otherwise if p is reflex point then the condition is that p is convex when q and p_next are joined
   //and q is convex when p and p_prev are joined
   return !(area_of_triangle(p->coordinate, q->coordinate, p_next->coordinate) >= -0.25 
            && area_of_triangle(q->coordinate, p->coordinate, p_prev->coordinate) >= -0.25);
}

//to make circularly double linked list
polygon_vertex *circular_doubly_linked_list(void)
{
   polygon_vertex *v;
   v = (polygon_vertex *)malloc(1 * sizeof(polygon_vertex));
   if (head)
   {
      v->next = head;
      v->prev = head->prev;
      head->prev = v;
      v->prev->next = v;
   }
   else
   {
      head = v;
      head->next = head->prev = v;
   }
   return v;
}

main()
{
   float x_coordinate, y_coordinate;
   int n;
   polygon_vertex *vertex;
   std::cout << "Enter the number of vertices of polygon: ";
   std::cin >> num_vertices;
   if (num_vertices < 3)
   { //if vertices entered are less than 3
      std::cout << "\nNumber of vertices of polygon cannot be less than three\n";
      exit(1);
   }
   std::cout << "\nEnter coordinates of vertices of polygon: \n";
   for (n = 1; n <= num_vertices; n++)
   {
      std::cin >> x_coordinate >> y_coordinate; //input coordinates and set indices of each vertex
      vertex = circular_doubly_linked_list();
      vertex->index = n - 1;
      vertex->coordinate[0] = x_coordinate;
      vertex->coordinate[1] = y_coordinate;
   }

   polygon_vertex *near1_point, *diag1_point, *check_ear_point, *diag2_point, *near2_point;
   check_ear_point = head;
   //loop to initially check all the vertices if they are ear or not , before triangulation
   do
   {
      diag2_point = check_ear_point->next; //adjacents points of ear point which act as diagonal
      diag1_point = check_ear_point->prev;
      //the vertex is ear if the corresponding diagonal is
      //totally inside the polygon without touching any edges
      check_ear_point->ear = check_neighborhood(diag1_point, diag2_point)    //in the nieghbourhood of prev point
                             && check_neighborhood(diag2_point, diag1_point) //in the nieghbourhood of next point
                             && check_diagonal(diag1_point, diag2_point);    //diagonal do not touch or intersect edges
      check_ear_point = check_ear_point->next;
   } while (check_ear_point != head);

   diag2_point = NULL;
   diag1_point = NULL;
   check_ear_point = NULL;

   for (n = num_vertices; n > 3;)
   { //removing ears untill we are left with 3 vertices
      check_ear_point = head;
      do
      { //loop through all vertices to chech for ear
         if (check_ear_point->ear)
         { //if the vertex is ear
            diag2_point = check_ear_point->next;
            near2_point = diag2_point->next;
            diag1_point = check_ear_point->prev;
            near1_point = diag1_point->prev;
            //printing diagonal indices
            std::cout << "( " << diag1_point->index << "," << diag2_point->index << " ) ";

            //again checking if the diagonal points are ear when the ear vertex is removed
            diag1_point->ear = check_neighborhood(near1_point, diag2_point) 
                                 && check_neighborhood(diag2_point, near1_point) 
                                 && check_diagonal(near1_point, diag2_point);
            diag2_point->ear = check_neighborhood(diag1_point, near2_point) 
                                 && check_neighborhood(near2_point, diag1_point) 
                                 && check_diagonal(diag1_point, near2_point);

            diag1_point->next = diag2_point; //removing ear
            diag2_point->prev = diag1_point;
            head = diag2_point;
            n--;   //reducing the total vertices of polygon
            break; //go to the outer loop to again start with new polygon
         }
         check_ear_point = check_ear_point->next; //if ear not found move to next vertex
      } while (check_ear_point != head);
   }
   std::cout << "\nThe above are the indices of the vertices which make diagonals.\n";
}
