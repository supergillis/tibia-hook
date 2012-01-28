#ifndef SCRIPTENGINE_H_
#define SCRIPTENGINE_H_

#include <QString>
#include <QStringList>
#include <QFile>
#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QScriptContext>

class ScriptEngine: public QScriptEngine, public QScriptEngineAgent {
	Q_OBJECT

public:
	ScriptEngine(QObject* = 0);

	QScriptValue require(const QString&);

	void clearRequiredFiles();

	void exceptionThrow(qint64, const QScriptValue&, bool);

private:
	QStringList _required;
};

#endif /* SCRIPTENGINE_H_ */
