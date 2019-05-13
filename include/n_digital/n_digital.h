#include "include/n_digital/node.h"

struct TableTips
{
    int id;
    int layer;
    int value;
};


class NDigital
{
public:
    NDigital();
    ~NDigital();

    void setMap3(std::vector<std::vector<int>> origion_map, std::vector<std::vector<int>> goal_map);
    void setMap4(std::vector<std::vector<int>> origion_map, std::vector<std::vector<int>> goal_map);
    void setMap5(std::vector<std::vector<int>> origion_map, std::vector<std::vector<int>> goal_map);

    void setResultNode();
    void setFFunction(int t);

    int findNext();

    void cleanr();

    void checkCloseTable();

    void getDefault3Map(std::vector<std::vector<int>> &origion_map, std::vector<std::vector<int>> &goal_map);
    void getDefault4Map(std::vector<std::vector<int>> &origion_map, std::vector<std::vector<int>> &goal_map);
    void getDefault5Map(std::vector<std::vector<int>> &origion_map, std::vector<std::vector<int>> &goal_map);

    std::vector<Node*> *getCloseTable();
    std::vector<std::vector<int>> getCloseMap(int id);
    std::vector<std::vector<int>> getEndCloseMap();

    std::vector<std::vector<int>> getOpeneMap(int id);
    std::vector<std::vector<int>> getMinOpeneMap();

    std::vector<std::vector<int>>  getResultMap(int id);
    
    std::vector<TableTips> getOpenTableTips();
    std::vector<TableTips> getCloseTableTips();
    std::vector<TableTips> getResultTableTips();
private:
    int dimension_;

    Node *root_node_;
    Node *result_node_;
    Point *now_point_;

    GlobalConfig *global_config_;

    std::vector<std::vector<int>> origion_;
    std::vector<std::vector<int>> goal_;

    std::vector<std::vector<int>> min_open_map_;

    std::map<int, Node*> *open_table_;
    std::vector<Node*> *close_table_;
    std::vector<Node*> result_table_;

    std::vector<TableTips> open_table_tips_;
    std::vector<TableTips> close_table_tips_;
    std::vector<TableTips> result_table_tips_;

    void update(int dimension, std::vector<std::vector<int>> origion_map, std::vector<std::vector<int>> goal_map);

    void updateResult();
private:
    
};
