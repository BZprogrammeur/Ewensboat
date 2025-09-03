import re
import json

def txt_to_geojson(input_file, output_file):
    coordinates = []

    # Regex pour extraire Lat et Lon
    pattern = re.compile(r"Lat:\s*([-+]?\d+\.\d+)\s*°,\s*Lon:\s*([-+]?\d+\.\d+)")

    with open(input_file, "r") as f:
        for line in f:
            match = pattern.search(line)
            if match:
                lat = float(match.group(1))
                lon = float(match.group(2))
                coordinates.append([lon, lat])  # GeoJSON = [lon, lat]

    geojson_data = {
        "type": "FeatureCollection",
        "features": [
            {
                "type": "Feature",
                "properties": {},
                "geometry": {
                    "type": "LineString",
                    "coordinates": coordinates
                }
            }
        ]
    }

    with open(output_file, "w") as f:
        json.dump(geojson_data, f, indent=2)

    print(f"✅ Fichier GeoJSON généré : {output_file}")

# Exemple d'utilisation
txt_to_geojson("log1.txt", "trace.geojson")
