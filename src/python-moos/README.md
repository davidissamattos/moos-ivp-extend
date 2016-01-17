python-moos
===========

python bindings for MOOS


#I cloned and compiled the code, and tried to run with python 2.7.10, but get the following error immediately upon creation of the pymoos.comms() object.

#Fatal Python error: PyEval_SaveThread: NULL tstate
#Abort trap: 6

#Am I missing something that needed to be done when compiling?
#@mandad
#mandad commented on Jul 29
#I fixed the problem, it had to do with compiling against a different version of Python than I was running the application (homebrew python on mac was not used as the default). It works with cmake -DPYTHON_LIBRARY="/usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/liblibpython2.7.dylib" ../run in the build directory (or whatever you python version is).
