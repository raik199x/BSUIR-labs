import cv2
import numpy as np
import matplotlib.pyplot as plt


def mexican_hat_wavelet(x, sigma):
    return (
        (2 / (np.sqrt(3 * sigma) * np.pi**0.25))
        * (1 - (x**2) / sigma**2)
        * np.exp(-(x**2) / (2 * sigma**2))
    )


def WaveletDecomposition(image, sigma):
    rows, cols = image.shape
    transformed_image = np.zeros((rows, cols, 4))

    for i in range(rows):
        for j in range(cols):
            x = np.arange(-5, 6)
            y = image[
                max(0, i - 5) : min(rows, i + 6), max(0, j - 5) : min(cols, j + 6)
            ].flatten()

            transformed_image[i, j, 0] = np.sum(
                y.reshape(-1, 1) * mexican_hat_wavelet(x, sigma)
            )  # ВВ (аппроксимация)
            transformed_image[i, j, 1] = np.sum(
                y.reshape(-1, 1) * x * mexican_hat_wavelet(x, sigma)
            )  # ВН (детали по горизонтали)
            transformed_image[i, j, 2] = np.sum(
                y.reshape(-1, 1) * np.ones_like(x) * mexican_hat_wavelet(x, sigma)
            )  # НВ (детали по вертикали)
            transformed_image[i, j, 3] = np.sum(
                y.reshape(-1, 1) * x * np.ones_like(x) * mexican_hat_wavelet(x, sigma)
            )  # НН (детали по обоим направлениям)

    return transformed_image


def WaveQuantizeWithEncode(transformed_image, num_bits=8):
    max_value = np.max(transformed_image)
    min_value = np.min(transformed_image)
    step_size = (max_value - min_value) / (2**num_bits)

    quantized_image = np.round((transformed_image - min_value) / step_size)
    quantized_image = np.clip(quantized_image, 0, 2**num_bits - 1)

    encoded_image = quantized_image.astype(np.uint8)

    return encoded_image, step_size, min_value


def decode_and_inverse_quantize(encoded_image, step_size, min_value):
    decoded_image = encoded_image * step_size + min_value

    return decoded_image


def inverse_wavelet_transform(decoded_image):
    rows, cols, _ = decoded_image.shape
    reconstructed_image = np.zeros((rows, cols))

    for i in range(rows):
        for j in range(cols):
            reconstructed_image[i, j] = (
                decoded_image[i, j, 0]
                + decoded_image[i, j, 1]
                + decoded_image[i, j, 2]
                + decoded_image[i, j, 3]
            )

    return reconstructed_image


def compute_psnr(original, reconstructed):
    mse = np.mean((original - reconstructed) ** 2)
    max_pixel = np.max(original)
    psnr = 20 * np.log10(max_pixel / np.sqrt(mse))
    return psnr


def full_reconstruction(encoded_image, step_size, min_value):
    decoded_image = decode_and_inverse_quantize(encoded_image, step_size, min_value)
    reconstructed_image = inverse_wavelet_transform(decoded_image)

    return reconstructed_image


if __name__ == "__main__":
    # Загрузка изображения
    image = cv2.imread("data/sample.png", cv2.IMREAD_GRAYSCALE)

    # Применение вейвлет-преобразования
    sigma = 1.0
    transformed_image = WaveletDecomposition(image, sigma)

    # Визуализация результата
    plt.figure(figsize=(12, 6))
    plt.subplot(2, 3, 1)
    plt.imshow(image, cmap="gray")
    plt.title("Original Image")
    plt.axis("off")

    for i in range(4):
        plt.subplot(2, 3, i + 2)
        plt.imshow(transformed_image[:, :, i], cmap="gray")
        plt.title(f"Level {i+1}")
        plt.axis("off")

    plt.tight_layout()
    plt.show()

    # Применение функции квантования и кодирования
    num_bits = 8
    encoded_image, step_size, min_value = WaveQuantizeWithEncode(
        transformed_image, num_bits
    )

    # Визуализация оригинального изображения и закодированного изображения
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    plt.imshow(transformed_image[:, :, 0], cmap="gray")
    plt.title("Original Transformed Image (Approximation)")
    plt.axis("off")

    plt.subplot(1, 2, 2)
    plt.imshow(encoded_image[:, :, 0], cmap="gray")
    plt.title("Quantized and Encoded Image (Approximation)")
    plt.axis("off")

    plt.tight_layout()
    plt.show()

    # Применение обратного квантования и обратного вейвлет-преобразования
    decoded_image = decode_and_inverse_quantize(encoded_image, step_size, min_value)
    reconstructed_image = inverse_wavelet_transform(decoded_image)

    # Визуализация оригинального изображения и восстановленного изображения
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 3, 1)
    plt.imshow(transformed_image[:, :, 0], cmap="gray")
    plt.title("Original Transformed Image (Approximation)")
    plt.axis("off")

    plt.subplot(1, 3, 2)
    plt.imshow(encoded_image[:, :, 0], cmap="gray")
    plt.title("Quantized and Encoded Image (Approximation)")
    plt.axis("off")

    plt.subplot(1, 3, 3)
    plt.imshow(reconstructed_image, cmap="gray")
    plt.title("Reconstructed Image (Approximation)")
    plt.axis("off")

    plt.tight_layout()
    plt.show()

    # Полный цикл обработки: кодирование, передача данных, восстановление
    num_bits = 8
    encoded_image, step_size, min_value = WaveQuantizeWithEncode(
        transformed_image, num_bits
    )
    reconstructed_image = full_reconstruction(encoded_image, step_size, min_value)

    # Визуализация оригинального изображения, закодированного и восстановленного изображения
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 3, 1)
    plt.imshow(transformed_image[:, :, 0], cmap="gray")
    plt.title("Original Transformed Image (Approximation)")
    plt.axis("off")

    plt.subplot(1, 3, 2)
    plt.imshow(encoded_image[:, :, 0], cmap="gray")
    plt.title("Quantized and Encoded Image (Approximation)")
    plt.axis("off")

    plt.subplot(1, 3, 3)
    plt.imshow(reconstructed_image, cmap="gray")
    plt.title("Fully Reconstructed Image (Approximation)")
    plt.axis("off")

    plt.tight_layout()
    plt.show()

    # Измерение ошибки восстановления
    psnr = compute_psnr(transformed_image[:, :, 0], reconstructed_image)
    print(f"PSNR: {psnr} dB")
