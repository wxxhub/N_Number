#include "include/n_digital/node.h"

using namespace std;

Node::Node(const int layer, GlobalConfig *global_config, std::map<int, Node*> *open_set, vector<Node*> *close_table)
{
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
    for (int i = 0; i < 4; i++)
    {
        vector<vector<int>> new_map = map_;
        
        bool result = move(Direction(i), new_map);
        if (result == true)
        {
            // add id
            global_config_->addId();

            Node *new_node = new Node(layer_ + 1, global_config_, open_table_, close_table_);
            new_node->setMap(dimension_, new_map, goal_, Direction(i));
            new_node->setParentNode(this);
            child_node_.push_back(new_node);

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

bool Node::move(Direction direction, vector<vector<int>>& map)
{
    if (direction == forbid_direction_)
    {
        return false;
    }

    switch (direction)
    {
        case move_up:
        {
            if (now_point_->y - 1 < 0)
                return false;

            map[now_point_->y][now_point_->x] = map[now_point_->y-1][now_point_->x];
            map[now_point_->y-1][now_point_->x] = 0;

            return true;
        }
            
        
        case move_down:
        {
            if (now_point_->y + 1 >= dimension_)
                return false;

            map[now_point_->y][now_point_->x] = map[now_point_->y+1][now_point_->x];
            map[now_point_->y+1][now_point_->x] = 0;

            return true;
        }
        
        case move_left:
        {
            if (now_point_->x - 1 < 0)
                return false;

            map[now_point_->y][now_point_->x] = map[now_point_->y][now_point_->x - 1];
            map[now_point_->y][now_point_->x - 1] = 0;

            return true;
        }

        case move_right:
        {
            if (now_point_->x + 1 >= dimension_)
                return false;

            map[now_point_->y][now_point_->x] = map[now_point_->y][now_point_->x + 1];
            map[now_point_->y][now_point_->x + 1] = 0;

            return true;
        }
        
        default:
            return false;
    }
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

std::vector<std::vector<int>> Node::getMap()
{
    return map_;
}

void Node::setF()
{
    f_ = g_ + h_;
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
