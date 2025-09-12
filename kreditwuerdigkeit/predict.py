import pandas as pd
import joblib

def vorhersage(alter, einkommen, kreditlaufzeit, schuldenquote):
    model = joblib.load("kreditmodell.pkl")

    # DataFrame mit den richtigen Spaltennamen
    daten = pd.DataFrame(
        [[alter, einkommen, kreditlaufzeit, schuldenquote]],
        columns=["alter", "einkommen", "kreditlaufzeit", "schuldenquote"]
    )
    # Vorhersage treffen
    vorhersage = model.predict(daten)

    #daten = [[alter, einkommen, kreditlaufzeit, schuldenquote]]
    #vorhersage = model.predict(daten)
    if vorhersage[0] == 1:
        print("✅ Kredit kann gewährt werden.")
    else:
        print("❌ Kredit sollte abgelehnt werden.")

# Beispielaufruf:
if __name__ == "__main__":
    vorhersage(30, 3200, 36, 0.3)
