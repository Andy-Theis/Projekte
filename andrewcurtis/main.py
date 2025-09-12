import random

# Startpräsentation: trivial, aber kompliziert geschrieben
# Beispiel: <a,b | aba^-1b^-1, a>
relations = ["aba^-1b^-1", "a"]

# Zielpräsentation (Standardform für 2 Generatoren)
goal = ["a", "b"]

def simplify_word(word):
    """Kürzt a*a^-1 usw."""
    changed = True
    while changed:
        changed = False
        for pattern in ["aa^-1", "a^-1a", "bb^-1", "b^-1b"]:
            if pattern in word:
                word = word.replace(pattern, "")
                changed = True
    return word or "1"

def apply_transformation(rels):
    rels = rels[:]  # Kopie
    move = random.choice(["invert_gen", "multiply_rels", "replace_gen"])
    
    if move == "invert_gen":
        g = random.choice(["a", "b"])
        rels = [r.replace(g, g + "^-1") if g not in r else r for r in rels]
        
    elif move == "multiply_rels":
        i, j = random.sample(range(len(rels)), 2)
        rels[i] = simplify_word(rels[i] + rels[j])
        
    elif move == "replace_gen":
        g1, g2 = random.sample(["a", "b"], 2)
        rels = [r.replace(g1, g1 + g2) for r in rels]
        
    # Kürzen der Wörter
    rels = [simplify_word(r) for r in rels]
    return rels

def score(rels):
    """Bewertung: je näher an ['a', 'b'], desto besser."""
    target_set = set(goal)
    return sum(1 for r in rels if r in target_set)

# Suchschleife
best = relations
best_score = score(best)

for step in range(1000):
    new_rels = apply_transformation(best)
    sc = score(new_rels)
    if sc > best_score:
        best, best_score = new_rels, sc
        print(f"Step {step}: {best} (score {sc})")
    if best_score == len(goal):
        break

print("Gefundene Präsentation:", best)
