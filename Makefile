CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD -MP -I./src -I./src/scheduler -I./src/data_structures/rb_tree

SRC = \
    src/main.cpp \
    src/Process.cpp \
    src/scheduler/Scheduler.cpp \
    src/scheduler/CFS.cpp \
    src/scheduler/RoundRobin.cpp \
    src/data_structures/rb_tree/RBTree.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = cfs_scheduler

-include $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(OBJ:.o=.d)