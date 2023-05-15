"""	
    2.1 На вход фильтра, применяемых для частотной селекции, входной сигнал необходима представить в частотной области.
    ---
	№	Signal					Algorithm				 	N
 	2	y=sin(3x)+cos(x)	БПФ с прореживанием по частоте	16
    ---
    Condition №3
    Filter №1: ВЧ оконный фильтр. Окно Хэмминга (Конечная Импульсная Характеристика)(finite impulse response filter)
    Filter №2: Режекторный узкополосный фильтр  (finite impulse response fБесконечная Импульсная Характеристика)(infinite impulse response filter) 
"""

import numpy as np  #! Do not use built in FFT, IFFT, convolution
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt, iirnotch
import fft
import random


def FIRF(base_y):
    # filter parameters
    nyquist_freq = 200
    cutoff_freq = 0.4
    order = 3

    # filter coefficient
    coeff1, coeff2 = butter(order, cutoff_freq / nyquist_freq, btype="highpass")
    y_filtered = filtfilt(coeff1, coeff2, base_y)

    return y_filtered


def IIRRF(y):
    # Определяем параметры фильтра
    fs = 100.0  # частота дискретизации
    f0 = 50.0  # центральная частота
    Q = 40.0  # добротность
    bw = f0 / Q  # полоса пропускания

    # Создаем фильтр
    b, a = iirnotch(f0, bw, fs)
    # Применяем фильтр к сигналу
    return filtfilt(b, a, y)


def showPlots(baseX, baseY, yMessed):
    fig, axs = plt.subplots(2, 4, figsize=(10, 8))
    plt.gcf().canvas.manager.set_window_title("Lab 4 (powered by raik199x + LeGushka)")

    # Base signal
    axs[0, 0].plot(baseX, baseY)
    axs[0, 0].set_title("Base Signal + Hamming Window")
    axs[0, 0].set_xlabel("Time")
    axs[0, 0].set_ylabel("Amplitude")

    # Frequency domain of BS
    frequencyDomainBS = fft.fft(baseY)
    freqBS = np.linspace(0, 1 / (x[1] - x[0]), num=len(frequencyDomainBS))

    axs[0, 1].stem(freqBS, frequencyDomainBS)
    axs[0, 1].set_title("Frequency Domain BS")
    axs[0, 1].set_xlabel("Frequency")
    axs[0, 1].set_ylabel("Magnitude")

    # Messed signal
    axs[0, 2].plot(baseX, yMessed)
    axs[0, 2].set_title("Messed Signal")
    axs[0, 2].set_xlabel("Time")
    axs[0, 2].set_ylabel("Amplitude")

    # Frequency domain of MS
    frequencyDomainMS = fft.fft(yMessed)
    freqMS = np.linspace(0, 1 / (x[1] - x[0]), num=len(frequencyDomainMS))

    axs[0, 3].stem(freqMS, frequencyDomainMS)
    axs[0, 3].set_title("Frequency Domain MS")
    axs[0, 3].set_xlabel("Frequency")
    axs[0, 3].set_ylabel("Magnitude")

    # FIRF filter
    firf = FIRF(yMessed)
    axs[1, 0].plot(baseX, firf)
    axs[1, 0].set_title("FIRF Filter Signal")
    axs[1, 0].set_xlabel("Time")
    axs[1, 0].set_ylabel("Amplitude")

    # FIRF frequency domain
    frequencyDomainFIRF = fft.fft(firf)
    freqFIRF = np.linspace(0, 1 / (x[1] - x[0]), num=len(frequencyDomainFIRF))

    axs[1, 1].stem(freqFIRF, frequencyDomainFIRF)
    axs[1, 1].set_title("Frequency domain FIRF")
    axs[1, 1].set_xlabel("Frequency")
    axs[1, 1].set_ylabel("Magnitude")

    # IIRF filter
    iirf = IIRRF(messedSignal)
    axs[1, 2].plot(x, iirf)
    axs[1, 2].set_title("IIRF Filter Signal")
    axs[1, 2].set_xlabel("Time")
    axs[1, 2].set_ylabel("Amplitude")

    # IIRF frequency domain
    frequencyDomainIIRF = fft.fft(iirf)
    freqIIRF = np.linspace(0, 1 / (x[1] - x[0]), num=len(frequencyDomainFIRF))

    axs[1, 3].stem(freqIIRF, frequencyDomainIIRF)
    axs[1, 3].set_title("Frequency Domain IIRF")
    axs[1, 3].set_xlabel("Frequency")
    axs[1, 3].set_ylabel("Magnitude")

    plt.tight_layout()
    plt.show()


def HammingWindow(y):
    # w(n) = 0.54 - 0.46 * cos(2 * pi * n / (N - 1)) -- Hamming
    window = np.zeros(len(y))
    for i in range(len(y)):
        window[i] = 0.54 - 0.46 * np.cos(2 * np.pi * i / (len(y) - 1))

    resultSignal = np.zeros(len(y))
    for i in range(len(y)):
        resultSignal[i] = y[i] * window[i]

    return resultSignal


def MessingUpSignal(y):
    resultSignal = np.zeros(len(y))
    for i in range(len(y)):
        resultSignal[i] = y[i] + random.random()
    return resultSignal


if __name__ == "__main__":
    N = 32  # N was change to higher value so we can see filtration
    x = np.linspace(0, 2 * np.pi, N)
    y = np.sin(3 * x) + np.cos(x)

    hammingSignal = HammingWindow(y)
    messedSignal = MessingUpSignal(hammingSignal)
    showPlots(x, hammingSignal, messedSignal)
