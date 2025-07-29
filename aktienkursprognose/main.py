import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense
import yfinance as yf

# Daten abrufen (z. B. Apple-Aktie)
ticker = 'AAPL'
df = yf.download(ticker, start='2015-01-01', end='2023-12-31')
df = df[['Close']]

# Daten normalisieren
scaler = MinMaxScaler()
scaled_data = scaler.fit_transform(df)

# Sequenzen erstellen
sequence_length = 60
X, y = [], []

for i in range(sequence_length, len(scaled_data)):
    X.append(scaled_data[i-sequence_length:i])
    y.append(scaled_data[i])

X, y = np.array(X), np.array(y)

# Trainings- / Testdaten splitten
split = int(len(X) * 0.8)
X_train, X_test = X[:split], X[split:]
y_train, y_test = y[:split], y[split:]

# LSTM-Modell erstellen
model = Sequential()
model.add(LSTM(units=50, return_sequences=True, input_shape=(X_train.shape[1], 1)))
model.add(LSTM(units=50))
model.add(Dense(units=1))
model.compile(optimizer='adam', loss='mean_squared_error')

# Modell trainieren
model.fit(X_train, y_train, epochs=10, batch_size=32)

# Vorhersage
predicted = model.predict(X_test)
predicted = scaler.inverse_transform(predicted)
real = scaler.inverse_transform(y_test)

# Plotten
plt.plot(real, label='Echte Werte')
plt.plot(predicted, label='Vorhergesagt')
plt.title('Aktienkursvorhersage (Testdaten)')
plt.xlabel('Tage')
plt.ylabel('Kurs ($)')
plt.legend()
plt.show()
# Modell speichern
model.save('aktienkursprognose_model.h5')
# Modell laden (optional)
# from tensorflow.keras.models import load_model
# model = load_model('aktienkursprognose_model.h5')
# Vorhersage für die nächsten 30 Tage
future_days = 30
future_sequence = scaled_data[-sequence_length:]
future_predictions = []
for _ in range(future_days):
    future_input = np.array(future_sequence).reshape((1, sequence_length, 1))
    future_pred = model.predict(future_input)
    future_predictions.append(future_pred[0, 0])
    future_sequence = np.append(future_sequence[1:], future_pred)
    future_sequence = future_sequence.reshape((sequence_length, 1))
# Zukünftige Vorhersagen zurückskalieren
future_predictions = scaler.inverse_transform(np.array(future_predictions).reshape(-1, 1))
# Plotten der zukünftigen Vorhersagen
plt.figure(figsize=(14, 7))
plt.plot(np.arange(len(real), len(real) + future_days), future_predictions, label='Zukünftige Vorhersagen', color='orange')
plt.title('Zukünftige Aktienkursvorhersage')
plt.xlabel('Tage')
plt.ylabel('Kurs ($)')
plt.legend()
plt.show()
# Zusätzliche Analyse: Fehlerberechnung
from sklearn.metrics import mean_squared_error, mean_absolute_error
mse = mean_squared_error(real, predicted)
mae = mean_absolute_error(real, predicted)
print(f'Mean Squared Error: {mse}')
print(f'Mean Absolute Error: {mae}')
# Zusätzliche Analyse: Korrelation zwischen Vorhersage und echten Werten
correlation = np.corrcoef(real.flatten(), predicted.flatten())[0, 1]
print(f'Korrelation zwischen Vorhersage und echten Werten: {correlation}')
# Zusätzliche Analyse: Visualisierung der Fehler
errors = real - predicted
plt.figure(figsize=(14, 7))
plt.plot(errors, label='Fehler (Echte Werte - Vorhergesagt)', color='red')
plt.title('Fehleranalyse der Aktienkursvorhersage')
plt.xlabel('Tage')
plt.ylabel('Fehler ($)')
plt.legend()
plt.show()
# Zusätzliche Analyse: Histogramm der Fehler
plt.figure(figsize=(14, 7))

plt.hist(errors, bins=50, color='blue', alpha=0.7)
plt.title('Histogramm der Fehler')
plt.xlabel('Fehler ($)')
plt.ylabel('Häufigkeit')
plt.axvline(0, color='red', linestyle='dashed', linewidth=1)
plt.show()
# Zusätzliche Analyse: Verteilung der Fehler
plt.figure(figsize=(14, 7))
plt.boxplot(errors, vert=False)
plt.title('Boxplot der Fehler')
plt.xlabel('Fehler ($)')
plt.grid()
plt.show()
