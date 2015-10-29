#include "mockmodel.h"

MockModel::MockModel()
{

}

int MockModel::getMap() const
{
    return map;
}

int MockModel::getUavCount() const
{
    return uavCount;
}

void MockModel::setMap(int m)
{
    map = m;
}
