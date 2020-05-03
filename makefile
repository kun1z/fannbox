CC := @clang

CPPFLAGS += -D "dbgv(x)=printf(\#x\"=%lld\n\",(long long)x);"
CPPFLAGS += -MP -MMD

CFLAGS += -std=c18 -Wall -Werror -Wfatal-errors -O3 -fomit-frame-pointer -march=native -mtune=native

LDLIBS += -L /usr/local/lib -l:libdoublefann.dll.a

all: fann_train.exe fann_execute.exe

fann_train.exe : fann_train.o blake2bmod.o common.o
	@$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include fann_train.d blake2bmod.d common.d

fann_execute.exe : fann_execute.o blake2bmod.o common.o
	@$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include fann_execute.d blake2bmod.d common.d

.PHONY: clean
clean:
	@rm -f *.o *.d fann_train.exe fann_execute.exe