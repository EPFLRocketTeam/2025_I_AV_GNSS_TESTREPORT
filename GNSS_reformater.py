import re
from datetime import datetime
import os

def reformater_fichier(fichier_entree):
    with open(fichier_entree, 'r', encoding='utf-8') as f:
        contenu = f.read()

    # Récupération de la première ligne pour générer le nom du fichier de sortie
    premiere_ligne = contenu.splitlines()[0]
    match_nom = re.search(
        r'time_delay_loop:\s*(\d+)\s*CpollingWait:\s*(\d+)\s*freq_nav:\s*(\d+)',
        premiere_ligne
    )

    if not match_nom:
        raise ValueError("Impossible de trouver les valeurs dans la première ligne pour générer le nom de fichier.")

    delay, cpolling, freq = match_nom.groups()
    date_str = datetime.now().strftime("%Y-%m-%d")
    base_nom = f"CP{cpolling}_delay{delay}_freq{freq}_{date_str}"
    fichier_sortie = base_nom + ".csv"

    # Vérifie si le fichier existe déjà et ajoute un suffixe numérique si besoin
    compteur = 2
    while os.path.exists(fichier_sortie):
        fichier_sortie = f"{base_nom}_{compteur}.csv"
        compteur += 1

    # Séparer les paquets GNSS
    paquets = contenu.split('=== DATA ===')

    lignes_reformatees = []

    for paquet in paquets:

        match = re.search(
            r"Lat:\s*(-?\d+)\s*\|\s*Lon:\s*(-?\d+).+?Alt:\s*(-?\d+).+?"
            r"Speed:\s*([-\d.]+).+?VelN:\s*([-\d.]+).+?"
            r"VelE:\s*([-\d.]+).+?VelD:\s*([-\d.]+).+?"
            r"CarrierSolution:\s*(\d+).+?Fix type:\s*(\d+).+?"
            r"Satellites:\s*(\d+).+?Loop\s+#(\d+)\s*\|\s*Durée:\s*(\d+)\s*µs\s*\|\s*Time to get data:\s*(\d+)",
            paquet, re.DOTALL
        )

        if match:
            lat, lon, alt, speed, velN, velE, velD, carrier, fix, sats, loop, duree, time_to_get_data = match.groups()
            ligne = f"#{loop};{duree};{time_to_get_data};{lat};{lon};{alt};{speed};{velN};{velE};{velD};{carrier};{fix};{sats};"
            lignes_reformatees.append(ligne)

    # Écriture dans le nouveau fichier
    with open(fichier_sortie, 'w', encoding='utf-8') as f:
        f.write('Loop;Time;time to get data;Lat;Long;Alt;Speed;velN;velE;velD;Carrier;fix;sats\n')
        for ligne in lignes_reformatees:
            f.write(ligne + '\n')

    print(f"Fichier sauvegardé sous : {fichier_sortie}")

# Utilisation
#reformater_fichier('3msdv_30hz.log')
#reformater_fichier('3msdv_20hz.log')
#reformater_fichier('3msdv_10hz.log')





