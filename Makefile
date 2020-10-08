CC = g++

# NS3LIB = /home/zliu0/usr/ns3-opt/lib/
# NS3INC = /home/zliu0/usr/ns3-opt/
# LDFLAGS = -L${NS3LIB} -lns3.31-core-optimized -lns3.31-point-to-point-optimized -lns3.31-tap-bridge-optimized -lns3.31-csma-optimized \
# -lns3.31-applications-optimized -lns3.31-bridge-optimized -lns3.31-internet-optimized -lns3.31-network-optimized -lns3.31-nix-vector-routing-optimized

NS3LIB = /home/zliu0/usr/ns3-debug/lib
NS3INC = /home/zliu0/usr/ns3-debug/
LDFLAGS = -L${NS3LIB} -lns3.31-core-debug -lns3.31-point-to-point-debug -lns3.31-tap-bridge-debug -lns3.31-csma-debug \
-lns3.31-applications-debug -lns3.31-bridge-debug -lns3.31-internet-debug -lns3.31-network-debug -lns3.31-nix-vector-routing-debug

CXXFLAGS = -std=c++11 -I${NS3INC}
CFLAGS = -I${NS3INC} 
LD = $(CC)

TARGET = FTNetSim 

OBJECTS = $(patsubst %.cc, %.o, $(shell find . -name "*.cc"))
DEPS = $(patsubst %.cc, %.d, $(shell find . -name "*.cc"))

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET) $(OBJECTS)
