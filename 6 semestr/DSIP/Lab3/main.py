# 	№	Signal					Algorithm				 	N
# 	2	y=sin(3x)+cos(x)	БПФ с прореживанием по частоте	16

from ctypes import c_bool
import numpy as np
from matplotlib import pyplot as plt

# Non fft signals
N_original = 16
x = np.linspace(0, 2 * np.pi, N_original)
y = np.sin(3 * x) + np.cos(x)


def walsh_transform(signal):
    N = len(signal)
    if N == 1:
        return signal

    even = walsh_transform(signal[::2])
    odd = walsh_transform(signal[1::2])
    y = np.concatenate([even + odd, even - odd])
    return y


def inverse_walsh_transform(signal):
    N = len(signal)
    if N == 1:
        return signal

    even = inverse_walsh_transform(signal[:N//2])
    odd = inverse_walsh_transform(signal[N//2:])
    y = np.empty(N, dtype=np.int32)
    y[::2] = (even + odd) // 2
    y[1::2] = (even - odd) // 2
    return y


def showPlots():
    # Creating graphic of default signal
    fig, axs = plt.subplots(1, 3, figsize=(9, 6))
    plt.gcf().canvas.manager.set_window_title("Lab 3 (powered by raik199x + LeGushka)")

    # Plot original signal
    axs[0].plot(x, y)
    axs[0].set_title("Original Signal y=sin(3x)+cos(x)")
    axs[0].set_xlabel("Time")
    axs[0].set_ylabel("Amplitude")

    # walsh transform
    """c = walsh_transform(y)
    axs[1].bar(x, np.sign(c), width=0.5)
    axs[1].set_title("Walsh transform")
    axs[1].set_xlabel("Time")
    axs[1].set_ylabel("Amplitude")

    axs[2].step(x, sympy.fwht(y))
    axs[2].set_title("Walsh numpy transform")
    axs[2].set_xlabel("Time")
    axs[2].set_ylabel("Amplitude")"""

    # walsh ?
    axs[1].step(x, walsh_transform(y))
    axs[1].set_title("Walsh transform")
    axs[1].set_xlabel("Time")
    axs[1].set_ylabel("Amplitude")

    axs[2].plot(x, inverse_walsh_transform(walsh_transform(y)))
    axs[2].set_title("Revert")
    axs[2].set_xlabel("Time")
    axs[2].set_ylabel("Amplitude")

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    showPlots()
