import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report
import joblib

# Dummy-Daten
data = pd.DataFrame({
    "alter": [25, 45, 35, 50, 23, 60, 31, 40, 22, 55],
    "einkommen": [2000, 4000, 3000, 4500, 1800, 6000, 3200, 4100, 1900, 5000],
    "kreditlaufzeit": [12, 60, 36, 48, 24, 72, 30, 60, 18, 60],
    "schuldenquote": [0.3, 0.4, 0.2, 0.6, 0.5, 0.2, 0.3, 0.4, 0.7, 0.2],
    "kreditwürdig": [1, 1, 1, 0, 0, 1, 1, 1, 0, 1]
})

# Daten aufteilen
X = data.drop("kreditwürdig", axis=1)
y = data["kreditwürdig"]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Modell trainieren
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Bewertung
predictions = model.predict(X_test)
print("Klassifikationsbericht:")
print(classification_report(y_test, predictions))

# Modell speichern
joblib.dump(model, "kreditmodell.pkl")
