#include "route_select.h"

int main(int argc, char **argv)
{

  RouteSelect *rs = new RouteSelect();
  
  // ld->printNeighbors();
  rs->searchSln(rs->getRootNode()[0]);
  rs->printBest();

  delete rs;
  
  Dijkstra *dk = new Dijkstra();
  
  dk->searchSln(dk->getRoot());
  dk->printBest();
  
  delete dk;
  
  return 0;

}
