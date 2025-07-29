import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs

def generate_data():
    # Generiert synthetische Daten mit 3 Clustern
    X, y_true = make_blobs(n_samples=300, centers=3, cluster_std=0.60, random_state=0)
    return X, y_true

def perform_clustering(X, n_clusters=3):
    kmeans = KMeans(n_clusters=n_clusters)
    kmeans.fit(X)
    return kmeans.labels_, kmeans.cluster_centers_

def plot_clusters(X, labels, centers):
    plt.scatter(X[:, 0], X[:, 1], c=labels, s=50, cmap='viridis')
    plt.scatter(centers[:, 0], centers[:, 1], c='red', s=200, alpha=0.75, marker='X')
    plt.title("K-Means Clusteranalyse")
    plt.xlabel("Feature 1")
    plt.ylabel("Feature 2")
    plt.show()

if __name__ == "__main__":
    X, y_true = generate_data()
    labels, centers = perform_clustering(X)
    plot_clusters(X, labels, centers)
    print("Clusteranalyse abgeschlossen.")
    