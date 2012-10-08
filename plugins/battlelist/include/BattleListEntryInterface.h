#ifndef BATTLELISTENTRYINTERFACE_H
#define BATTLELISTENTRYINTERFACE_H

#include <QString>

class BattleListEntryInterface {
public:
    virtual quint32 id() const = 0;

    virtual const char* rawName() const = 0;
    virtual QString name() const = 0;
};

#endif
