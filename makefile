SDK_PATH = sdk/
CFLAGS = -std=c++17 -fno-rtti -fPIC -I$(SDK_PATH)

all: host plugin

host: host.o
	g++ $(CFLAGS) host.o -lstdc++fs -ldl -o hostapp

host.o: host/host.cpp $(SDK_PATH)/api.hpp
	g++ $(CFLAGS) -c host/host.cpp -o host.o

plugin: plugin.o
	g++ $(CFLAGS) plugin.o -shared -o plugins/example.plg

plugin.o: plugin/dllmain.cpp $(SDK_PATH)/api.hpp
	g++ $(CFLAGS) -c -fPIC plugin/dllmain.cpp -o plugin.o

clean:
	rm plugins/example.plg
	rm hostapp
	rm *.o
