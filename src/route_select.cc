#include "route_select.h"

#ifdef ROUTE_SELECT_H_

RouteSelect::RouteSelect(void)
{
  bestAns = INI_MAX;
  init();  
  // interFeed();
}

RouteSelect::~RouteSelect(void)
{
  bestSln.clear();
  curSln.clear();
  for(auto it=m_Node.begin();it!=m_Node.end();++it)
    delete it->second;
}

void RouteSelect::setNode(const std::string &parent, const std::string &child, const float &dist)
{
  auto it = m_Node.find(parent);
  if(it == m_Node.end())
  {
    m_Node[parent] = new Node;
    m_Node[parent]->name = parent;
  }
  it = m_Node.find(child);
  if(it == m_Node.end())
  {
    m_Node[child] = new Node;
    m_Node[child]->name = child;
  }
  setNeighbor(m_Node[parent], m_Node[child], dist);
}

void RouteSelect::init(void)
{
  setNode("A" , "B1", 5); setNode("A" , "B2", 3);
  setNode("B1", "C1", 1); setNode("B1", "C2", 3); setNode("B1", "C3", 6);
  setNode("B2", "C2", 8); setNode("B2", "C3", 7); setNode("B2", "C4", 6);
  setNode("C1", "D1", 6); setNode("C1", "D2", 8);
  setNode("C2", "D1", 3); setNode("C2", "D2", 5);
  setNode("C3", "D2", 3); setNode("C3", "D3", 3);
  setNode("C4", "D2", 8); setNode("C4", "D3", 4);
  setNode("D1", "E" , 3);
  setNode("D2", "E" , 2);
  setNode("D3", "E" , 2);
}

void RouteSelect::interFeed(void)
{
  while(true)
  {
    std::string parent, child;
    float dist;
    LOG(INFO) << "Info: feed parent and child with its distance";
    fprintf(stderr, "praent name: ");
    std::cin >> parent;
    fprintf(stderr, "child name: ");
    std::cin >> child;
    fprintf(stderr, "its distance: ");
    std::cin >> dist;
    if(dist < 0)
    {
      LOG(INFO) << "dsitance could not less than 0, please re-feed";
      continue;
    }
    else if(dist == 0)
    {
      LOG(INFO) << "distance equal to 0, feed done";
      break;
    }
    LOG(INFO) << "successfully accept node";
    setNode(parent, child, dist);
  }
}

void RouteSelect::setNeighbor(Node *parent, Node *child, const float &dist)
{
  parent->children[child->name] = std::make_pair(child, dist);
  child->parents[parent->name] = std::make_pair(parent, dist);
}

void RouteSelect::printNeighbor(const Node *node)
{
  LOG(INFO) << node->name;
  LOG(INFO) <<"has " << node->parents.size() << " parents: ";
  for(auto it=node->parents.begin();it!=node->parents.end();++it)
    LOG(INFO) << it->first << ", " << it->second.second;
  LOG(INFO) << "has " << node->children.size() << " children: ";
  for(auto it=node->children.begin();it!=node->children.end();++it)
    LOG(INFO) << it->first << ", " << it->second.second;
}

void RouteSelect::printNeighbors(void)
{
  for(auto it=m_Node.begin();it!=m_Node.end();++it)
    printNeighbor(it->second);
}

std::vector<Node*> RouteSelect::getRootNode(void)
{
  std::vector<Node*> root;
  for(auto it=m_Node.begin();it!=m_Node.end();++it)
  {
    if(it->second->parents.size())
      continue;
    root.push_back(it->second);
  }
  return root;
}

Node *RouteSelect::getNode(const std::string &name)
{
  return m_Node[name];
}

void RouteSelect::searchSln(const Node *node)
{ 
  curSln.push_back(const_cast<Node*>(node));
  if(!node->children.size())
  {
    printSln(curSln);
    LOG(INFO) << "current ansower: " << calDis(curSln) << " bestAns: " << bestAns;
    if(calDis(curSln) < bestAns)
    {
      LOG(INFO) << "swap...";
      bestAns = calDis(curSln);
      bestSln = curSln;
    }
    LOG(INFO) << "no children... lenth: " << curSln.size();
    if(curSln.size())
      curSln.pop_back();
    LOG(INFO) << "at " << curSln.back()->name << ", leaving searching...";
    return;
  }

  LOG(INFO) << "recursive... " << node->name;
  for(auto it=node->children.begin();it!=node->children.end();++it)
  {
    auto nextNode = it->second.first;
    LOG(INFO) << "just loop for " << node->name 
              << " which has " << node->children.size() << " children"
              << ", now looping for " << nextNode->name
              << " which has " << nextNode->children.size() << " children";
    searchSln(nextNode);
  }
  curSln.pop_back();
}

void RouteSelect::printSln(const std::vector<Node*> &sln)
{
  for(int i=0;i<sln.size();++i)
  {
    if(i<sln.size() -1)
      fprintf(stderr, "%2s -> ", sln[i]->name.c_str());
    else
      fprintf(stderr, "%2s\n", sln[i]->name.c_str());
  }
}

void RouteSelect::printBest(void)
{
  LOG(INFO) << "best score: " << bestAns << "\tbest solution: ";
  printSln(bestSln);
}

float RouteSelect::calDis(std::vector<Node*> &sln)
{
  float sum = 0;
  for(int i=0;i<sln.size()-1;++i)
    sum += sln[i]->children[sln[i+1]->name].second;
  return sum;    
}

#endif // ! LAMIA_DEFINITION_H_
