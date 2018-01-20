#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <map>
#include <vector>

// #include "prettyprint.hpp"

// CGAL triangulation
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// CGAL LP
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void test_case() {
  int sherlock_price, req_where, req_who, req_how, n_agents, n_members;
  std::cin >> sherlock_price >> req_where >> req_who >> req_how >> n_agents
      >> n_members;
  std::vector<K::Point_2> member_coordinates(n_members);
  std::map<K::Point_2, int> coordinates_member;
  std::vector<int> where_rate(n_members);
  std::vector<int> who_rate(n_members);
  std::vector<int> how_rate(n_members);
  for (int member_index = 0; member_index < n_members; member_index++) {
    K::Point_2 point;
    std::cin >> point >> where_rate[member_index] >> who_rate[member_index] >>
        how_rate[member_index];
    member_coordinates[member_index] = point;
    coordinates_member[point] = member_index;
  }

  Triangulation t;
  t.insert(member_coordinates.begin(), member_coordinates.end());

  Program lp (CGAL::LARGER, true, 0, true, 24);
  lp.set_b(0, req_where);
  lp.set_b(1, req_who);
  lp.set_b(2, req_how);

  std::vector<std::pair<int, int> > hourly_wages(n_agents);
  std::vector<int> closest_member(n_agents);
  for (int agent_index = 0; agent_index < n_agents; agent_index++) {
    K::Point_2 point;
    int hourly_wage;
    std::cin >> point >> hourly_wage;
    K::Point_2 nearest_member_coordinates = t.nearest_vertex(point)->point();
    int member_index = coordinates_member[nearest_member_coordinates];
    hourly_wages[agent_index] = std::make_pair(agent_index, hourly_wage);
    closest_member[agent_index] = member_index;
  }

  std::sort(hourly_wages.begin(), hourly_wages.end(),
      [](std::pair<int, int> a, std::pair<int, int> b) {
        return a.second < b.second;
      });

  std::vector<bool> member_visited(n_members, false);
  int unvisited_member_index = 0;
  for (int hourly_wage_index = 0; hourly_wage_index < n_agents;
      hourly_wage_index++) {
    int agent_index = hourly_wages[hourly_wage_index].first;
    int member_index = closest_member[agent_index];
    if (!member_visited[member_index]) {
      member_visited[member_index] = true;
      lp.set_a(unvisited_member_index, 0, where_rate[member_index]);
      lp.set_a(unvisited_member_index, 1, who_rate[member_index]);
      lp.set_a(unvisited_member_index, 2, how_rate[member_index]);
      lp.set_c(unvisited_member_index, hourly_wages[hourly_wage_index].second);
      unvisited_member_index++;
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  assert (s.solves_linear_program(lp));

  if (s.is_infeasible() or (s.objective_value() > sherlock_price)) {
    std::cout << "H" << std::endl;
  } else {
    std::cout << "L" << std::endl;
  }
}

int main() {
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
