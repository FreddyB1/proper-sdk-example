#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <cstdio>
#include <experimental/filesystem>

#include "api.hpp"

struct HostNatives final: public NativesInterface {
	void print(const char* message) const {
		printf("%s\n", message);
	}
};

size_t loadPlugins(const NativesInterface& natives, std::experimental::filesystem::path path, std::experimental::filesystem::path ext) {
	size_t loaded = 0;
	for (auto& p: std::experimental::filesystem::recursive_directory_iterator(path)) {
		if (p.path().extension() == ext) {
			auto lib =
#ifdef _WIN32
				LoadLibraryW(p.path().c_str());
#else
				dlopen(p.path().c_str(), RTLD_NOW);
#endif
			if (!lib) {
				printf("Error: could not load plugin %s\n", p.path().generic_string().c_str());
				continue;
			}

			auto func = reinterpret_cast<SDK_GetPluginInterface_t>(
#ifdef _WIN32
				GetProcAddress(lib, "SDK_GetPluginInterface"));
#else
				dlsym(lib, "SDK_GetPluginInterface"));
#endif
			if (!func) {
				printf("Error: could not find SDK_GetPluginInterface for plugin %s\n", p.path().generic_string().c_str());
				continue;
			}

			PluginInterface& plugin = func();

			struct HostNativeAccessInterface final: public NativesAccessInterface
			{
				inline void setnatives(const NativesInterface* ptr) {
					nativesptr = ptr;
				}
			};
			auto& nativeaccess = static_cast<HostNativeAccessInterface&>((NativesAccessInterface&)plugin);
			nativeaccess.setnatives(&natives);

			plugin.OnModuleInit();
			++loaded;
		}
	}

	return loaded;
}

int main()
{
	HostNatives natives;
	printf("Loaded %lu plugin(s).", loadPlugins(natives, std::experimental::filesystem::current_path() / "plugins", ".plg"));

    return 0;
}
