#include "api.hpp"

struct ExamplePlugin final: public PluginInterface
{
	void OnModuleInit() {
		natives().print("OnModuleInit\n");
	}
};

PLUGIN_DESC(ExamplePlugin)
