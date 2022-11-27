//
//  graph.cpp
//  rmathc
//
//  Created by Rushikesh Bangar on 08/10/22.
//

#include "graph.hpp"

using namespace std;


string Graph::matrix_tostring() {
  string s = "[";
  for(int i = 0; i < this->matrix.size(); i++) {
    s += "[ ";
    for(int j = 0; j < this->matrix[i].size(); j++) {
      s += ::to_string(this->matrix[i][j]) + ", ";
    }
    s += " ]\n";
  }s += "]";
  return s;
}
  
void Graph::update_edge(int a, int b, double weight) {
  this->matrix[a][b] = weight;
}

void Graph::add_edge(int a, int b, double weight){
  matrix[a][b] = weight;
}

int main() {
  Graph graph(5);
  graph.update_edge(1, 2, 12);
  cout << graph.matrix_tostring()<<endl;
  return 1;
}
