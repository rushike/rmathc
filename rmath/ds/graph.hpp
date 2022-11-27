//
//  graph.hpp
//  rmathc
//
//  Created by Rushikesh Bangar on 08/10/22.
//

#ifndef graph_hpp
#define graph_hpp

#include<iostream>
#include<vector>
#include <list>

class Graph{
  private :
    std::vector<std::vector<double>> matrix;
  
  public :
    Graph(int size);
    void update_edge(int a, int b, double weight);
    void add_edge(int a, int b, double weight);
    std::string matrix_tostring();
};


#endif /* graph_hpp */
