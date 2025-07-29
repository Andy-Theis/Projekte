import tensorflow as tf
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras import layers, models
import numpy as np

# Beispiel-Daten: einfache Sätze mit zwei Klassen (0 oder 1)
texts = [
    "I love machine learning",
    "Deep learning is awesome",
    "I hate spam emails",
    "Spam emails are annoying",
    "I enjoy coding in Python",
    "I dislike bugs in code"
]
labels = [1, 1, 0, 0, 1, 0]

# Tokenizer vorbereiten
tokenizer = Tokenizer(num_words=1000, oov_token="<OOV>")
tokenizer.fit_on_texts(texts)
sequences = tokenizer.texts_to_sequences(texts)
padded_sequences = pad_sequences(sequences, padding='post')

# Modell bauen
model = models.Sequential([
    layers.Embedding(input_dim=1000, output_dim=16, input_length=padded_sequences.shape[1]),
    layers.SimpleRNN(32),
    layers.Dense(1, activation='sigmoid')
])

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

if __name__ == "__main__":
    # Training
    model.fit(padded_sequences, np.array(labels), epochs=20, verbose=2)

    # Beispielvorhersage
    test_texts = ["I love spam", "coding is fun"]
    test_seq = tokenizer.texts_to_sequences(test_texts)
    test_pad = pad_sequences(test_seq, maxlen=padded_sequences.shape[1], padding='post')
    predictions = model.predict(test_pad)

    for text, pred in zip(test_texts, predictions):
        print(f"Text: {text} -> Klassifikation: {'Positiv' if pred > 0.5 else 'Negativ'} ({pred[0]:.3f})")
# Ausgabe der Modellzusammenfassung

    model.summary()
# Ausgabe der Modellzusammenfassung
    print("Training abgeschlossen.")
            