#ifndef ROUTE_SELECT_H_
#define ROUTE_SELECT_H_

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <glog/logging.h>

#define INI_MAX 10e100

typedef struct Node
{
  std::string name;
  std::map<std::string, std::pair<Node*, float> > parents;
  std::map<std::string, std::pair<Node*, float> > children;
}Node;
typedef std::map<std::string, std::pair<Node*, float> > Neighbor;
typedef std::map<std::string, Node*> NodeMap;

class RouteSelect
{
public:
  RouteSelect(void);
  ~RouteSelect(void);
  void printNeighbor(const Node *);
  void printNeighbors(void);
  void searchSln(const Node *);
  void printSln(const std::vector<Node*> &);
  void printBest(void);
  std::vector<Node*> getRootNode(void);
  Node *getNode(const std::string &);

private:
  void init(void);
  void interFeed(void);
  void setNode(const std::string &, const std::string &, const float &);
  void setNeighbor(Node *, Node *, const float &);
  float calDis(std::vector<Node*> &sln);

  NodeMap m_Node;
  float bestAns;
  std::vector<Node*> bestSln, curSln;
};

#endif // ! LAMIA_DEFINITION_H_
