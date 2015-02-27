include ../common.mk

CXXFLAGS = $(CXXFLAGS_STD) \
	$(CFLAGS_VX_GTK) \
	$(CFLAGS_IMAGESOURCE) \
     $(CFLAGS_MATH) \
     $(CFLAGS_COMMON) \
     $(CFLAGS_LCMTYPES) \
     $(CFLAGS_GSL) \
     -O2 -w
LDFLAGS  = $(LDFLAGS_STD) \
	$(LDFLAGS_VX_GTK) \
	  $(LDFLAGS_IMAGESOURCE) \
	  $(LDFLAGS_MATH) \
	  $(LDFLAGS_COMMON) \
		$(LDFLAGS_LCMTYPES) \
		$(LDFLAGS_GSL)

#LIB_MAEBOT_DATA = $(LIB_PATH)/libmaebotdata.a
#LIB_MAEBOT_DATA_OBJ = maebot_data.o

#LIB_LASER_MATCHER = $(LIB_PATH)/lasermatcher.a
#LIB_LASER_MATCHER_OBJ = laser_matcher.o

#LIB_OCCUPANCY_MAP = $(LIB_PATH)/occupancymap.a
#LIB_OCCUPANCY_MAP_OBJ = occupancy_map.o


#LIB_MAPPING = $(LIB_PATH)/libmapping.a

LIB_A2 = $(LIB_PATH)/liba2.a
LIB_A2_OBJ = image_processor.o

LIBDEPS  = $(call libdeps, $(LDFLAGS))

BIN_A2_MASK = $(BIN_PATH)/a2_mask
ALL = $(LIB_A2) $(BIN_A2_MASK)

all: $(ALL)

$(LIB_A2): $(LIB_A2_OBJ) $(LIBDEPS)
	@echo "\t$@"
	@ar rc $@ $^

$(BIN_A2_MASK): a2_mask.o $(LIB_A2) $(LIBDEPS)
	@echo "\t$@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f *.o *~ *.a
	@rm -f $(ALL)
