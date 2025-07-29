import tensorflow as tf
from tensorflow.keras import layers, models
import numpy as np

def load_data():
    (x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()
    # Normalisieren auf [0,1]
    x_train = x_train.astype('float32') / 255
    x_test = x_test.astype('float32') / 255
    # Dimension erweitern für CNN (Batch, Höhe, Breite, Kanäle)
    x_train = np.expand_dims(x_train, -1)
    x_test = np.expand_dims(x_test, -1)
    return (x_train, y_train), (x_test, y_test)

def build_model():
    model = models.Sequential([
        layers.Conv2D(32, (3,3), activation='relu', input_shape=(28,28,1)),
        layers.MaxPooling2D((2,2)),
        layers.Conv2D(64, (3,3), activation='relu'),
        layers.MaxPooling2D((2,2)),
        layers.Flatten(),
        layers.Dense(64, activation='relu'),
        layers.Dense(10, activation='softmax')
    ])
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    return model

if __name__ == "__main__":
    (x_train, y_train), (x_test, y_test) = load_data()
    model = build_model()
    model.summary()

    model.fit(x_train, y_train, epochs=5, batch_size=64, validation_split=0.1)

    test_loss, test_acc = model.evaluate(x_test, y_test)
    print(f"Test accuracy: {test_acc:.4f}")
