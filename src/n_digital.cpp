#include "include/n_digital/n_digital.h"

using namespace std;

NDigital::NDigital()
    : dimension_(0)
{
    open_table_ = new map<int, Node*>();
    close_table_ = new vector<Node*>();
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

void NDigital::setMap3(int origion[3][3], int goal[3][3])
{
    update(3, origion, goal);
}

void NDigital::setMap3(vector<vector<int>> origion, vector<vector<int>> goal)
{
    update(3, origion, goal);
}

void NDigital::update(int dimension, int origion[3][3], int goal[3][3])
{
    dimension_ = dimension;
    
    origion_.resize(dimension_, vector<int>(dimension_));
    goal_.resize(dimension_, vector<int>(dimension_));

    for (int x = 0; x < dimension_; x++)
    {
        for (int y = 0; y < dimension_; y++)
        {
            origion_[y][x] = origion[y][x];
            goal_[y][x] = goal[y][x];

            if (origion[y][x] == 0)
            {
                now_point_->x = x;
                now_point_->y = y;
            }
        }
    }

    root_node_->setMap(dimension_, origion_, &goal_, none_direction);
}

void NDigital::update(int dimension, vector<vector<int>> origion, vector<vector<int>> goal)
{
    dimension_ = dimension;
    
    origion_.resize(dimension_, vector<int>(dimension_));
    goal_.resize(dimension_, vector<int>(dimension_));

    for (int x = 0; x < dimension_; x++)
    {
        for (int y = 0; y < dimension_; y++)
        {
            origion_[y][x] = origion[y][x];
            goal_[y][x] = goal[y][x];

            if (origion[y][x] == 0)
            {
                now_point_->x = x;
                now_point_->y = y;
            }
        }
    }

    root_node_->setMap(dimension_, origion_, &goal_, none_direction);
}

void NDigital::cleanr()
{
    // root_node_->cleanr();

    global_config_->reset();

    origion_.clear();
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

void NDigital::getDefault3Map(int origion[3][3], int goal[3][3])
{
    int default_origion[3][3] = {
        1, 2, 3,
        4, 5, 0,
        6, 8, 7,
    };

    int default_goal[3][3] = {
        1, 2, 3,
        6, 4, 5,
        0, 8, 7,
    };

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            origion[i][j] = default_origion[i][j];
            goal[i][j]    =default_goal[i][j];
        }
    }
}

void NDigital::getDefault3Map(vector<vector<int>> &origion, vector<vector<int>> &goal)
{
    origion.resize(3);
    goal.resize(3);
    int default_origion[3][3] = {
        1, 2, 3,
        4, 5, 0,
        6, 8, 7,
    };

    int default_goal[3][3] = {
        1, 2, 3,
        6, 4, 5,
        0, 8, 7,
    };

    for (int i = 0; i < 3; i++)
    {
        origion[i].resize(3);
        goal[i].resize(3);
        for (int j = 0; j < 3; j++)
        {
            origion[i][j] = default_origion[i][j];
            goal[i][j]    =default_goal[i][j];
        }
    }
}

vector<Node*> *NDigital::getCloseTable()
{
    return close_table_;
}

vector<vector<int>> NDigital::getCloseMap(int id)
{
    return (*close_table_)[id]->getMap();
}

vector<vector<int>> NDigital::getEndCloseMap()
{
    return (*close_table_)[close_table_->size() - 1]->getMap();
}

vector<TableTips> NDigital::getOpenTableTips()
{
    open_table_tips_.resize(open_table_->size());
    vector<bool> if_set(open_table_->size(), false);
    for (int i = 0; i < open_table_->size(); i++)
    {
        map<int, Node*>::iterator iter = open_table_->begin();
        TableTips new_table_tips = {0, 0, 0};
        int min_site = 0;
        for (int j = 0; iter != open_table_->end(); iter++, j++)
        {
            if (if_set[j] == false)
            {
                if (new_table_tips.value == 0)
                {
                    new_table_tips.id = iter->second->getId();
                    new_table_tips.layer = iter->second->getLayer();
                    new_table_tips.value = iter->second->getF();
                    min_site = j;
                }
                else if(new_table_tips.value > iter->second->getF())
                {
                    new_table_tips.id = iter->second->getId();
                    new_table_tips.layer = iter->second->getLayer();
                    new_table_tips.value = iter->second->getF();
                    min_site = j;
                }
            }
        }

        if_set[min_site] = true;
        open_table_tips_[i] = new_table_tips;
    }

    return open_table_tips_;
}

vector<TableTips> NDigital::getCloseTableTips()
{
    close_table_tips_.resize(close_table_->size());

    for (int i = 0; i < close_table_->size(); i++)
    {
        close_table_tips_[i].id = (*close_table_)[i]->getId();
        close_table_tips_[i].layer = (*close_table_)[i]->getLayer();
        close_table_tips_[i].value = (*close_table_)[i]->getF();
    }
    return close_table_tips_;
}
