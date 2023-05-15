# 	№	Signal					Algorithm				 	N
# 	2	y=sin(3x)+cos(x)	БПФ с прореживанием по частоте	16

import numpy as np
from matplotlib import pyplot as plt


def dft(y):
    # Initially, the length of the signal is calculated and stored in N variable
    N = len(y)
    # A numpy array 'c' of 0's with dtype complex is created with length (N).
    c = np.zeros(N, dtype=complex)

    # Two for loops are used to compute the Discrete Fourier Transform.
    # The outer loop constructed using k variable iterates through frequency indexes/ bins, from 0 to N-1.
    for k in range(N):
        # The inner loop constructed using n variable iterates through temporal indexes of the signal, from 0 to N-1.
        for n in range(N):
            # Compute the sum using the formula given for DFT
            # Here, we multiply each sample value y[n] by e^(-2J*pi*k*n/N), where J represent the `imaginary` number,
            # k*n*2pi / N is the angle that rotates around the circumference circle freq bin number of times per N samples,
            # thus making sure that each freq gets its corresponding coefficient.
            c[k] += y[n] * np.exp(-2j * np.pi * k * n / N)

    # Returns the calculated discrete fourier transform coefficients.
    return c


def idft(c):
    N = len(c)
    y = np.zeros(
        N, dtype=complex
    )  # an empty numpy array with zeros of the same length as input array 'c'

    for n in range(N):  # Outer Loop iterating values from 0 to length of c - 1
        for k in range(N):  # Inner loop iterating values from 0 to length of c - 1

            # Calculating value using cosine and sine functions
            y[n] += c[k] * np.exp(2j * np.pi * k * n / N)

        y[n] /= N  # divide each element by N to get final result

    return y.real  # returning the real part of y


# Fast Fourier Transform
def fft(y):
    # Initially, the length of the signal is calculated and stored in N variable
    N = len(y)
    # If the length of the vector is 1, return y
    if N == 2:
        c = np.zeros(2, dtype=np.complex)
        c[0] = y[0] + y[1]
        c[1] = y[0] - y[1] 
        return c
    # Even and odd arrays 'b' and 'c' of 0's with dtype complex is created with length (N//2).
    b = np.zeros(N // 2, dtype=complex)
    c = np.zeros(N // 2, dtype=complex)

    # The for loop is used to compute the Fourier transform
    for i in range(N // 2):
        twiddle_factor = np.exp(-2j * np.pi * i / N)
        b[i] = y[i] + y[i + N // 2]
        c[i] = (y[i] - y[i + N // 2]) * twiddle_factor

    # Recursively call the FFT on each of the parts
    y_b = fft(b)
    y_c = fft(c)

    # Combining results
    y = np.zeros(N, dtype=complex)
    for i in range(N//2):
        y[2 * i] = y_b[i]
        y[2 * i + 1] = y_c[i]

    # Returns the calculated fast fourier transform coefficients.
    return y

def ifft(y):
    # Initially, the length of the signal is calculated and stored in N variable
    N = len(y)
    # If the length of the vector is 1, return y
    if N == 1:
        return y

    # Even and odd arrays 'b' and 'c' of 'y' vector
    b = y[::2]
    c = y[1::2]

    # Recursively call the FFT on each of the parts
    y_b = ifft(b)
    y_c = ifft(c)

    # Combining results
    y = np.zeros(N, dtype=complex)

    for i in range(N // 2):
        twiddle_factor = np.exp(2j * np.pi * i / N)
        y[i] = y_b[i] + twiddle_factor * y_c[i]
        y[i + N // 2] = y_b[i] - twiddle_factor * y_c[i]

    # Returns the calculated fast fourier transform coefficients.
    return y


def showPlots():
    # Creating graphic of default signal
    x = np.linspace(0, 2 * np.pi, 16)
    y = np.sin(3 * x) + np.cos(x)

    fig, axs = plt.subplots(2, 4, figsize=(10, 8))
    plt.gcf().canvas.manager.set_window_title("Lab 1 (powered by raik199x + LeGushka)")

    # Plot original signal
    for i in range(2):
        axs[i, 0].plot(x, y)
        axs[i, 0].set_title("Original Signal y=sin(3x)+cos(x)")
        axs[i, 0].set_xlabel("Time")
        axs[i, 0].set_ylabel("Amplitude")

    # Compute and plot FFT
    c = fft(y)
    freq = np.linspace(0, 1 / (x[1] - x[0]), num=len(c))
    axs[1, 1].stem(freq, np.abs(c))
    axs[1, 1].set_title("Amplitude Spectrum (FFT)")
    axs[1, 1].set_xlabel("Frequency")
    axs[1, 1].set_ylabel("Magnitude")

    axs[1, 3].stem(freq, np.angle(c))
    axs[1, 3].set_title("Phase Spectrum")
    axs[1, 3].set_xlabel("Frequency")
    axs[1, 3].set_ylabel("Radians")

    y_reconstructed = ifft(c)
    axs[1, 2].plot(x, y_reconstructed)
    axs[1, 2].set_title("Signal -> FFT -> IFFT")
    axs[1, 2].set_xlabel("Time")
    axs[1, 2].set_ylabel("Amplitude")

    # Compute and plot DFT
    c = dft(y)
    freq = np.linspace(-1 / (2 * (x[1] - x[0])), 1 / (2 * (x[1] - x[0])), num=len(c))
    axs[0, 1].stem(freq, np.abs(c))
    axs[0, 1].set_title("Amplitude Spectrum (DFT)")
    axs[0, 1].set_xlabel("Frequency")
    axs[0, 1].set_ylabel("Magnitude")

    axs[0, 3].stem(freq, np.angle(c))
    axs[0, 3].set_title("Phase Spectrum")
    axs[0, 3].set_xlabel("Frequency")
    axs[0, 3].set_ylabel("Radians")

    y_reconstructed = idft(c)
    axs[0, 2].plot(x, y_reconstructed.real)
    axs[0, 2].set_title("Signal -> DFT -> IDFT")
    axs[0, 2].set_xlabel("Time")
    axs[0, 2].set_ylabel("Amplitude")

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    showPlots()
