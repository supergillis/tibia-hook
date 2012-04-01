#include "Settings.h"
#include "StringException.h"

#include <json.h>
#include <json.cpp>

Settings::Settings(const QString& json) throw (Exception) {
	bool parsed = true;
	QVariant result = QtJson::Json::parse(json, parsed);
	if(!parsed)
		throw StringException("Could not parse JSON string!");
	values_ = result.toMap();
}

QVariant Settings::value(const QString& key) {
	return values_.value(key);
}
