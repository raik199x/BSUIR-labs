# 	№	Signal				N
# 	2	y=sin(3x) z=cos(x)	16
"""
TODO:
> matrix view of BPF
> complex conjugation
> Tensor product
"""

import numpy as np
import correlation
import convolution
from matplotlib import pyplot as plt
from fft import fft, ifft

#! WARNING: in functions to avoid ambiguity z = x, y = y

# Non fft signals
N = 1001
x = np.linspace(0, 2 * np.pi, N)
y = np.sin(3 * x)
z = np.cos(x)
# fft signals
fftN = 16
xFFT = np.linspace(0, 2 * np.pi, fftN)
yFFT = np.sin(3 * xFFT)
zFFt = np.cos(xFFT)

# remember, baseSignalX = z
def drawPlot(
    baseSignalX,
    baseSignalY,
    selfMadeConvolution,
    numpyConvolution,
    selfMadeCorrelation,
    numpyCorrelation,
    convFFT,
    corrFFT,
):
    plt.figure(figsize=(13, 7))
    plt.gcf().canvas.manager.set_window_title("Lab 2 (powered by raik199x + LeGushka)")

    # 1 Base signals
    plt.subplot(2, 4, 1)
    plt.title("y = sin(3x)")
    plt.plot(baseSignalY)

    plt.subplot(2, 4, 5)
    plt.title("z = cos(x)")
    plt.plot(baseSignalX)

    # 2 Convolution (our + numpy)
    plt.subplot(2, 4, 3)
    plt.plot(selfMadeConvolution)
    plt.title("Convolution")

    plt.subplot(2, 4, 7)
    plt.title("Convolution (NumPy)")
    plt.plot(numpyConvolution)

    # 3 Correlation (our + numpy)
    plt.subplot(2, 4, 4)
    plt.title("Correlation")
    plt.plot(selfMadeCorrelation)

    plt.subplot(2, 4, 8)
    plt.title("Correlation (NumPy)")
    plt.plot(numpyCorrelation)

    # 4 Convolution + Correlation (our but FFT)
    plt.subplot(2, 4, 2)
    plt.title("Convolution via FFT")
    plt.plot(convFFT)

    plt.subplot(2, 4, 6)
    plt.title("Correlation via FFT")
    plt.plot(corrFFT)

    plt.show()


if __name__ == "__main__":
    drawPlot(
        z,
        y,
        convolution.convolution(z, y, N),
        convolution.NumPyConvolution(z, y),
        correlation.correlation(z, y, N),
        correlation.NumPyCorrelation(z, y),
        convolution.convolutionFFT(z, y),
        correlation.correlationFFT(z, y),
    )
