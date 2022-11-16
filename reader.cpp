#include <set>
#include <map>
#include <algorithm>  // for set_intersection, set_union
#include <functional> // for less
#include "reader.h"

// prototype of bison-generated parser function
int yyparse();

StringList Reader::name(Literals *x)
{
  StringList y;
  Literals::iterator it;

  for (it = x->begin(); it != x->end(); it++)
  {
    y.insert(name(*it));
  }

  return y;
}

string Reader::name(Literal x)
{
  unsigned short i = x / 2;
  StringList::iterator it;

  if (i >= m_fluents.size())
    return NULL;

  it = m_fluents.begin();

  for (i = 0; i < x / 2; i++)
  {
    it++;
  }

  if (x % 2 == 0)
    return *it;

  return (NEGATION_SYMBOL + (*it));
}

int Reader::read()
{
  return yyparse();
}

void Reader::print() const
{
  StringList::const_iterator it;
  StringList2::const_iterator it2;
  PropositionList::const_iterator it3;
  Actions::const_iterator it4;
  unsigned int i;

  cout << "FLUENT DECLARATION" << endl;
  cout << "----------------------------" << endl;
  cout << "Total fluents: " << m_fluents.size() << endl;
  for (it = m_fluents.begin(); it != m_fluents.end(); it++)
  {
    cout << "\t>>>> " << *it << endl;
  }

  cout << endl;
  cout << endl;

  cout << "ACTION DECLARATION" << endl;
  cout << "----------------------------" << endl;
  cout << "Total actions: " << m_actions.size() << endl;
  for (it4 = m_actions.begin(); it4 != m_actions.end(); it4++)
  {
    it4->print();
    cout << endl;
  }

  cout << endl;
  cout << endl;

  cout << "AGENT DECLARATION" << endl;
  cout << "---------------------------" << endl;
  cout << "Total agents: " << m_agents.size() << endl;
  for (it = m_agents.begin(); it != m_agents.end(); it++)
  {
    cout << "\t>>>> " << *it << endl;
  }
  cout << endl;
  cout << endl;

  cout << "PROPOSITIONS" << endl;
  cout << "----------------------------" << endl;
  for (it3 = m_propositions.begin(); it3 != m_propositions.end(); it3++)
  {
    it3->print();
    cout << endl;
  }

  // print init cstate
  Nodes::const_iterator it5;
  cout << "INIT" << endl;
  cout << "----------------------------" << endl;
  for (it5 = k_init.begin(); it5 != k_init.end(); it5++)
  {
    it5->print();

    cout << endl;
  }

  cout << endl;

  // print goal state
  cout << "GOAL " << endl;
  cout << "----------------------------" << endl;
  for (it5 = m_gd.begin(); it5 != m_gd.end(); it5++)
  {
    it5->print();
    cout << endl;
  }

}
