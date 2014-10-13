#!/usr/bin/env python

import subprocess
import numpy
from datetime import datetime
from pylab import *
#plot, savefig, xlabel, ylabel, title, grid, show, mean, std, arange, figtext, autoscale, gcf, ylim

output = subprocess.check_output("./qtswaptest").decode("utf-8").splitlines()
figtext(0, 0, output[0])
output = output[1:]

data = list(map(lambda s: int(s) / 1000000.0, output))

notes  = "frames: {}\n".format(len(data))
notes += "average: {:.2f} ms/frame\n".format(mean(data))
notes += "stddev: {:.3f}".format(std(data))

plot(arange(len(data)), data, 'x')
autoscale(tight=True, axis="x")
ylim(0, 40 if max(data) < 40 else max(data))
figtext(0, 1, notes, va="top", size=10)
xlabel("Frame number")
ylabel("Frame time (ms)")
title("Frame times [qtswaptest]")
gcf().canvas.set_window_title("qtswaptest")
grid(True)
savefig("qtswaptest_{}.png".format(datetime.datetime.now().isoformat()))
show()
