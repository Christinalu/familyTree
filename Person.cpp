// Xiaolu Zhu (Christina)
// xza172@sfu.ca

#include "Person.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

bool Person::setFather(Person* newFather)
{
    ///
    if (newFather == nullptr || newFather->sex == Sex::FEMALE)
    {
        return false;
    }
    ///
    if (father != nullptr)
    {
        if (father->removeChild(this) == false)
            return false;
    }
    if (newFather->addChild(this) == false)
        return false;

    return true;
}

bool Person::setMother(Person* newMother)
{
    ///
    if (newMother == nullptr || newMother->sex == Sex::MALE)
    {
        return false;
    }
    ///
    if (mother != nullptr)
    {
        if (mother->removeChild(this) == false)
            return false;
    }
    if (newMother->addChild(this) == false)
        return false;

    return true;
}

bool Person::hasChild(const Person* child) const
{
    auto target = findChild(child);
    // target found
    if (target != children.end()) {
        return true;
    }
    return false;
}

bool Person::addChild(Person* child)
{
    if (child != NULL) {
        children.push_back(child);
        
        if (sex == Sex::MALE)
        {   
            //removes it from the old parent's children list...?
            child->father = this;
        }
        else
        {
            child->mother = this;
        }
        return true;
    }
    return false;
}

bool Person::removeChild(Person* child)
{
    if (child != NULL) {
        auto target = findChild(child);
        // no target found
        if (children.end() == target) {
            return false;
        }
        children.erase(target);
        if (sex == Sex::MALE)
        {
            child->father = nullptr;
        }
        else
        {
            child->mother = nullptr;
        }
        
        return true;
    }
    return false;
}

void Person::removeAllChildren()
{
    for (int i = 0; i < getNumChildren(); i++)
    {
        children[i]->father = NULL;
        children[i]->mother = NULL;
    }
    children.clear();
}

void Person::getAncestors(People& results) const
{
    std::stack<Person*> ancestors;
    ancestors.push(getFather());
    ancestors.push(getMother());
    Person* next_node;
    while (!ancestors.empty())
    {
        next_node = ancestors.top();
        ancestors.pop();
        if (next_node->getFather() != NULL) {
            ancestors.push(next_node->getFather());
        }
        if (next_node->getMother() != NULL) {
            ancestors.push(next_node->getMother());
        }
        results.push_back(next_node);
    }
}

// Traverse the B-tree
void Person::getDescendants(People& results) const
{
    std::stack<Person*> descendants;
    for (int i = 0; i < getNumChildren(); i++) {
        descendants.push(getChild(i));
    }
    Person* next_node;
    while (!descendants.empty())
    {
        next_node = descendants.top();
        descendants.pop();
        for (int i = 0; i < next_node->getNumChildren(); i++) {
            descendants.push(next_node->getChild(i));
        }
        results.push_back(next_node);
    }
}

void Person::getSiblings(People& results) const
{
    //vector_name.insert(position, iterator1 , iterator2)
    if (getFather() != NULL) {
        results.insert(results.end(), getFather()->children.begin(), getFather()->children.end());
    }
    if (getMother() != NULL) {
        results.insert(results.end(), getMother()->children.begin(), getMother()->children.end());
    }

    // eliminate that person him/herself if in the result list
    for (int i = 0; i < results.size(); i++)
    {
        if(results[i] == this){
            results.erase(results.begin()+i);
            i--;
            continue;
        }
        auto himself = std::find(results.begin()+i+1, results.end(), results[i]);
        if (himself != results.end()){
            results.erase(himself);
            i--;
        }
    }
    
}

void Person::getCousins(People& results) const
{
    if (getFather() != NULL) {
        Person* grandFather = father->getFather();
        if (grandFather != NULL) {
            for (int i = 0; i < grandFather->getNumChildren(); i++) {
                if (grandFather->getChild(i) == father)
                    continue; //pass this case
                Person* others = grandFather->getChild(i);
                for (int j = 0; j < others->getNumChildren(); j++) {
                    results.push_back(others->getChild(j));
                }
            }
        }
    }
}

People::iterator Person::findChild(const Person* p)
{
    auto target = std::find(children.begin(), children.end(), p);
    return target;
}

People::const_iterator Person::findChild(const Person* p) const
{
    auto target = std::find(children.begin(), children.end(), p);
    return target;
}

