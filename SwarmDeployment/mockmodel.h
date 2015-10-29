#ifndef MOCKMODEL_H
#define MOCKMODEL_H


class MockModel
{
public:
    MockModel();
    int obstacle0x;
    int obstacle0y;
    int obstacle0height;
    int obstacle0width;

    int obstacle1x;
    int obstacle1y;
    int obstacle1height;
    int obstacle1width;

    int obstacle2x;
    int obstacle2y;
    int obstacle2height;
    int obstacle2width;

    int obstacle3x;
    int obstacle3y;
    int obstacle3height;
    int obstacle3width;


    int uavCount;

    int getUavCount() const;
    int getMap() const;

    void setMap(int m);

protected:
    int map;
};

#endif // MOCKMODEL_H
