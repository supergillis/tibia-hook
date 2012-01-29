#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QObject>
#include <QStringList>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptEngineAgent>

#include "Handler.h"
#include "EncryptedMessage.h"
#include "Module.h"

class ScriptHandler: public Handler {
	Q_OBJECT

public:
	ScriptHandler(QObject* = 0);

	QScriptEngine* scriptEngine();
	const QScriptEngine* scriptEngine() const;

	QScriptValue classObject();
	const QScriptValue classObject() const;

	void install(Module*);
	void reload();
	void require(const QString&);

	void receiveFromClient(const EncryptedMessage*);
	void receiveFromServer(const EncryptedMessage*);

	QScriptValue createClassPrototype();
	QScriptValue createClass(QScriptValue);
	QScriptValue createClass(QScriptValue, QScriptValue);
	QScriptValue createInstancePrototype();
	QScriptValue createInstance(QScriptValue, QScriptValue);

private:
	void initializeClientObject();
	void initializeMemoryObject();
	void initializeNetworkObject();
	void initializePacketObject();

	bool receiveFromClientInternal(const EncryptedMessage*);

	QScriptEngine _engine;
	QScriptValue _classObject;
	QScriptValue _packetObject;
	QScriptValue _networkObject;

	QScriptString _instanceHandle;
	QScriptString _constructorHandle;
	QScriptString _extendHandle;
	QScriptString _createHandle;
	QScriptString _extendedHandle;

	QScriptString _receiveFromClientHandle;
	QScriptString _receiveFromServerHandle;

	QStringList _requiredFiles;
};

namespace Handlers {
	namespace Class {
		static QScriptValue extend(QScriptContext*, QScriptEngine*);
		static QScriptValue create(QScriptContext*, QScriptEngine*);
		static QScriptValue extended(QScriptContext*, QScriptEngine*);
		static QScriptValue constructor(QScriptContext*, QScriptEngine*);
	};

	namespace Network {
		static QScriptValue sendToServer(QScriptContext*, QScriptEngine*);
		static QScriptValue sendToClient(QScriptContext*, QScriptEngine*);
	};

	namespace Client {
		static QScriptValue sendPacket(QScriptContext*, QScriptEngine*);
		static QScriptValue sendKeyPress(QScriptContext*, QScriptEngine*);
	};

	namespace PacketRead {
		static QScriptValue readU8(QScriptContext*, QScriptEngine*);
		static QScriptValue readU16(QScriptContext*, QScriptEngine*);
		static QScriptValue readU32(QScriptContext*, QScriptEngine*);
		static QScriptValue readString(QScriptContext*, QScriptEngine*);
	};

	namespace PacketWrite {
		static QScriptValue constructor(QScriptContext*, QScriptEngine*);
		static QScriptValue writeU8(QScriptContext*, QScriptEngine*);
		static QScriptValue writeU16(QScriptContext*, QScriptEngine*);
		static QScriptValue writeU32(QScriptContext*, QScriptEngine*);
		static QScriptValue writeString(QScriptContext*, QScriptEngine*);
	}

	namespace Memory {
		static QScriptValue readU8(QScriptContext*, QScriptEngine*);
		static QScriptValue readU16(QScriptContext*, QScriptEngine*);
		static QScriptValue readU32(QScriptContext*, QScriptEngine*);
		static QScriptValue readString(QScriptContext*, QScriptEngine*);
	};
};

#endif /* SCRIPTHANDLER_H_ */
