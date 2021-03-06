/////////////////////////////////////////////////////////////////////////////
//
//              R O T A T I O N   S E A R C H
//
// This package contains the source code which implements the
// BnB rotation search algorithm and the nested 6 DoF registration
// algorithm proposed in
//
// A. Parra Bustos, T.-J. Chin, A. Eriksson, H. Li and D. Suter
// Fast Rotation Search with Stereographic Projections for 3D Registration
// IEEE Transactions on Pattern Analysis and Machine Intelligence (TPAMI)
//
// Copyright (c) 2016 Alvaro PARRA BUSTOS (aparra@cs.adelaide.edu.au.)
// School of Computer Science, The University of Adelaide, Australia
// The Australian Center for Visual Technologies
// http://cs.adelaide.edu.au/~aparra
// Please acknowledge the authors by citing the above paper in any academic
// publications that have made use of this package or part of it.
//
/////////////////////////////////////////////////////////////////////////////


#ifndef REG_STATE_PRIORITY_HASHTABLE_H_
#define REG_STATE_PRIORITY_HASHTABLE_H_

#include "state.h"
#include <cstddef>

#ifdef __linux__ //asuming gcc
#include <tr1/unordered_map>
using namespace std::tr1;
#else
#include <unordered_map> //asuming clang
#endif

namespace reg {
namespace search {

//---------------------------------------------
//      State Priority Hashtable
//---------------------------------------------

template <typename SSR, typename Scalar, template <typename SSR_, typename Scalar_> class SS >
class StatePriorityHashtable
{

private:
    class Node;
    class Queue;
    struct Hash;

    #ifdef __linux__ //asuming gcc
    typedef std::tr1::unordered_map<Scalar, Queue*, Hash> Map;
    #else
    typedef std::unordered_map<Scalar, Queue*, Hash> Map;
    #endif
    class Node
    {
    public:
        Node *next;
        SS<SSR, Scalar> *ss;

        Node(SS<SSR, Scalar> *ss):ss(ss){}
        ~Node()
        {
            delete ss;
        }

    };

    class Queue
    {
    public:
        Queue();
        ~Queue();

        Node *head;
        Node *tail;

        int size;
        SS<SSR, Scalar> *pop();
        void push(SS<SSR, Scalar> *ss);
        void stack(SS<SSR, Scalar> *ss); //not the best design...

        void dump(std::ofstream& ofs) const;
    }; // Queue


    struct Hash
    {
        std::size_t operator()(const int& q) const
        {
            unsigned int key=q;
            key = ((key >> 16) ^ key) * 0x45d9f3b;
            key = ((key >> 16) ^ key) * 0x45d9f3b;
            key = ((key >> 16) ^ key);
            return key;
        }
    };

    Map m_map;


public:
    StatePriorityHashtable(int bucketSize);
    ~StatePriorityHashtable();

    SS<SSR, Scalar> *pop();
    void push(SS<SSR, Scalar> *state);
    void prune(int curbest);

    void pruneAndReturn(int curbest, StatePriorityHashtable &table);

    unsigned int size() const;

    //dump table SSR to file
    void dump(std::ofstream& ofs) const;

    int m_max_upbnd;
    Queue *m_max_upbnd_queue;
    unsigned int m_size; // number of states
    //!param and fun for querying the weights
    Node *currNode;
    void initCurrNode(){
        currNode = m_max_upbnd_queue->head;
    }
    SS<SSR, Scalar> *jump(int NoSteps){
        for(int i=0;i<NoSteps;i++){
            if(currNode->next == NULL){
                return NULL;
            }
            else{
                currNode = currNode->next;
            }
        }
        return currNode->ss;
    }
};

    
    
    
//---------------------------------------------
//      State Priority Hashtable
//---------------------------------------------
    
template <typename SSR, typename Scalar, template <typename SSR_, typename Scalar_> class SS >
class StatePriorityHashtableDF
{
        
    private:
        class Queue;
        struct Hash;
        
#ifdef __linux__ //asuming gcc
        typedef std::tr1::unordered_map<Scalar, Queue*, Hash> Map;
#else
        typedef std::unordered_map<Scalar, Queue*, Hash> Map;
#endif
        
    int m_max_upbnd;
    Queue *m_max_upbnd_queue;
    unsigned int m_size; // number of states
        
    class Queue
    {
    private:
        class Node
        {
        public:
            Node *next;
            SS<SSR, Scalar> *ss;
            
            Node(SS<SSR, Scalar> *ss):ss(ss){}
            ~Node()
            {
                if (ss!=NULL) delete ss;
            }
            
        };
    public:
        Queue();
        ~Queue();
        
        Node *head;
        Node *tail;
        
        int size;
        SS<SSR, Scalar> *pop();
        void push(SS<SSR, Scalar> *ss);
        void stack(SS<SSR, Scalar> *ss); //not the best design...
        
        void dump(std::ofstream& ofs) const;
    }; // Queue
    
    
    struct Hash
    {
        std::size_t operator()(const int& q) const
        {
            unsigned int key=q;
            key = ((key >> 16) ^ key) * 0x45d9f3b;
            key = ((key >> 16) ^ key) * 0x45d9f3b;
            key = ((key >> 16) ^ key);
            return key;
        }
    };
    
    Map m_map;
    
    
public:
    StatePriorityHashtableDF(int bucketSize);
    ~StatePriorityHashtableDF();
    
    SS<SSR, Scalar> *pop();
    void push(SS<SSR, Scalar> *state);
    void prune(int curbest);
    unsigned int size() const;
    
    //dump table SSR to file
    void dump(std::ofstream& ofs) const;
};
    
} // End namespace search
} // End namespace reg


#include "state_priority_hashtable.hpp"
#include "state_priority_hashtable_df.hpp"

#endif
