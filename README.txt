NAME
       mkdoc

DESCRIPTION
       Initialize a basic LaTeX project directory (Makefile 
       included).

       Useful for creating PDF documents with relative ease. The
       LaTeX compiler being used is "pdflatex" from the "texlive"
       package, which is available on most Linux distributions.

       Running "make clean" will remove the ".log" and ".aux"
       files while leaving the PDF alone.

USAGE
       mkdoc [name]         Create a project called [name].
       
       mkdoc -h, --help     Print a brief usage summary.

EXAMPLES
       mkdoc my_document
       
       mkdoc "My Document"

       mkdoc Desktop/work/my_document

       mkdoc "Documents/My Document"

LICENSE
       GNU General Public License, Version 3.0

AUTHOR
       Justin Roach, https://github.com/justinr835/mkdoc
