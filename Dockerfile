# Image de base légère avec Python (requis pour PIO)
FROM python:3.11-slim

# Installation des dépendances système pour ESP-IDF et Git
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    libpython3-dev \
    && rm -rf /var/lib/apt/lists/*

# Installation de PlatformIO
RUN pip install --no-cache-dir -U platformio

# Définition du répertoire de travail
WORKDIR /app

# On copie le projet dans l'image (optionnel pour la CI, mais utile pour le dev local)
COPY . .

# Par défaut, on lance la compilation
CMD ["pio", "run"]