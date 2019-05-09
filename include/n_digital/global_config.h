#include <iostream>

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

    void reset()
    {
        id_ = 0;
    }

    int getNowId()
    {
        return id_;
    }

private:
    int id_ = 0;
};