#include "include/n_digital/node.h"

using namespace std;

Node::Node(const int layer, GlobalConfig *global_config, std::map<int, Node*> *open_set, vector<Node*> *close_table)
{
    parent_node_ = NULL;
    layer_ = layer;
    g_ = layer_;
    now_point_ = new Point();
    global_config_ = global_config;
    open_table_ = open_set;
    close_table_ = close_table;
    id_ = global_config_->getNowId();
}

Node::~Node()
{
    if (now_point_ != nullptr)
        delete now_point_;

    for (int i = 0; i < child_node_.size(); i++)
    {
        if (child_node_[i] != nullptr)
            delete child_node_[i];
    }
}

int Node::process()
{
    if (layer_ > max_layer)
        return -2;

    //  move 
    int failed_times = 0;
    for (int i = 0; i < 4; i++)
    {
        vector<vector<int>> new_map = map_;
        
        MoveResult result = move(Direction(i), new_map);
        if (result == MOVE_SUCCESS)
        {
            Node *new_node = createChildNode(new_map, Direction(i));
            if (new_node)
            {
                // add new_node in open_set
                (*open_table_)[global_config_->getNowId()] = new_node;

                // remove current node from open_set and put it to close_table
                map<int, Node*>::iterator iter = open_table_->begin();
                for (; iter != open_table_->end(); iter++)
                {
                    if (iter->first == id_)
                    {
                        close_table_->push_back(iter->second);
                        open_table_->erase(iter);
                        break;
                    }
                }

                // if match finish find
                if (new_node->getH() == 0)
                {
                    printf ("ok!\n");
                    return 0;
                }
            }
            else
            {
                failed_times++;
            }
        }
        else
        {
            failed_times++;
        }
    }

    if (failed_times == 4)
    {
        if (open_table_->size() <= 1)
            return -2;
            
        map<int, Node*>::iterator iter = open_table_->begin();
        for (; iter != open_table_->end(); iter++)
        {
            if (iter->first == id_)
            {
                close_table_->push_back(iter->second);
                open_table_->erase(iter);
                break;
            }
        }
    }

    // charge child node size
    if (child_node_.size() <= 0)
    {
        printf ("no chiled!\n");
        return -1;
    }

    return 1;
}

void Node::setMap(const int dimension, const vector<vector<int>> map, std::vector<std::vector<int>> *goal, Direction last_direction)
{
    map_.assign(map.begin(), map.end());
    goal_ = goal;
    dimension_ = dimension;
    h_ = 0;
    for (int x = 0; x < dimension_; x++)
    {
        for (int y = 0; y < dimension_; y++)
        {
            if (map_[y][x] != (*goal_)[y][x])
                h_++;

            if (map_[y][x] == 0)
            {
                now_point_->x = x;
                now_point_->y = y;
            }
        }
    }

    // set forbid_direction
    switch (last_direction)
    {
    case move_up:
        forbid_direction_ = move_down;
        break;

    case move_down:
        forbid_direction_ = move_up;
        break;

    case move_left:
        forbid_direction_ = move_right;
        break;
    
    case move_right:
        forbid_direction_ = move_left;
        break;
    
    default:
        forbid_direction_ = none_direction;
        break;
    }

    setF();
}

void Node::setParentNode(Node* parent_node)
{
    parent_node_ = parent_node;
}

MoveResult Node::move(Direction direction, vector<vector<int>>& new_map)
{
    if (direction == forbid_direction_)
    {
        return MOVE_FAILED;
    }

    switch (direction)
    {
        case move_up:
        {
            if (now_point_->y - 1 < 0)
                return MOVE_FAILED;

            new_map[now_point_->y][now_point_->x] = new_map[now_point_->y-1][now_point_->x];
            new_map[now_point_->y-1][now_point_->x] = 0;

            break;
        }
            
        
        case move_down:
        {
            if (now_point_->y + 1 >= dimension_)
                return MOVE_FAILED;

            new_map[now_point_->y][now_point_->x] = new_map[now_point_->y+1][now_point_->x];
            new_map[now_point_->y+1][now_point_->x] = 0;

            break;
        }
        
        case move_left:
        {
            if (now_point_->x - 1 < 0)
                return MOVE_FAILED;

            new_map[now_point_->y][now_point_->x] = new_map[now_point_->y][now_point_->x - 1];
            new_map[now_point_->y][now_point_->x - 1] = 0;

            break;
        }

        case move_right:
        {
            if (now_point_->x + 1 >= dimension_)
                return MOVE_FAILED;

            new_map[now_point_->y][now_point_->x] = new_map[now_point_->y][now_point_->x + 1];
            new_map[now_point_->y][now_point_->x + 1] = 0;

            break;
        }
        
        default:
            return MOVE_FAILED;
    }

    if (global_config_->calculateH(new_map) > h_)
        return H_BYOUND;

    return MOVE_SUCCESS;
}

int Node::getH()
{
    return h_;
}

int Node::getG()
{
    return g_;
}

int Node::getF()
{
    return f_;
}

int Node::getId()
{
    return id_;
}

int Node::getLayer()
{
    return layer_;
}

Node* Node::getParentNode()
{
    return parent_node_;
}

std::vector<std::vector<int>> Node::getMap()
{
    return map_;
}

void Node::setF()
{
    f_ = g_ + h_;
}

Node* Node::createChildNode(vector<vector<int>> new_map, Direction direction)
{
    // check if same with open or close table's member
    
    map<int, Node*>::iterator iter = open_table_->begin();
    for (; iter != open_table_->end(); iter++)
    {
        int not_same_num = 0;
        vector<vector<int>> iter_map = iter->second->getMap();
        for (int i = 0; i < iter_map.size(); i++)
        {
            for (int j = 0; j < iter_map[i].size(); j++)
            {
                if (iter_map[i][j] != new_map[i][j])
                {
                    not_same_num++;
                }
            }
        }

        if (not_same_num == 0)
            return NULL;
    }

    for (int n = 0; n < (*close_table_).size(); n++)
    {
        int not_same_num = 0;
        vector<vector<int>> iter_map = (*close_table_)[n]->getMap();

        for (int i = 0; i < iter_map.size(); i++)
        {
            for (int j = 0; j < iter_map[i].size(); j++)
            {
                if (iter_map[i][j] != new_map[i][j])
                {
                    not_same_num++;
                }
            }
        }

        if (not_same_num == 0)
            return NULL;
    }
    

    // add id
    global_config_->addId();

    Node *new_node = new Node(layer_ + 1, global_config_, open_table_, close_table_);
    new_node->setMap(dimension_, new_map, goal_, direction);
    Node *self_node = this;
    new_node->setParentNode(self_node);
    child_node_.push_back(new_node);

    return new_node;
}

void Node::printMap(const vector<vector<int>> map)
{
    if (map.size() <= 0)
    {
        printf("***** no map *****\n");
        return;
    }

    printf("******** start ********\n");

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            printf("%5d", map[i][j]);          
        }
        printf("\n");
    }
    
    printf("********* end *********\n\n");
}
