#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <algorithm>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  int n_restaurants, n_locations;
  std::cin >> n_restaurants;

  while (n_restaurants > 0) {
    std::vector<IK::Point_2> restaurants(n_restaurants);
    for (int restaurant_index = 0; restaurant_index < n_restaurants;
        restaurant_index++) {
      IK::Point_2 restaurant;
      std::cin >> restaurant;
      restaurants[restaurant_index] = restaurant;
    }
    Triangulation t;
    t.insert(restaurants.begin(), restaurants.end());

    std::cin >> n_locations;
    IK::FT squared_distance;
    for (int location_index = 0; location_index < n_locations;
        location_index++){
      IK::Point_2 location;
      std::cin >> location;
      squared_distance =
          CGAL::squared_distance(t.nearest_vertex(location)->point(), location);
      std::cout << CGAL::to_double(squared_distance) << std::endl;
    }
    
    std::cin >> n_restaurants;
  }
}
