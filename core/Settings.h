#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QDebug>
#include <QFile>
#include <QString>

#include <SettingsInterface.h>
#include <Exception.h>

class Settings: public SettingsInterface
{
public:
	Settings(const QString&) throw (Exception);

	QVariant value(const QString&);

private:
	QVariantMap values_;
};

#endif
