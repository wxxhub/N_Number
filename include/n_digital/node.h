#include <iostream>
#include <vector>
#include <map>

#include "include/n_digital/global_config.h"

const int max_layer = 32;

enum MoveResult
{
    MOVE_SUCCESS,
    MOVE_FAILED,
    H_BYOUND,
};

enum Direction
{
    move_up,
    move_down,
    move_left,
    move_right,
    none_direction,
};

struct Point
{
/**
 *      --->x
 *      |
 *      y
 **/
    int x;
    int y;
};

class Node
{
public:
    Node(const int layer, GlobalConfig *global_config, std::map<int, Node*> *open_set, std::vector<Node*> *close_table);
    ~Node();

    int process();

    void setMap(const int dimension, const std::vector<std::vector<int>> map, std::vector<std::vector<int>> *goal, Direction last_direction);
    void setParentNode(Node* parent_node);

    int getH();
    int getG();
    int getF();
    int getId();
    int getLayer();
    Node* getParentNode();

    std::vector<std::vector<int>> getMap();

    void printMap(const std::vector<std::vector<int>> map);

    void cleanr();

private:
    int dimension_;

    int g_;     // S0 -> n = layer
    int h_;     // n  -> Sg
    int f_;     // f_ = g_ + h_
    int layer_;

    int id_;

    Node* parent_node_;

    Point* now_point_;
    Direction forbid_direction_;
    Direction forbid_direction_list_;

    GlobalConfig* global_config_;

    std::vector<Node*> child_node_;

    std::vector<std::vector<int>> map_;
    std::vector<std::vector<int>> *goal_;

    std::map<int, Node*> *open_table_;
    std::vector<Node*> *close_table_;

    MoveResult move(Direction direction, std::vector<std::vector<int>>& map);
    void setF();
    int calculatHDistance(int x, int y);

    Node* createChildNode(std::vector<std::vector<int>> new_map, Direction direction);
};
