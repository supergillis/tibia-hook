#include "JsonConfig.h"

#include <json.h>
#include <json.cpp>

JsonConfig::JsonConfig(const QString& json) throw (Exception) {
	bool parsed = true;
	QVariant result = QtJson::Json::parse(json, parsed);
	if(!parsed)
		throw Exception("Could not parse JSON string!");
	values_ = result.toMap();
}

QVariant JsonConfig::value(const QString& key) {
	return values_.value(key);
}
