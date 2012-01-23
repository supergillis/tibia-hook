#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <QTcpServer>

class Provider: public QTcpServer {
	Q_OBJECT

public:
	Provider();

public slots:
	void pendingConnection();
};

#endif /* PROVIDER_H_ */
