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
    if (open_table_->size() <=0 )
        return -2;

    // choice min_f node
    map<int, Node*>::iterator iter = open_table_->begin();
    int index = -1;
    int min_f = 0;
    for (; iter != open_table_->end(); iter++)
    {
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
    
    int result = result = (*open_table_)[index]->process();

    if (result == -2)
        return result;
    
    updateResult();

    if (result == 0)
    {
        setResultNode();
    }

    return result;
}

void NDigital::setMap3(vector<vector<int>> origion_map, vector<vector<int>> goal_map)
{
    update(3, origion_map, goal_map);
}

void NDigital::setMap4(vector<vector<int>> origion_map, vector<vector<int>> goal_map)
{
    update(4, origion_map, goal_map);
}

void NDigital::setMap5(vector<vector<int>> origion_map, vector<vector<int>> goal_map)
{
    update(5, origion_map, goal_map);
}

void NDigital::update(int dimension, vector<vector<int>> origion_map, vector<vector<int>> goal_map)
{
    dimension_ = dimension;
    
    origion_.resize(dimension_, vector<int>(dimension_));
    goal_.resize(dimension_, vector<int>(dimension_));

    for (int x = 0; x < dimension_; x++)
    {
        for (int y = 0; y < dimension_; y++)
        {
            origion_[y][x] = origion_map[y][x];
            goal_[y][x] = goal_map[y][x];

            if (origion_map[y][x] == 0)
            {
                now_point_->x = x;
                now_point_->y = y;
            }
        }
    }

    root_node_->setMap(dimension_, origion_, &goal_, none_direction);
    global_config_->setGoalMap(goal_map);
}

void NDigital::setResultNode()
{
    result_node_ = (*open_table_)[open_table_tips_[0].id];

    Node *iter_node = result_node_;

    result_table_tips_.clear();
    result_table_.clear();

    while (iter_node)
    {
        result_table_.push_back(iter_node);

        TableTips new_table_tips;
        new_table_tips.id = iter_node->getId();
        new_table_tips.value = iter_node->getF();
        new_table_tips.layer = iter_node->getLayer();
        result_table_tips_.push_back(new_table_tips);

        iter_node = iter_node->getParentNode();
    }
}

void NDigital::setFFunction(int t)
{
    global_config_->setFFunction(t);
}

void NDigital::updateResult()
{
    // update open_table_tips
    if (open_table_->size() > 0)
    {
        open_table_tips_.clear();
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
                    else if(new_table_tips.value >= iter->second->getF())
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

        // update min_open_map
        vector<vector<int>> min_open_map = (*open_table_)[open_table_tips_[0].id]->getMap();
        min_open_map_.assign(min_open_map.begin(), min_open_map.end());
    }
    
    // update close_table_tips
    close_table_tips_.clear();
    close_table_tips_.resize(close_table_->size());

    for (int i = 0; i < close_table_->size(); i++)
    {
        close_table_tips_[i].id = (*close_table_)[i]->getId();
        close_table_tips_[i].layer = (*close_table_)[i]->getLayer();
        close_table_tips_[i].value = (*close_table_)[i]->getF();
    }
}

void NDigital::cleanr()
{
    global_config_->reset();

    origion_.clear();
    goal_.clear();
    open_table_->clear();
    close_table_->clear();
    result_table_.clear();

    open_table_tips_.clear();
    close_table_tips_.clear();
    result_table_tips_.clear();

    Node *delete_node = root_node_;
    root_node_ = new Node(0, global_config_, open_table_, close_table_);
    (*open_table_)[global_config_->getNowId()] = root_node_;
    delete delete_node;
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

void NDigital::getDefault3Map(vector<vector<int>> &origion_map, vector<vector<int>> &goal_map)
{
    origion_map.resize(3);
    goal_map.resize(3);
    int default_origion[3][3] = {
        1, 2, 3,
        4, 5, 0,
        6, 8, 7,
    };

    // int default_goal[3][3] = {
    //     1, 2, 3,
    //     6, 4, 5,
    //     0, 8, 7,
    // };

    int default_goal[3][3] = {
        0, 2, 3,
        6, 4, 5,
        1, 8, 7,
    };

    for (int i = 0; i < 3; i++)
    {
        origion_map[i].resize(3);
        goal_map[i].resize(3);
        for (int j = 0; j < 3; j++)
        {
            origion_map[i][j] = default_origion[i][j];
            goal_map[i][j]    =default_goal[i][j];
        }
    }
}

void NDigital::getDefault4Map(vector<vector<int>> &origion_map, vector<vector<int>> &goal_map)
{
    origion_map.resize(4);
    goal_map.resize(4);
    int default_origion[4][4] = {
        0, 1, 3, 8,
        4, 2, 5, 9,
        6, 8, 7, 10,
        11, 12, 13, 14,
    };

    int default_goal[4][4] = {
        1, 2, 3, 8,
        4, 5, 7, 9,
        6, 13, 12, 10,
        11, 8, 14, 0,
    };

    for (int i = 0; i < 4; i++)
    {
        origion_map[i].resize(4);
        goal_map[i].resize(4);
        for (int j = 0; j < 4; j++)
        {
            origion_map[i][j] = default_origion[i][j];
            goal_map[i][j]    =default_goal[i][j];
        }
    }
}

void NDigital::getDefault5Map(vector<vector<int>> &origion_map, vector<vector<int>> &goal_map)
{
    origion_map.resize(5);
    goal_map.resize(5);
    int default_origion[5][5] = {
        1, 2, 3, 9, 10,
        4, 5, 0, 11, 12,
        6, 8, 7, 13, 14,
        15, 16, 17, 18, 19,
        20, 21, 22, 23, 24,
    };

    int default_goal[5][5] = {
        1, 2, 3, 9, 10,
        4, 5, 11, 13, 12,
        6, 8, 7, 18, 14,
        15, 16, 17, 23, 19,
        20, 21, 22, 24, 25,
    };

    for (int i = 0; i < 5; i++)
    {
        origion_map[i].resize(5);
        goal_map[i].resize(5);
        for (int j = 0; j < 5; j++)
        {
            origion_map[i][j] = default_origion[i][j];
            goal_map[i][j]    =default_goal[i][j];
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

vector<vector<int>> NDigital::getOpeneMap(int id)
{
    return (*open_table_)[id]->getMap();
}

vector<vector<int>> NDigital::getMinOpeneMap()
{
    return min_open_map_;
}

std::vector<std::vector<int>>  NDigital::getResultMap(int id)
{
    return result_table_[id]->getMap();
}

vector<TableTips> NDigital::getOpenTableTips()
{
    return open_table_tips_;
}

vector<TableTips> NDigital::getCloseTableTips()
{
    return close_table_tips_;
}

vector<TableTips> NDigital::getResultTableTips()
{
    return result_table_tips_;
}
