#include "n_digital/node.h"

class NDigital
{
public:
    NDigital();
    ~NDigital();

    void setMap3(int test[3][3], int goal[3][3]);

    int findNext();

    void cleanr();

    void checkCloseTable();

private:
    int dimension_;

    Node *root_node_;
    Point *now_point_;

    GlobalConfig *global_config_;

    std::vector<std::vector<int>> test_;
    std::vector<std::vector<int>> goal_;

    std::map<int, Node*> *open_table_;
    std::vector<Node*> *close_table_;

    void update(int dimension, int test[3][3], int goal[3][3]);
private:
    
};