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


//*********************************************************************
//
//  Dijkstra
//
//*********************************************************************
Dijkstra::Dijkstra(void)
{
  bestScore = INI_MAX;
  m_MinYear = (int)INI_MAX;
  m_MaxYear = 0;
  init();
}


Dijkstra::~Dijkstra(void)
{
  
}

void Dijkstra::init()
{
  setPurchaseFee(1, 11);  setMaintainFee(1, 5);
  setPurchaseFee(2, 11);  setMaintainFee(2, 6);
  setPurchaseFee(3, 12);  setMaintainFee(3, 8);
  setPurchaseFee(4, 12);  setMaintainFee(4, 11);
  setPurchaseFee(5, 13);  setMaintainFee(5, 18);
}

void Dijkstra::setPurchaseFee(int year, float fee)
{
  m_PurchaseFee[year] = fee;
  if(year < m_MinYear)
    m_MinYear = year;
  if(year > m_MaxYear)
    m_MaxYear = year;
}

void Dijkstra::setMaintainFee(int year, float fee)
{
  m_MaintainFee[year] = fee;
  if(year < m_MinYear)
    m_MinYear = year;
  if(year > m_MaxYear)
    m_MaxYear = year;
}

void Dijkstra::searchSln(const int &root)
{
  curSln.push_back(root);
  
  if(root == m_MaxYear)
  {
    LOG(INFO) << "End of this route";
    curScore = calFee(curSln);
    if(curScore < bestScore)
    {
      LOG(INFO) << "swap...";
      m_BestSln.clear();
      bestScore = curScore;
      m_BestSln.push_back(curSln);
    }
    else if(curScore == bestScore)
    {
      LOG(INFO) << "insert...";
      m_BestSln.push_back(curSln);
    }
    printSln(curSln);
    curSln.pop_back();
    return;
  }
    
  for(int i=root;i<m_MaxYear;++i)
  {
    searchSln(i+1);
  }
  
  curSln.pop_back();
  
}

int Dijkstra::getRoot(void)
{
  return m_MinYear;
}

float Dijkstra::calFee(const std::vector<int> &sln)
{
  float sum = 0;
  int i=0;
  for(;i<sln.size()-1;++i)
  {
    int A = sln[i+0];
    int B = sln[i+1];
    // LOG(INFO) << "Add purchase < " << A << ", " << m_PurchaseFee[A] << ">";
    sum += m_PurchaseFee[A];
    for(int j=A;j<B;++j)
    {
      // LOG(INFO) << "Add maintain < " << j-A+1 << ", " << m_MaintainFee[j-A+1] << ">";
      sum += m_MaintainFee[j-A+1];
    }
  }
  // LOG(INFO) << "Add maintain < " << m_MaxYear-sln[i-1]+1 << ", " << m_MaintainFee[m_MaxYear-sln[i-1]+1] << ">";
  sum += m_MaintainFee[m_MaxYear-sln[i-1]+1];
  return sum;
}

void Dijkstra::printSln(const std::vector<int> &sln)
{
  fprintf(stderr, "fee: %f\tsolution: ", calFee(sln));
  for(int i=0;i<sln.size();++i)
  {
    if(i<sln.size() -1)
      fprintf(stderr, "%2d -> ", sln[i]);
    else
      fprintf(stderr, "%2d\n", sln[i]);
  }
}

void Dijkstra::printBest(void)
{
  LOG(INFO) << "best score: " << bestScore << "\tbest solution: ";
  for(int i=0;i<m_BestSln.size();++i)
    printSln(m_BestSln[i]);
  
  /*
  LOG(INFO) << "Info: purchase fee: ";
  for(auto it=m_PurchaseFee.begin();it!=m_PurchaseFee.end();++it)
    LOG(INFO) << "<" << it->first << ", " << it->second << ">";
  LOG(INFO) << "Info: maintain fee: ";
  for(auto it=m_MaintainFee.begin();it!=m_MaintainFee.end();++it)
    LOG(INFO) << "<" << it->first << ", " << it->second << ">";
  */
}














#endif // ! LAMIA_DEFINITION_H_
