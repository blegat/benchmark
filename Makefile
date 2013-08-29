OS := $(shell uname)
ifeq ($(OS), Darwin)
  OPEN = open
else
  OPEN = xdg-open
endif

show-html-doc: doc
	$(OPEN) doc/html/index.html &

doc:
	doxygen Doxyfile

.PHONY: doc show-html-doc
