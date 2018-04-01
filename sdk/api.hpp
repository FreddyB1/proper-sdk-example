#pragma once

struct NativesInterface
{
	virtual void print(const char* message) const=0;
};

class NativesAccessInterface
{
protected:
	const NativesInterface* nativesptr;

	inline const NativesInterface& natives() const {
		return *nativesptr;
	}
};

struct PluginInterface: private NativesAccessInterface
{
	using NativesAccessInterface::natives;

	virtual void OnModuleInit()=0;

};

typedef PluginInterface& (*SDK_GetPluginInterface_t)();

#ifdef _WIN32
#define PLUGIN_DESC(cls) static cls __##cls; extern "C" __declspec(dllexport) PluginInterface& SDK_GetPluginInterface() { return __##cls; }
#else
#define PLUGIN_DESC(cls) static cls __##cls; extern "C" PluginInterface& SDK_GetPluginInterface() { return __##cls; }
#endif
