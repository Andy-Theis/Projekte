import numpy as np
import yfinance as yf
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import load_model # pyright: ignore[reportMissingImports]

ticker = 'AAPL'
model = load_model('lstm_stock_model.h5')

# Neueste Daten abrufen
df = yf.download(ticker, period='90d')
close_data = df['Close'].values.reshape(-1, 1)

# Skalieren
scaler = MinMaxScaler()
scaled_data = scaler.fit_transform(close_data)

# Letzte 60 Tage für Vorhersage vorbereiten
X_input = scaled_data[-60:].reshape(1, 60, 1)

# Vorhersage
predicted_scaled = model.predict(X_input)
predicted_price = scaler.inverse_transform(predicted_scaled)

print(f"📈 Prognostizierter nächster Schlusskurs ({ticker}): ${predicted_price[0][0]:.2f}")
