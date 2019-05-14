#include <iostream>
#include <vector>

class GlobalConfig
{
public:
    GlobalConfig(){};

    int addId()
    {
        int id = id_;
        id_++;

        return id;
    }

    void setGoalMap(std::vector<std::vector<int>> goal_map)
    {
        goal_map_.assign(goal_map.begin(), goal_map.end());
    }

    void reset()
    {
        id_ = 0;
    }
    void setFFunction(int f)
    {
        if (f == 0)
            f_function_ = 0;
        else
            f_function_ = 1;
    }

    int getNowId()
    {
        return id_;
    }

    int getFFunction()
    {
        return f_function_;
    }

    int calculateH(std::vector<std::vector<int>> compare_map)
    {
        int h = 0;
        if (f_function_ == 0)
        {
            for (int x = 0; x < compare_map.size(); x++)
            {
                for (int y = 0; y < compare_map[x].size(); y++)
                {
                    if (compare_map[x][y] != goal_map_[x][y])
                        h++;
                }
            }
        }
        else
        {
            for (int x = 0; x < compare_map.size(); x++)
            {
                for (int y = 0; y < compare_map[x].size(); y++)
                {
                    if (compare_map[x][y] != goal_map_[x][y])
                        h += getHDis(compare_map[x][y], x, y);
                }
            }
        }

        return h;
    }

    int getHDis(int value, int x1, int y1)
    {
        for (int x = 0; x <  goal_map_.size(); x++)
            {
                for (int y = 0; y <  goal_map_[x].size(); y++)
                {
                    if ( goal_map_[x1][y1] == goal_map_[x][y])
                    {
                        return abs(x1 - x) + abs(y1 - y);
                    }
                }
            }
    }

private:
    int id_ = 0;
    int f_function_ = 0;

    std::vector<std::vector<int>> goal_map_;
};