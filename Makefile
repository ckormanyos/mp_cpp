###############################################################################
#  Copyright Christopher Kormanyos 1999 - 2023.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/ExtendedNumberTypes/mp_cpp
# make --jobs=8 all

MY_STD            = c++20

ifneq ($(STD),)
MY_STD            = $(STD)
endif

CXX               = g++
CC                = gcc
ECHO              = echo

PATH_UNIX         = $(CURDIR)/unix
PATH_SRC          = $(CURDIR)
PATH_OBJ          = $(PATH_UNIX)/obj
PATH_FFTW         = $(CURDIR)/fft/FFTW/215

CINCLUDES         = -I.                                       \
                    -I$(PATH_FFTW)/fftw                       \
                    -I$(PATH_FFTW)/rfftw

GCCFLAGS          = -Wall                                     \
                    -Wextra                                   \
                    -Wodr                                     \
                    -Wpedantic                                \
                    -O3                                       \
                    -march=native                             \
                    -mtune=native                             \
                    $(CINCLUDES)

CFLAGS            = $(GCCFLAGS)                               \
                    -std=c11

CXXFLAGS          = --std=$(MY_STD)                           \
                    $(GCCFLAGS)

LDFLAGS           = $(CFLAGS)                                 \
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
                    $(PATH_OBJ)/mp_fft_base.o                 \
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
                    $(PATH_SRC)/mp/mp_cpp.h                               \
                    $(PATH_SRC)/mp/mp_cpp_limits.h                        \
                    $(PATH_SRC)/mp/mp_detail.h                            \
                    $(PATH_SRC)/mp/mp_detail_pown_template.h              \
                    $(PATH_SRC)/mp/mp_math.h                              \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_base.h                   \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_fftw2.h                  \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_fftw2_protos.h           \
                    $(PATH_SRC)/mp/mp_fft/mp_fft_multiply.h               \
                    $(PATH_SRC)/samples/gamma.h                           \
                    $(PATH_SRC)/samples/samples.h


.PHONY: all
all: UNIX_DIR $(PATH_UNIX)/bessel.exe $(PATH_UNIX)/gamma.exe $(PATH_UNIX)/ln2.exe $(PATH_UNIX)/pi.exe $(PATH_UNIX)/test.exe

###############################################################
#
# Print GNUmake and compiler versions.
#
###############################################################

.PHONY: version
version :
	@echo
	@echo GNUmake version
	@make -v
	@echo
	@echo Print compiler version
	@$(CXX) -v
	@echo Print compiler C++ flags
	@echo $(CXXFLAGS)

###############################################################
#
# Make the temporary directories if not yet present.
#
###############################################################

.PHONY: UNIX_DIR
UNIX_DIR : version
	@echo
	@echo Create temporary directories
	@mkdir -p $(PATH_UNIX)
	@mkdir -p $(PATH_OBJ)

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

$(PATH_OBJ)/mp_fft_base.o: $(PATH_SRC)/mp/mp_fft/mp_fft_base.cpp $(MP_HEADERS)
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

$(PATH_UNIX)/bessel.exe :   $(FILES_OBJ)  $(PATH_OBJ)/bessel_main.o $(PATH_OBJ)/bessel.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/bessel_main.o -o $(PATH_UNIX)/bessel.exe

$(PATH_UNIX)/gamma.exe  :   $(FILES_OBJ) $(PATH_OBJ)/gamma_main.o $(PATH_OBJ)/gamma.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/gamma_main.o -o $(PATH_UNIX)/gamma.exe

$(PATH_UNIX)/ln2.exe    :   $(FILES_OBJ) $(PATH_OBJ)/ln2_main.o $(PATH_OBJ)/ln2.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/ln2_main.o -o $(PATH_UNIX)/ln2.exe

$(PATH_UNIX)/pi.exe     :   $(FILES_OBJ) $(PATH_OBJ)/pi_main.o $(PATH_OBJ)/pi.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/pi_main.o -o $(PATH_UNIX)/pi.exe

$(PATH_UNIX)/test.exe   :   $(FILES_OBJ) $(PATH_OBJ)/test_main.o $(PATH_OBJ)/test.o
	@$(ECHO) +++ linking executable: $@
	@$(CXX) $(LDFLAGS) $(FILES_OBJ) -x none $(PATH_OBJ)/test_main.o -o $(PATH_UNIX)/test.exe

.PHONY: clean
clean:
	rm -r $(PATH_OBJ)/*.o
