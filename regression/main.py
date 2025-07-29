import numpy as np

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1 - x)

class NeuralNetwork:
    def __init__(self, input_size, hidden_size, output_size, lr=0.5):
        self.lr = lr
        self.weights_input_hidden = np.random.uniform(-1, 1, (input_size, hidden_size))
        self.weights_hidden_output = np.random.uniform(-1, 1, (hidden_size, output_size))
        self.bias_hidden = np.zeros((1, hidden_size))
        self.bias_output = np.zeros((1, output_size))

    def feedforward(self, X):
        self.hidden_input = np.dot(X, self.weights_input_hidden) + self.bias_hidden
        self.hidden_output = sigmoid(self.hidden_input)
        self.final_input = np.dot(self.hidden_output, self.weights_hidden_output) + self.bias_output
        self.final_output = self.final_input  # Für Regression: linear output
        return self.final_output

    def backpropagate(self, X, y, output):
        output_error = y - output
        output_delta = output_error  # linear output, derivative = 1

        hidden_error = output_delta.dot(self.weights_hidden_output.T)
        hidden_delta = hidden_error * sigmoid_derivative(self.hidden_output)

        self.weights_hidden_output += self.hidden_output.T.dot(output_delta) * self.lr
        self.bias_output += np.sum(output_delta, axis=0, keepdims=True) * self.lr
        self.weights_input_hidden += X.T.dot(hidden_delta) * self.lr
        self.bias_hidden += np.sum(hidden_delta, axis=0, keepdims=True) * self.lr

        return np.mean(output_error**2)

    def train(self, X, y, epochs=1000):
        for epoch in range(epochs):
            output = self.feedforward(X)
            loss = self.backpropagate(X, y, output)
            if epoch % 100 == 0:
                print(f"Epoch {epoch}, MSE: {loss}")

if __name__ == "__main__":
    # Beispiel: Funktion sin(x) approximieren
    X = np.linspace(0, 2 * np.pi, 100).reshape(-1, 1)
    y = np.sin(X)

    nn = NeuralNetwork(input_size=1, hidden_size=10, output_size=1, lr=0.01)
    nn.train(X, y, epochs=1000)

    import matplotlib.pyplot as plt
    pred = nn.feedforward(X)
    plt.plot(X, y, label="True sin(x)")
    plt.plot(X, pred, label="NN Prediction")
    plt.legend()
    plt.show()
