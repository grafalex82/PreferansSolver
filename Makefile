#This file is a common makefile for directory. It performs dependencies update and also running the
#Makefile.sub to actual build process. It also performs correct cleanup of this part of project

include Makefile.defs

all: cardsCore
	@cp -fp $(CORE_DLL_FILE) $(LIB_DIR)

cardsCore: $(OBJS_DIR) $(DEPS_DIR) $(DEPS)
	$(MAKE) -f Makefile.sub $(CORE_DLL_FILE)

$(DEPS_DIR):
	mkdir $(DEPS_DIR)

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean: FORCE
	-@rm -rf $(OBJS_DIR)
	-@rm -rf $(DEPS_DIR)
	-@rm -f $(LIB_DIR)/$(CORE_DLL)

FORCE:
	

$(DEPS_DIR)/%.d: $(SRC_DIR)/%.cpp
	@if g++ $(CXXFLAGS) -MM -MT $(OBJS_DIR)/$(*F).o -MF $@ $< ; then \
	    echo Created dependencies for $< ;\
	else \
	    echo Failed dependencies for $< ;\
	    rm -f $@ ;\
	    exit 1 ;\
	fi
