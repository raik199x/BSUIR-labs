import numpy as np
import fft

# x, y are base signals, N - length
def correlation(x, y, N):
    # Z(m) = 1/nΣX(h)Y(m+h) -- Base formula from task
    # Where m,h are indexes of arrays
    # n is length of array (given in task)
    # Z result signal
    # x, y Base signals

    z = np.zeros(N)
    for m in range(N):
        for h in range(N):
            if h + m > N - 1:
                break
            z[m] += x[h] * y[h + m]
        z[m] = z[m] / N
    return z


def correlationFFT(x, y):
    """Correlate two signals using the FFT"""
    # Reverse the second signal
    y_rev = np.flip(y)
    
    # Calculate the length of the output signal
    len_x = len(x)
    len_y = len(y)
    len_out = len_x + len_y - 1
    
    # Pad the input signals to the nearest power of 2 for FFT optimization
    n = 2 ** int(np.ceil(np.log2(len_out)))
    x_pad = np.pad(x, (0, n - len_x), 'constant')
    y_pad = np.pad(y_rev, (0, n - len_y), 'constant')
    
    # Compute the FFT of the padded input signals
    X = fft.fft(x_pad)
    Y = fft.fft(y_pad)
    
    # Multiply the FFTs to get the correlation
    Z = X * Y
    
    # Compute the inverse FFT of the correlation
    z = fft.ifft(Z)
    
    # Return the real part of the output signal
    return np.real(z[:len_out//2])


# Function for testing correlation, not to real use
def NumPyCorrelation(z, y):
    return np.correlate(z, y, mode="full")
