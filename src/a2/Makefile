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
LIB_EECS467 = $(LIB_PATH)/libeecs467.a

#LIB_A1 = $(LIB_PATH)/liba1.a
#LIB_A1_OBJ = occupancy_map.o action_model.o particle_data.o maebot_data.o sensor_model.o odometry_matcher.o path_planning.o 
#pose_tracker.o 

LIBDEPS  = $(call libdeps, $(LDFLAGS))

BIN_A2_MASK = $(BIN_PATH)/a2_mask
ALL = $(BIN_A2_MASK)

all: $(ALL)

#$(LIB_A1): $(LIB_A1_OBJ) $(LIBDEPS)
#	@echo "\t$@"
#	@ar rc $@ $^

$(BIN_A2_MASK): a2_mask.o $(LIB_EECS467) $(LIBDEPS)
	@echo "\t$@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f *.o *~ *.a
	@rm -f $(ALL)
