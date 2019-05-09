#include "n_digital/n_digital.h"

using namespace std;

NDigital::NDigital()
    : dimension_(0)
{
    open_table_ = new map<int, Node*>();
    close_table_ = new std::vector<Node*>();
    now_point_ = new Point();
    global_config_ = new GlobalConfig();
    root_node_ = new Node(0, global_config_, open_table_, close_table_);
    (*open_table_)[global_config_->getNowId()] = root_node_;
}

NDigital::~NDigital()
{

}

int NDigital::findNext()
{
    // choice next node
    map<int, Node*>::iterator iter = open_table_->begin();
    int index = -1;
    int min_f = 0;
    for (; iter != open_table_->end(); iter++)
    {
        printf ("min_F: %d\n", iter->second->getF());
        printf ("id_: %d\n", iter->first);
        root_node_->printMap(iter->second->getMap());
        if (index == -1)
        {
            index = iter->first;
            min_f = iter->second->getF();
        }
        else
        {
            if (min_f > iter->second->getF())
            {
                index = iter->first;
                min_f = iter->second->getF();
            }
        }
    }

    // process min_f node
    printf ("index: %d\n", index);
    int result = (*open_table_)[index]->process();

    return result;
}

void NDigital::setMap3(int test[3][3], int goal[3][3])
{
    update(3, test, goal);
}

void NDigital::update(int dimension, int test[3][3], int goal[3][3])
{
    dimension_ = dimension;
    
    test_.resize(dimension_, vector<int>(dimension_));
    goal_.resize(dimension_, vector<int>(dimension_));

    for (int x = 0; x < dimension_; x++)
    {
        for (int y = 0; y < dimension_; y++)
        {
            test_[y][x] = test[y][x];
            goal_[y][x] = goal[y][x];

            if (test[y][x] == 0)
            {
                now_point_->x = x;
                now_point_->y = y;
            }
        }
    }

    root_node_->setMap(dimension_, test_, &goal_, none_direction);
}

void NDigital::cleanr()
{
    // root_node_->cleanr();

    global_config_->reset();

    test_.clear();
    goal_.clear();
    open_table_->clear();
}

void NDigital::checkCloseTable()
{
    printf ("############## start ##############\n");
    for (int i = 0; i < close_table_->size(); i++)
    {
        root_node_->printMap((*close_table_)[i]->getMap());
    }
    printf ("############### end ###############\n");
}
