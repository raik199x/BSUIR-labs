import numpy as np

# Fast Fourier Transform
def fft(y):
    # Initially, the length of the signal is calculated and stored in N variable
    N = len(y)
    # If the length of the vector is 1, return y
    if N == 2:
        c = np.zeros(2, dtype=np.complex_)
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
    for i in range(N // 2):
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
