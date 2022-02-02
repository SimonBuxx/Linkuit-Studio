#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QGraphicsItem>

class DataModel
{
public:
    DataModel();
    //void AddComponent(QGraphicsItem pComponent);

protected:
    std::vector<QGraphicsItem> mComponentsVector;
};

#endif // DATAMODEL_H
