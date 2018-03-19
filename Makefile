CXXFLAGS=-std=c++11 -Wall -W

all: binding.o return_lambda.o complex_dispatch.o brace_init.o
	$(CXX) $(CXXFLAGS) -o binding binding.o $(LDFLAGS)
	$(CXX) $(CXXFLAGS) -o return_lambda return_lambda.o $(LDFLAGS)
	$(CXX) $(CXXFLAGS) -o complex_dispatch complex_dispatch.o $(LDFLAGS)
	$(CXX) $(CXXFLAGS) -o brace_init brace_init.o $(LDFLAGS)

clean:
	rm -f *.o
