#include "kstate.h"
#include "define.h"
#include "kedge.h"
#include "kripke.h"
#include <map>
#include <list>

/*********************************************************************
 Kstate implementation
**********************************************************************/

Kstate::Kstate()
{
    // fromState = -1;
    // fromEvent = -1;
    edge_out = EdgeList();
    edge_in = EdgeList();
    mapEdgelabelNextState = map<int, set<int> >();
}

Kstate::~Kstate()
{

}

EdgeList Kstate::get_in()
{
	return edge_in;
}

EdgeList Kstate::get_out()
{
	return edge_out;
}

Literals Kstate::get_lits()
{
	return lits;
}

int Kstate::get_id()
{
	return id;
}

const int Kstate::get_id_const() const
{
	return id;
}

bool Kstate::get_exist()
{
	return exists;
}

void Kstate::set_in(EdgeList in)
{
	edge_in = in;
}

void Kstate::set_out(EdgeList out)
{
	edge_out = out;
}

void Kstate::set_lits(Literals ls)
{
	lits = ls;
}

void Kstate::set_id(int ident)
{
	id = ident;
}

void Kstate::set_exist(bool ex)
{
	exists = ex;
}

map<int, set<int> > Kstate::get_preparedMap(){
    return mapEdgelabelNextState;
}

void Kstate::reset_map(){
    mapEdgelabelNextState = map<int, set<int> >();
}

void Kstate::add_to_map(int ag, int des){
    
    set<int> isAdding;
    if(mapEdgelabelNextState.find(ag) != mapEdgelabelNextState.end()){
        isAdding = mapEdgelabelNextState[ag];
    }else{
        isAdding = set<int>();
    }
    isAdding.insert(des);
    mapEdgelabelNextState[ag] = isAdding;
}

bool Kstate::entail_lit(Literal l) const
{
    Literals::const_iterator it;
    it = lits.find(l);
    if(it == lits.end())
    {
        return false;
    }
    return true;
}

bool Kstate::entail_lits(Literals ls) const
{
    if(ls.size()==0) // Literal = [] then return true;
    {
        return true;
    }
    Literals::const_iterator it;
    for(it = ls.begin(); it!= ls.end();it++)
    {
        if(!entail_lit(*it))
        {
            return false;
        }
    }
    return true;
}

bool Kstate::entail_fluform(FluentFormula ff) const
{
    FluentFormula::const_iterator it;
    for(it = ff.begin(); it!= ff.end();it++)
    {
        if(entail_lits(*it))
        {
            return true;
        }
    }
    return false;
}


Kstate* Kstate::copy()
{
	Kstate* k1 = new Kstate();
    k1->set_lits(lits);
    k1->set_id(id); //keep the same id
    k1->set_out(edge_out);
    k1->set_in(edge_in);
    return k1;
}

void Kstate::add_out(int e)
{
	edge_out.push_back(e);
}

void Kstate::add_in(int e)
{
	edge_in.push_back(e);
}

void Kstate::print_info()
{	
	cout << "State " << id << " specs:" << endl;
        //cout << "Does exist: " << exists << endl;
		cout << "--The number of edges out: " << edge_out.size() << endl;
		EdgeList::iterator ite;
		for(ite = edge_out.begin();ite!=edge_out.end();ite++)
		{
			cout << *ite << " ";
		}
		cout << endl;
		cout << "--The number of edges in: " << edge_in.size() << endl;
		for(ite = edge_in.begin();ite!=edge_in.end();ite++)
		{
			cout << *ite << " ";
		}
		cout << endl;
        cout << "--contains literals: ";
	Literals::iterator it;

	
	cout << "Literals contained in state " << id << ":";
	for(it = lits.begin(); it != lits.end(); it++)
		cout << *it << ", ";
	cout << endl;

}

bool Kstate::entail_BF(Kripke k, BFNode node, Agent ag, bool check) const
{
    Kripke temp;
    temp.set_stateslist(k.get_stateslist());
    temp.set_edgeslist(k.get_edgeslist());

    Kstate pState;// = k.get_stateslist().at(id);
    States tempStatesList = k.get_stateslist();
    for (auto itTemp = tempStatesList.begin(); itTemp != tempStatesList.end(); itTemp++){
        if (itTemp->get_id_const() == id){
            pState = *itTemp;
            break;
        }
    }
    EdgeList el = pState.get_out();

    EdgeList::const_iterator it;
    bool exist_label;
    if (check){
        exist_label = false;
    } else {
        exist_label = true;
    }
    map<int, set<int> > k1map = pState.get_preparedMap();
    if (k1map.find(ag) != k1map.end()){
        set<int> forcheck = k1map[ag];
        for (int i : forcheck){
            exist_label = true;
            temp.set_pId(i);
            if (check && !temp.entail(node)){
                return false;
            } else if (!check && !temp.entail1(node)){
                return false;
            }
        }
    }
    if (!exist_label){
        return false;
    }
    return true;
}


bool Kstate::entail(BFNode const & bfNode, Kripke k) const
{
    // cout << "Node type " << bfNode.node_type << endl;
    switch (bfNode.node_type)
    {
        case fluForm:
        {
            return entail_fluform(*bfNode.fluform);
        }

        case BForm:
        {
            Agent ag = bfNode.agentPro2;
            BFNode bf = *bfNode.bfnode1;
            return entail_BF(k, bf, ag, true);
        }
        case propForm:
        {
            if (bfNode.bfOperator == BFNOT){
                return !entail(*bfNode.bfnode1, k);
            } else if (bfNode.bfOperator == NONE){
                return entail(*bfNode.bfnode1, k);
            } else if (bfNode.bfOperator == BFOR){
                return entail(*bfNode.bfnode1, k) || entail(*bfNode.bfnode2, k);
            } else if (bfNode.bfOperator == BFAND){
                return entail(*bfNode.bfnode1, k) && entail(*bfNode.bfnode2, k);
            } else{
                return true;
            }
        }
        case EForm:
        {
            Agents::const_iterator it;

            for (it = bfNode.agents.begin(); it != bfNode.agents.end(); it++){
                if (!entail_BF(k, *(bfNode.bfnode1), *it, true)){
                    return false;
                }
            }
            return true;
        }
        case BFEmpty:
            // cout << "BFEmpty" << endl;
            return true;
        default:
            // cout << "default" << endl;
            return true;
    }
}

int Kstate::loopIn()
{
    int result = 0;
    for (auto e1 = edge_in.begin(); e1 != edge_in.end(); e1++){
        if (find(edge_out.begin(), edge_out.end(), *e1) != edge_out.end())
        result++;
    }
    return result;
}