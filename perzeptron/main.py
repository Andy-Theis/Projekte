import numpy as np

class Perceptron:
    def __init__(self, input_dim, lr=0.1, epochs=1000):
        self.weights = np.zeros(input_dim + 1)  # +1 für Bias
        self.lr = lr
        self.epochs = epochs

    def predict(self, x):
        x_with_bias = np.insert(x, 0, 1)  # Bias-Term hinzufügen
        z = np.dot(self.weights, x_with_bias)
        return 1 if z >= 0 else 0

    def train(self, X, y):
        for _ in range(self.epochs):
            for xi, target in zip(X, y):
                prediction = self.predict(xi)
                error = target - prediction
                if error != 0:
                    x_with_bias = np.insert(xi, 0, 1)
                    self.weights += self.lr * error * x_with_bias

if __name__ == "__main__":
    # Beispiel-Daten: AND-Logik
    X = np.array([[0,0], [0,1], [1,0], [1,1]])
    y = np.array([0, 0, 0, 1])

    p = Perceptron(input_dim=2)
    p.train(X, y)

    print("Gewichte:", p.weights)
    for x in X:
        print(f"Input: {x}, Vorhersage: {p.predict(x)}")
    # Testen des Perzeptrons
    test_data = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
    for x in test_data:
        print(f"Input: {x}, Vorhersage: {p.predict(x)}")
    # Ausgabe der Vorhersagen

    print("Vorhersagen:")
    for x in test_data:
        print(f"Input: {x}, Vorhersage: {p.predict(x)}")

    # Ausgabe der Gewichte
    print("Endgültige Gewichte:", p.weights)
    # Ausgabe der Gewichte
    print("Training abgeschlossen.")