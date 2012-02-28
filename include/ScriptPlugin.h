#ifndef SCRIPTPLUGIN_H_
#define SCRIPTPLUGIN_H_

#ifdef Q_WS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define EXPORT_FUNCTION_NAME load
#define EXPORT_FUNCTION_STRING "load"

#define EXPORT_PLUGIN(className) \
	extern "C" EXPORT ScriptPluginInterface* EXPORT_FUNCTION_NAME() { \
		return new className(); \
	}

#endif
