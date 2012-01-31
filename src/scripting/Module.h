#ifndef MODULE_H_
#define MODULE_H_

#include <QObject>
#include <QString>

class ModuleManager;
class Module: public QObject {
public:
	Module(QObject*);

	virtual QString name() const = 0;
	virtual bool install(ModuleManager*) = 0;
};

#endif /* MODULE_H_ */
