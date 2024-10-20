###############################################################################
#  Copyright Christopher Kormanyos 1999 - 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/mp_cpp
# make GCC=g++ STD=c++11 clean
# make GCC=g++ STD=c++11 prepare
# make GCC=g++ STD=c++11 --jobs=8 all

# To run sanitizers
# make GCC=g++ STD=c++11 SAN=1 clean
# make GCC=g++ STD=c++11 SAN=1 prepare
# make GCC=g++ STD=c++11 SAN=1 --jobs=8 all

MY_GCC            = g++
MY_STD            = c++20
MY_SAN            =

ifneq ($(GCC),)
MY_GCC           := $(GCC)
endif

ifneq ($(STD),)
MY_STD           := $(STD)
endif

ifneq ($(SAN),)
MY_SAN           := $(SAN)
endif

CXX               = $(MY_GCC)
CC                = $(MY_GCC)
ECHO              = echo

PATH_UNIX         = $(CURDIR)/unix
PATH_SRC          = $(CURDIR)
PATH_OBJ          = $(PATH_UNIX)/obj
PATH_FFTW         = $(CURDIR)/fft/FFTW/215

CINCLUDES         = -I.                                       \
                    -I$(PATH_FFTW)/fftw                       \
                    -I$(PATH_FFTW)/rfftw

ASAN_FLAGS        = -fsanitize=address                        \
                    -fsanitize=leak

UBSAN_FLAGS       = -fsanitize=shift                          \
                    -fsanitize=shift-exponent                 \
                    -fsanitize=shift-base                     \
                    -fsanitize=integer-divide-by-zero         \
                    -fsanitize=null                           \
                    -fsanitize=signed-integer-overflow        \
                    -fsanitize=bounds                         \
                    -fsanitize=alignment                      \
                    -fsanitize=float-divide-by-zero           \
                    -fsanitize=float-cast-overflow            \
                    -fsanitize=enum

GCCFLAGS          = -Werror                                   \
                    -Wall                                     \
                    -Wextra                                   \
                    -Wpedantic                                \
                    -O3                                       \
                    -march=native                             \
                    -mtune=native                             \
                    $(CINCLUDES)

ifneq ($(MY_SAN),)
GCCFLAGS         := $(GCCFLAGS)                               \
                    $(ASAN_FLAGS)                             \
                    $(UBSAN_FLAGS)
endif

CFLAGS            = $(GCCFLAGS)                               \
                    -Wno-strict-prototypes                    \
                    -std=c11                                  \
                    -x c

CXXFLAGS          = --std=$(MY_STD)                           \
                    -Wconversion                              \
                    -Wsign-conversion                         \
                    -Wshadow                                  \
                    -Wundef                                   \
                    -Wunused-parameter                        \
                    -Wuninitialized                           \
                    -Wunreachable-code                        \
                    -Winit-self                               \
                    -Wzero-as-null-pointer-constant           \
                    $(GCCFLAGS)

LDFLAGS           = $(GCCFLAGS)                               \
                    -lpthread

FFTW_OBJ          = $(addprefix $(PATH_OBJ)/,$(basename $(notdir $(wildcard $(PATH_FFTW)/fftw/*.c))))
#*/
FFTW_OBJ         := $(addsuffix .o,$(FFTW_OBJ))

RFFTW_OBJ         = $(addprefix $(PATH_OBJ)/,$(basename $(notdir $(wildcard $(PATH_FFTW)/rfftw/*.c))))
#*/
RFFTW_OBJ        := $(addsuffix .o,$(RFFTW_OBJ))

FFTW_THREADS_OBJ  = $(addprefix $(PATH_OBJ)/,$(basename $(notdir $(wildcard $(PATH_FFTW)/threads/*.c))))
#*/
FFTW_THREADS_OBJ := $(addsuffix .o,$(FFTW_THREADS_OBJ))

FILES_OBJ         = $(FFTW_OBJ)                               \
                    $(RFFTW_OBJ)                              \
                    $(FFTW_THREADS_OBJ)                       \
                    $(PATH_OBJ)/mp_fft_fftw2.o                \
                    $(PATH_OBJ)/mp_fft_multiply.o             \
                    $(PATH_OBJ)/mp_base.o                     \
                    $(PATH_OBJ)/mp_convert.o                  \
                    $(PATH_OBJ)/mp_core.o                     \
                    $(PATH_OBJ)/mp_cpp.o                      \
                    $(PATH_OBJ)/mp_cpp_limits.o               \
                    $(PATH_OBJ)/mp_detail.o                   \
                    $(PATH_OBJ)/mp_func.o                     \
                    $(PATH_OBJ)/mp_hyp.o                      \
                    $(PATH_OBJ)/mp_num.o                      \
                    $(PATH_OBJ)/mp_op.o                       \
                    $(PATH_OBJ)/mp_pow.o                      \
                    $(PATH_OBJ)/mp_trig.o                     \
                    $(PATH_OBJ)/bessel.o                      \
                    $(PATH_OBJ)/gamma.o                       \
                    $(PATH_OBJ)/ln2.o                         \
                    $(PATH_OBJ)/pi.o                          \
                    $(PATH_OBJ)/test.o

MP_HEADERS       =  $(PATH_SRC)/mp/mp_base.h                              \
                    $(PATH_SRC)/mp/mp_complex.h                           \
                    $(PATH_SRC)/mp/mp_core.h                              \
                    $(PATH_SRC)/mp/mp_core_memory.h                       \
                    $(PATH_SRC)/mp/mp_cpp.h                               \
                    $(PATH_SRC)/mp/mp_cpp_limits.h                        \
                    $(PATH_SRC)/mp/mp_detail.h                            \
                    $(PATH_SRC)/mp/mp_detail_pown_template.h              \
                    $(PATH_SRC)/mp/mp_math.h                              \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_base.h                   \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_fftw2.h                  \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_fftw2_protos.h           \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_multiply.h               \
                    $(PATH_SRC)/samples/samples.h


###############################################################
#
# The main build target.
#
###############################################################

.PHONY: all
all: $(PATH_UNIX)/test.exe

###############################################################
#
# Make the temporary directories if not yet present.
#
###############################################################

.PHONY: UNIX_DIR
UNIX_DIR :
	@echo
	@echo Create temporary directories
	@mkdir -p $(PATH_UNIX)
	@mkdir -p $(PATH_OBJ)

###############################################################
#
# Print GNUmake and compiler versions.
# Make the temporary directories if not yet present.
#
###############################################################

.PHONY: prepare
prepare : UNIX_DIR
	@echo
	@echo Print GNUmake version
	@make -v
	@echo
	@echo Print compiler version
	@$(CXX) -v
	@echo
	@echo Print compiler C++ flags
	@echo $(CXXFLAGS)
	@echo

###############################################################
#
# Compile the mp files.
#
###############################################################

$(PATH_OBJ)/mp_base.o: $(PATH_SRC)/mp/mp_base.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_convert.o: $(PATH_SRC)/mp/mp_convert.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_core.o: $(PATH_SRC)/mp/mp_core.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_cpp.o: $(PATH_SRC)/mp/mp_cpp.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_cpp_limits.o: $(PATH_SRC)/mp/mp_cpp_limits.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $<  -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_detail.o: $(PATH_SRC)/mp/mp_detail.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $<  -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_func.o: $(PATH_SRC)/mp/mp_func.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c mp/mp_func.cpp -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_hyp.o: $(PATH_SRC)/mp/mp_hyp.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $<  -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_num.o: $(PATH_SRC)/mp/mp_num.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_op.o: $(PATH_SRC)/mp/mp_op.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_pow.o: $(PATH_SRC)/mp/mp_pow.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_trig.o: $(PATH_SRC)/mp/mp_trig.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_fft_fftw2.o: $(PATH_SRC)/mp/mp_fft/mp_fft_fftw2.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c mp/mp_fft/mp_fft_fftw2.cpp -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/mp_fft_multiply.o: $(PATH_SRC)/mp/mp_fft/mp_fft_multiply.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

###############################################################
#
# Compile the sample files.
#
###############################################################

$(PATH_OBJ)/bessel.o: $(PATH_SRC)/samples/bessel.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/gamma.o: $(PATH_SRC)/samples/gamma.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/ln2.o: $(PATH_SRC)/samples/ln2.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/pi.o: $(PATH_SRC)/samples/pi.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/test.o: $(PATH_SRC)/samples/test.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

###############################################################
#
# Compile the fftw, the rfftw and the fftw-threads files using pattern rules.
#
###############################################################

$(PATH_OBJ)/%.o : $(PATH_FFTW)/rfftw/%.c
	@$(ECHO) +++ compile: $< to $@
	@$(CC) $(CFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/%.o : $(PATH_FFTW)/fftw/%.c
	@$(ECHO) +++ compile: $< to $@
	@$(CC) $(CFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/%.o : $(PATH_FFTW)/threads/%.c
	@$(ECHO) +++ compile: $< to $@
	@$(CC) $(CFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

###############################################################
#
# Compile the sample files containing the main() subroutines.
#
###############################################################

$(PATH_OBJ)/bessel_main.o : $(PATH_SRC)/bessel/bessel_main.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/gamma_main.o : $(PATH_SRC)/gamma/gamma_main.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/ln2_main.o : $(PATH_SRC)/ln2/ln2_main.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/pi_main.o : $(PATH_SRC)/pi/pi_main.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

$(PATH_OBJ)/test_main.o : $(PATH_SRC)/test/test_main.cpp $(MP_HEADERS)
	@$(ECHO) +++ compile: $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $(PATH_OBJ)/$(basename $(@F)).o

###############################################################
#
# Link the projects.
#
###############################################################

$(PATH_UNIX)/bessel.exe : $(FILES_OBJ) $(PATH_OBJ)/bessel_main.o $(PATH_OBJ)/bessel.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/bessel_main.o -o $(PATH_UNIX)/bessel.exe

$(PATH_UNIX)/gamma.exe  : $(FILES_OBJ) $(PATH_UNIX)/bessel.exe $(PATH_OBJ)/gamma_main.o $(PATH_OBJ)/gamma.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/gamma_main.o -o $(PATH_UNIX)/gamma.exe

$(PATH_UNIX)/ln2.exe    : $(FILES_OBJ) $(PATH_UNIX)/bessel.exe $(PATH_UNIX)/gamma.exe $(PATH_OBJ)/ln2_main.o $(PATH_OBJ)/ln2.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/ln2_main.o -o $(PATH_UNIX)/ln2.exe

$(PATH_UNIX)/pi.exe     : $(FILES_OBJ) $(PATH_UNIX)/bessel.exe $(PATH_UNIX)/gamma.exe $(PATH_UNIX)/ln2.exe $(PATH_OBJ)/pi_main.o $(PATH_OBJ)/pi.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/pi_main.o -o $(PATH_UNIX)/pi.exe

$(PATH_UNIX)/test.exe   : $(FILES_OBJ) $(PATH_UNIX)/pi.exe $(PATH_UNIX)/bessel.exe $(PATH_UNIX)/gamma.exe $(PATH_UNIX)/ln2.exe  $(PATH_OBJ)/test_main.o $(PATH_OBJ)/test.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/test_main.o -o $(PATH_UNIX)/test.exe

.PHONY: clean
clean:
	rm -r $(PATH_OBJ)/*.o
