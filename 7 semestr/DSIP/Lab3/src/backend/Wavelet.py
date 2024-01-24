import numpy as np


class MexicanWavelet:
    def __init__(self):
        pass

    # Mexican Hat wavelet
    def MHAT(self, x, sigma):
        return (
            (2 / (np.sqrt(3 * sigma) * np.pi**0.25))
            * (1 - (x**2) / sigma**2)
            * np.exp(-(x**2) / (2 * sigma**2))
        )

    # Вертикаль, Насыщение (Vertical, Saturation)
    def WaveletDecomposition(self, image, sigma):
        rows, cols = image.shape
        transformed_image = np.zeros((rows, cols, 4))

        for i in range(rows):
            for j in range(cols):
                x = np.arange(-5, 6)
                y = image[
                    max(0, i - 5) : min(rows, i + 6), max(0, j - 5) : min(cols, j + 6)
                ].flatten()

                transformed_image[i, j, 0] = np.sum(
                    y.reshape(-1, 1) * self.MHAT(x, sigma)
                )  # ВВ (approximation)
                transformed_image[i, j, 1] = np.sum(
                    y.reshape(-1, 1) * x * self.MHAT(x, sigma)
                )  # ВН (детали по горизонтали)
                transformed_image[i, j, 2] = np.sum(
                    y.reshape(-1, 1) * np.ones_like(x) * self.MHAT(x, sigma)
                )  # НВ (детали по вертикали)
                transformed_image[i, j, 3] = np.sum(
                    y.reshape(-1, 1) * x * np.ones_like(x) * self.MHAT(x, sigma)
                )  # НН (детали по обоим направлениям)

        return transformed_image
