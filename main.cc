#include "route_select.h"

int main(int argc, char **argv)
{

  RouteSelect *rs = new RouteSelect();
  
  // ld->printNeighbors();
  rs->searchSln(rs->getRootNode()[0]);
  rs->printBest();

  delete rs;
  return 0;

}
