import cv2
import numpy as np
import random
import math


class NeuralNetwork:
	def __init__(self):
		# Creating Layers and neurons
		self.InputLayerSize = 36  # 6 x 6 pixels
		self.InputLayer = list()

		self.OutputLayerSize = 5
		self.OutputLayer = list()  # 5

		self.HiddenLayer1Size = 10  # adjust size of hidden layer
		self.HiddenLayer1 = list()

		self.Q = list()
		for _ in range(self.HiddenLayer1Size):
			self.Q.append(random.uniform(-1, 1))
		self.T = list()
		for _ in range(self.OutputLayerSize):
			self.T.append(random.uniform(-1, 1))

		# preparing input -> hidden weights
		self.WeightsInputHidden1 = list()
		for i in range(self.InputLayerSize):
			weights = list()
			for j in range(self.HiddenLayer1Size):
				weights.append(random.uniform(-1, 1))
			self.WeightsInputHidden1.append(weights)

		# preparing hidden1 -> output weights
		self.WeightsHidden1Output = list()
		for j in range(self.HiddenLayer1Size):
			weights = list()
			for k in range(self.OutputLayerSize):
				weights.append(random.uniform(-1, 1))
			self.WeightsHidden1Output.append(weights)

		self.LearningRate = 0.3  # [0:1], but need to find the perfect one

		self.HiddenLayer1Results = list()  # buffer

	def sigmoid(self, value) -> float:
		return 1 / (1 + math.exp(-value))

	def GetInputNeurons(self, pathToImage):
		image = cv2.imread(pathToImage)
		if image is None:
			print("CRINGE")
		Result = list()
		height, width = image.shape[:2]
		# Iterating through image pixels
		for y in range(height):
			for x in range(width):
				if np.array_equal(image[y, x], [255, 255, 255]):
					Result.append(1)
				else:
					Result.append(0)
		return Result

	def GetNeuralResult(self, InputNeurons: list) -> list:
		self.HiddenLayer1Results.clear()

		hiddenNeurons = list()
		for j in range(self.HiddenLayer1Size):
			hiddenNeuronResult = 0
			for i in range(self.InputLayerSize):
				hiddenNeuronResult += InputNeurons[i] * self.WeightsInputHidden1[i][j]
			hiddenNeurons.append(self.sigmoid(hiddenNeuronResult + self.Q[j]))
		self.HiddenLayer1Results = hiddenNeurons.copy()

		outputNeurons = list()
		for k in range(self.OutputLayerSize):
			outputNeuronResult = 0
			for j in range(self.HiddenLayer1Size):
				outputNeuronResult += self.WeightsHidden1Output[j][k] * hiddenNeurons[j]
			outputNeurons.append(self.sigmoid(outputNeuronResult + self.T[k]))

		return outputNeurons

	def LearnNeuralNetwork(self, ImagesPaths):
		"""
		Training neural network

		send classes in right order to set weights appropriately
		Class1, Class2, ..., Class<AmountOfClasses>
		"""
		# Backpropagation algorithm of studying

		InputNeuronsLists = list()
		for path in ImagesPaths:
			InputNeuronsLists.append(self.GetInputNeurons(path))

		counter = 0
		while True:
			for num, path in enumerate(ImagesPaths):
				# print(InputNeurons)
				OutputNeurons = self.GetNeuralResult(InputNeuronsLists[num])
				mistakes = list()
				for expectPos in range(len(OutputNeurons)):
					if num == expectPos:
						mistakes.append(1 - OutputNeurons[expectPos])
					else:
						mistakes.append(0 - OutputNeurons[expectPos])

				if np.max(np.abs(mistakes)) < 0.005:
					print(counter)
					return
				counter += 1
				# print(mistakes)
				# Correcting Hidden -> output results (w[j][k])
				for j in range(self.HiddenLayer1Size):
					for k in range(self.OutputLayerSize):
						self.WeightsHidden1Output[j][k] = (
							self.WeightsHidden1Output[j][k]
							+ self.LearningRate
							* OutputNeurons[k]
							* (1 - OutputNeurons[k])
							* mistakes[k]
							* self.HiddenLayer1Results[j]
						)
				HiddenLayerMistake = list()
				for j in range(self.HiddenLayer1Size):
					SMistake = 0
					for k in range(self.OutputLayerSize):
						SMistake += mistakes[k] * OutputNeurons[k] * (1 - OutputNeurons[k]) * self.WeightsHidden1Output[j][k]
					HiddenLayerMistake.append(SMistake)

				# Correcting Input -> Hidden (v[i][j])
				for i in range(self.InputLayerSize):
					for j in range(self.HiddenLayer1Size):
						self.WeightsInputHidden1[i][j] = (
							self.WeightsInputHidden1[i][j]
							+ self.LearningRate
							* self.HiddenLayer1Results[j]
							* (1 - self.HiddenLayer1Results[j])
							* HiddenLayerMistake[j]
							* InputNeuronsLists[num][i]
						)

				# Correcting Q
				for j in range(self.HiddenLayer1Size):
					self.Q[j] = (
						self.Q[j]
						+ self.LearningRate
						* self.HiddenLayer1Results[j]
						* (1 - self.HiddenLayer1Results[j])
						* HiddenLayerMistake[j]
					)

				# Correcting T
				for k in range(self.OutputLayerSize):
					self.T[k] = self.T[k] + self.LearningRate * OutputNeurons[k] * (1 - OutputNeurons[k]) * mistakes[k]
