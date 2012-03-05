#ifndef JSONCONFIG_H_
#define JSONCONFIG_H_

#include <QDebug>
#include <QFile>
#include <QString>

#include <ConfigInterface.h>
#include <Exception.h>

class JsonConfig: public ConfigInterface
{
public:
	JsonConfig(const QString&) throw (Exception);

	QVariant value(const QString&);

private:
	QVariantMap values_;
};

#endif
