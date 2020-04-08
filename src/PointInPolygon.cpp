#define DEGENERATE -2

#include <algorithm>    // std::max
#include <include/PointInPolygon.h>

PointInPolygon::PointInPolygon(PolyLine &curve) : boundary_curve(curve); 

/*
 * Compute oriented intersection number \mod 2 
 * relative to {{point}}.
 *
 */
 bool PointInPolygon::pointInPolygon(Vector2D &point) {

 	const double EPSILON = 0.01;

 	int acc = 0;

 	for (edge : boundary_curve) {
 	
 	}

 }

int PointInPolygon::edgeIntersect(Vector2D &point, Vector2D &ray_direction, Edge &edge) {

	// Imagine an infinite length ray from {{point}} in direction (1, EPSILON), 
	//    \beta(t) = t * <1, EPSILON> + p
	// Then determine if the ray \beta(t) intersects {{edge}} by
	// checking if the endpoints lie on either side of \beta, the
	// ray
	double slope_to_end   = (double)( edge.end.y - point.y ) / (double)( edge.end.x - point.x );
	double slope_to_start = (double)( edge.start.y - point.y ) / (double)( edge.start.x - point.x );
    double slope_of_ray   = (double) ray_direction.y / (double) ray_direction.x;

    double max_slope = std::max(slope_to_start, slope_to_end);
    double min_slope = std::min(slope_to_start, slope_to_end);

    if ( (slope_of_ray > max_slope) || (slope_of_ray < min_slope) ) return 0;

	// Compute the orientation of the intersection by computing
	// the determinate of {{ray_direction}} and oriented edge as a vector. 
	// 
	//            [ ray_direction.x (edge.end.x - edge.start.x) ]
	//    det =   [ ray_direction.y (edge.end.y - edge.start.y) ] 
	//
	double det = ray_direction.x * (edge.end.y - edge.start.y) - ray_direction.y * (edge.end.x - edge.start.x);

	if (det > 0 ) return +1;
	if (det < 0 ) return -1;
	
	// what's left is the degenerate case, when the ray \beta 
	// contains the segment {{edge}}. Handle by perturbing  
	// \beta by some epsilon.
	return DEGENERATE;
}
