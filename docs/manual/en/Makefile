
ADOC = asciidoctor

all:
  $(ADOC) manual.txt

clean:
  rm -f manual.html

auto:
  while true; do $(ADOC) manual.txt; \
      inotifywait -q -e close_write manual.txt; done
