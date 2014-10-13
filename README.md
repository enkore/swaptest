Qt swap test
============

Based on https://github.com/sletta/stuff/tree/master/cpp/swaptest

- build: `qmake && make`
- run: `python runtest.py` (requires Python 3 + pylab)
- After 10 seconds of flickering you'll get a nice plot and some stats
- Automatically saves plot as `qtswaptest_<date+time>.png` in the current directory

I added the plotting script, because I can't look at that flickering for more
than a second before I get a headache...
