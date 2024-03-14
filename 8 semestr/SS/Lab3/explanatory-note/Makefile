PDFLATEX = xelatex
BSUIR_REPORT_PDF = report
BIBTEX = bibtex
MAKEINDEX = makeindex
RM = rm -f

build:
	$(PDFLATEX) $(BSUIR_REPORT_PDF)
	$(MAKEINDEX) $(BSUIR_REPORT_PDF).nlo -s nomencl.ist -o $(BSUIR_REPORT_PDF).nls
	# $(BIBTEX) $(BSUIR_REPORT_PDF).aux
	# Used for master paper only
	# $(BIBTEX) My.aux
	# $(PDFLATEX) $(BSUIR_REPORT_PDF)
	# $(PDFLATEX) $(BSUIR_REPORT_PDF)
	# Citations count fixing
	$(PDFLATEX) $(BSUIR_REPORT_PDF)

cleanall: clean
	$(RM)  *.pdf

.PHONY: clean cleanall
clean:
	$(RM) *.aux *.log *.out *.toc *.gz *.gz\(busy\) *.blg *.bbl *.nlo *.nls *.fls *.xdv *.fdb_latexmk *.ilg content/*.aux

open:
	make clean
	make build
	xdg-open $(BSUIR_REPORT_PDF).pdf
