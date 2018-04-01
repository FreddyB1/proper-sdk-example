#include "api.hpp"

struct ExamplePlugin final: public PluginInterface
{
	void OnModuleInit() {
		natives().print("OnModuleInit");
	}
};

PLUGIN_DESC(ExamplePlugin)
