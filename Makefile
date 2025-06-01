.PHONY: all clean
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2
ASYNCSERVER = $(wildcard src/async/*.cpp)
SYNCSERVER = $(wildcard src/sync/*.cpp)
CLIENT = $(wildcard src/client/*.cpp)
OBJASYNC = $(ASYNCSERVER:.cpp=.o)
OBJSYNC = $(SYNCSERVER:.cpp=.o)
OBJCLIENT = $(CLIENT:.cpp=.o)
CLIENTEXE = client
ASYNCEXE = async_server
SYNCEXE = sync_server
BUILD = build

all: $(ASYNCEXE) $(SYNCEXE) $(CLIENTEXE)

$(ASYNCEXE): $(OBJASYNC) | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $(ASYNCEXE) $(BUILD)

$(SYNCEXE): $(OBJSYNC) | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $(SYNCEXE) $(BUILD)

$(CLIENTEXE): $(OBJCLIENT) | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^
	mv $(CLIENTEXE) $(BUILD)

$(BUILD):
	mkdir $(BUILD)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJASYNC) $(OBJSYNC) $(OBJCLIENT)
	rm -rf $(BUILD)