CXX = g++

#CXXFLAGS += -D_WIN32 # Если компиляция происходит под Windows
CXXFLAGS += -Dlinux # Если компиляция происходит под Linux

# Общие флаги
CXXFLAGS += -O3
CXXFLAGS += -std=c++14

# Сильно увеличивает размер, но делает проги независимыми на чужом компе
LINKFLAGS += -static-libgcc -static-libstdc++

# Если эту опцию поместить перед mwindows, то получится консоль с окном
LINKFLAGS += -mconsole 

# Оптимизация размера
LINKFLAGS += -s 
SIZEFLAGS += -Wl,--gc-sections -Wl,--strip-all 
SIZEFLAGS += -fdata-sections -ffunction-sections
SIZEFLAGS += -ffast-math
SIZEFLAGS += -Wunused
SIZEFLAGS += -flto

INCLUDES += keyboard_listen.h

mini_typing_statistics: mini_typing_statistics.o keyboard_listen.o
	$(CXX) $(SIZEFLAGS) $(CXXFLAGS) mini_typing_statistics.o keyboard_listen.o -o mini_typing_statistics

%.o: %.cpp $(INCLUDES)
	$(CXX) $(SIZEFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm typing_statistics.o 
	rm keyboard_listen.o 
	rm typing_statistics