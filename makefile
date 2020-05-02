CC := clang

CPPFLAGS += -D "dbgv(x)=printf(\#x\"=%lld\n\",(long long)x);"
CPPFLAGS += -MP -MMD

CFLAGS += -std=c18 -Wall -Werror -Wfatal-errors -O3 -fomit-frame-pointer -march=native -mtune=native

LDLIBS += -L /usr/local/lib -l:libdoublefann.dll.a

all: xor_train.exe xor_execute.exe

xor_train.exe : xor_train.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include xor_train.d

xor_execute.exe : xor_execute.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include xor_execute.d

.PHONY: clean
clean:
	@rm -f *.o *.d xor_train.exe xor_execute.exe xor.net