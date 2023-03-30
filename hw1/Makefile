CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic

SRCDIR=.
OBJDIR=obj
BINDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCS))
EXEC=$(BINDIR)/main

$(EXEC): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(BINDIR)
