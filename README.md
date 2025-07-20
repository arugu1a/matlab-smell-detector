# matlab-smell-detector

A small program that utilises tree-sitters syntax trees to search for common Code Smells in MATLAB code.

## About

This program recursively searches for all MATLAB files in a given directory utilising [tinydir](https://github.com/cxong/tinydir). For every file it constructs its concrete syntax tree using [tree-sitter](https://github.com/tree-sitter/tree-sitter) and [tree-sitter-matlab](https://github.com/acristoffers/tree-sitter-matlab). Different queries are then performed on the tree to detect patterns associated with smells. The current supported smells are:

- Long Function
- Long Parameter List

## Installation/Build

To build the program, follow these steps:

```shell
# acquire the code (e.g. clone using git)
git clone https://github.com/arugu1a/matlab-smell-detector.git

# move into the repository
cd matlab-smell-detector

# this command will clone the required third-party repositories 
# tree-sitter and tree-sitter-matlab into /third_party
# ! requires git
make setup

# compiles the binary main
make
```

## Usage

After building the program, the binary file **main** is the main entry point.

```shell
# search smells in the example_files
./main example_files
```

>**main** should only be executed from the project root directory. Otherwise the **config.ini** file cannot be found by the program.

To configure the smell detection thresholds, you can change them directly in the **config.ini** file. Just make sure to not add any whitespaces or to edit the key or section names. Changing thresholds doesn't require recompilation.

After the search is complete. The python script **<span>plot.py<span>** visualizes the occurence of found smells. The python script requires *pandas*.

```shell
# depending on your python installation run plot.py 
# to plot results from the generated output.csv
python3 plot.py
```

You can use the following command to remove the downloaded third-party libraries:

```shell
make clean all
```

## Acknowledgments and Licence

This project uses the following open source libraries:

- [tree-sitter](https://github.com/tree-sitter/tree-sitter) - parser generator tool and an incremental parsing library
- [tree-sitter-matlab](https://github.com/acristoffers/tree-sitter-matlab) - MATLAB grammar for tree-sitter
- [tinydir](https://github.com/cxong/tinydir) - lightweight directory traversal library

Their respective licences can be found in NOTICE.txt.

## Notes

- If you have compatibility issues between tree-sitter and the matlab grammar, try downgrading to these tested versions:
    - **tree-sitter 0.25.6**
    - **matlab grammar 1.0.6**

## TODO

- add link to quick start and licences
- transition to C++ to group smells into classes for better expandability
- add god class code smell
    - weighted methods per class (weighted cyclomatic complexity)
    - tight class cohesion
    - access to foreign data
- if WMC done with actual CFG, then dead code smell
should be easy to implement aswell
    -> so doing CFG is cooler but also harder
- add licence
- maybe make requirements more clear (git for make,
pandas for <span>plot.py<span>)
- add better examples
