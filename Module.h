#ifndef MODULE_H_
#define MODULE_H_

#include <QObject>
#include <QScriptEngine>

class Module: public QObject {
	Q_OBJECT

public:
	Module(QObject* = NULL);

	virtual void install(QScriptEngine*) = 0;
};

#endif /* MODULE_H_ */
