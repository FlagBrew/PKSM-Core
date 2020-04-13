SOURCES		:=	source \
				source/i18n \
				source/personal \
				source/pkx \
				source/sav \
				source/utils \
				source/wcx \
				swshcrypto

INCLUDES	:=	include \
				include/enums \
				include/personal \
				include/pkx \
				include/sav \
				include/utils \
				include/wcx \
				swshcrypto

format:
	clang-format -i -style=file $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp)) $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.h) $(wildcard $(dir)/*.hpp))
