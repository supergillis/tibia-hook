#ifndef MODULE_H_
#define MODULE_H_

#include <QObject>

class ScriptHandler;
class Module: public QObject {
	Q_OBJECT

public:
	Module(QObject* = NULL);

	virtual void install(ScriptHandler*) = 0;
};

#endif /* MODULE_H_ */
