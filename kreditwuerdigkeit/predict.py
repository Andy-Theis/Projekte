import joblib

def vorhersage(alter, einkommen, laufzeit, schuldenquote):
    model = joblib.load("kreditmodell.pkl")
    daten = [[alter, einkommen, laufzeit, schuldenquote]]
    vorhersage = model.predict(daten)
    if vorhersage[0] == 1:
        print("✅ Kredit kann gewährt werden.")
    else:
        print("❌ Kredit sollte abgelehnt werden.")

# Beispielaufruf:
if __name__ == "__main__":
    vorhersage(30, 3200, 36, 0.3)
