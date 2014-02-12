XXFLAGS= -O2 
CXXFLAGS= -g 
SRC_DIRS=.
INCLUDE_DIRS=-I ./include
LIBS=-lpthread
all: tfidf

SRC=$(foreach srcdir, $(SRC_DIRS),$(wildcard $(srcdir)/*.cpp))
OBJS:=$(patsubst %.cpp, %.o, $(SRC))
vpath %.h $(INCLUDE_DIRS)
vpath %.cpp $(SRC_DIRS)

tfidf:     $(OBJS)
	$(CXX) $(CXXFLAGS) ${LIBS}    $^ -o  $@




.PHONY : clean
clean:
	-@rm -rf $(OBJS)  tfidf 
