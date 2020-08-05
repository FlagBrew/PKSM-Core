SOURCES		:=	source \
				source/i18n \
				source/personal \
				source/pkx \
				source/sav \
				source/utils \
				source/wcx

INCLUDES	:=	include \
				include/enums \
				include/personal \
				include/pkx \
				include/sav \
				include/utils \
				include/wcx

format:
	clang-format -i -style=file $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.hpp) $(wildcard $(dir)/*.h)) $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.h) $(wildcard $(dir)/*.hpp))
