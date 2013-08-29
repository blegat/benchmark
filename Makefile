show-html-doc: doc
        xdg-open doc/html/index.html &
doc:
	doxygen Doxyfile

.PHONY: doc show-html-doc
