#ifndef SCRIPTPLUGIN_H_
#define SCRIPTPLUGIN_H_

#ifdef Q_WS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define EXPORT_PLUGIN(className) \
	extern "C" EXPORT ScriptPluginInterface* load() { \
		return new className(); \
	}

#endif
