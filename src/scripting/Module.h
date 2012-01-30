#ifndef MODULE_H_
#define MODULE_H_

#include <QString>

class ScriptHandler;
class Module {
public:
	virtual QString name() const = 0;

	bool install(ScriptHandler*);

protected:
	ScriptHandler* handler() const;

	virtual void install() = 0;

private:
	ScriptHandler* handler_;
};

#endif /* MODULE_H_ */
